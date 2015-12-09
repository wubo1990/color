// color.cpp
//

#include "stdafx.h"

#include <cstdio>
#include <map>
using namespace std;

#include "color.hpp"


namespace color
{

	class ColorQueryTask
	{

	private:

		vector<pair<string, string>> config;

		KmeansQuerier* querier;


	public:

		ColorQueryTask(void)
		{
			config.clear();
			querier = NULL;
		}

		int Run(int argc, _TCHAR* argv[])
		{
			//input are less than two
			if (argc < 2)
			{
				printf("Usage: color.exe <pathname> [config]\n");
				return 0;
			}

			//invaild input parameters
			if (!ParseParameter(argc, argv))
			{
				return -1;
			}

			//creat the color querier
			querier = color::CreateColorQuerier();
			
			//vaild input
			if (querier != NULL)
			{
				for (int i = 0; i < config.size(); ++i)
				{
					querier->SetParameter(config[0].first.c_str(), config[0].second.c_str());
				}
				string r = querier->Query(argv[1]);

				if (r.size() > 0)
				{
					printf("color [bbggrr]\n%s", r.c_str());
				}
			}

			return 0;
		}

	private:

		//parse the input parameter
		//Input should be "C=n" n is the number of the color
		bool ParseParameter(int argc, _TCHAR* argv[])
		{
			char name[1024], value[1024];

			string pathname = argv[1];
			char* p = strtok(&pathname[0], "C=");
			if (p != NULL)
			{
				p = strtok(NULL, "C=");
				if (p != NULL && sscanf(p, "%[0-9]", value) == 1)
				{
					config.push_back(pair<string, string>("C", value));
				}
			}

			//check the input for numbers of color
			for (int i = 2; i < argc; ++i)
			{
				if (sscanf(argv[i], "%[^=]=%s", name, value) == 2)
				{
					config.push_back(pair<string, string>(name, value));
				}
				else
				{
					printf("invalid parameter format: %s\n", argv[i]);
					return false;
				}
			}
		}

	};

}

int _tmain(int argc, _TCHAR* argv[])
{
	color::ColorQueryTask task;

	return task.Run(argc, argv);
}


