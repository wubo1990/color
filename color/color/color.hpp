// color.hpp
//

#include <cstdio>
#include <vector>
using namespace std;

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/shape.hpp"

using namespace cv;

namespace color
{
	class KmeansQuerier
	{

	private:

		int clusterCount;

	public:

		//Default cluster number is 2
		KmeansQuerier(void)
		{
			clusterCount = 2;
		}

		//read image
		string Query(const char* pathname) const
		{
			//input image
			Mat image = imread(cv::String(pathname));
			
			//invaild input image
			if (image.empty())
			{
				printf("can not open file: %s\n", pathname);
				return "";
			}
			return Query(image);
		}

		//set the number of color, default is 1
		void SetParameter(const char* name, const char* value)
		{
			if (strcmp("C", name) == 0)
			{
				clusterCount = atol(value) + 1;
			}
		}

	private:

		//checking the color
		string Query(Mat& image) const
		{
			//output string
			string clr = "";

			//Data for clustering
			Mat pixel(image.rows * image.cols, 1, CV_32FC3);
			
			//Array to store the cluster indices 
			vector<int> label;

			//output matrix for cluster center
			Mat center(clusterCount, 1, CV_32FC3);

			
			for (int i = 0; i < image.rows; ++i)
			{
				for (int j = 0; j < image.cols; ++j)
				{
					unsigned char* s = image.data + image.step * i + 3 * j;
					float* t = (float*)(pixel.data + pixel.step * (image.cols * i + j));
					t[0] = s[0];
					t[1] = s[1];
					t[2] = s[2];
				}
			}

			//define the criteria and apply kmeans
			kmeans(pixel, clusterCount, label,
				TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 128, 1.0),
				4, KmeansFlags::KMEANS_PP_CENTERS, center);

			vector<ClusterCenter> cc(clusterCount);
			for (int i = 0; i < label.size(); ++i)
			{
				++cc[label[i]].count;
			}
			
			//get the color
			for (int i = 0; i < clusterCount; ++i)
			{
				float* p = (float*)(center.data + pixel.step * i);
				cc[i].b = p[0] + 0.5;
				cc[i].g = p[1] + 0.5;
				cc[i].r = p[2] + 0.5;
				cc[i].gray = (p[0] + p[1] + p[2]) / 3.0;
			}

			//sort the cluster center
			sort(cc.begin(), cc.end());
			
			//save the result in string
			int k = 1;
			for (int i = 0; i < clusterCount; ++i)
			{
				if (cc[i].gray < 220)
				{
					char s[32];
					sprintf(s, "%X%X%02X\n", cc[i].b, cc[i].g, cc[i].r);
					clr += string(s);
					++k;
				}
				if (k >= clusterCount)
				{
					break;
				}
			}

			return clr;
		}

	protected:

		//define the cluster center
		struct ClusterCenter
		{

			ClusterCenter(void)
			{
				r = 0;
				g = 0;
				b = 0;
				gray = 0.0F;
				count = 0;
			}

			bool operator < (const ClusterCenter& x)
			{
				return gray * x.count < x.gray * count;
			}

			unsigned char r, g, b;
			float gray;
			int count;

		};

	};


	KmeansQuerier* CreateColorQuerier()
	{
		return new KmeansQuerier();
	}

} // namespace color
