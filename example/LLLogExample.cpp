#include "LogLL.h"
using namespace LOG_LL;

#include <iostream>
using namespace std;

int main()
{
	//     //控制台（软件初始化）

	cout << "log test begin " << endl;

	LogLL log;
	log.AddColorConsole("logger");
	log.Init();



	system("pause");

	return 0;
}