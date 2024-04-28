#include "LogLL.h"
using namespace LOG_LL;

#include <iostream>
using namespace std;

void ConsoleLogTest()
{
    LogLL log;
    log.AddColorConsole("logger-console");
    log.Init();
    LOG_LL_TRACE("logger-console", "TRACE log data:{}", "这是TRACE");
    LOG_LL_DEBUG("logger-console", "DEBUG log data:{}", "这是DEBUG");
    LOG_LL_INFO("logger-console", "INFO log data:{}", "这是INFO");
    LOG_LL_WARN("logger-console", "WARN log data:{}", "这是WARN");
    LOG_LL_ERROR("logger-console", "ERROR log data:{}", "这是ERROR");
    LOG_LL_CRITI("logger-console", "CRITI log data:{}", "这是CRITI");
}

// TODO
void RotatingFileLogTest()
{

}

// TODO
void DailyFileLogTest()
{

}

int main()
{
	//     //控制台（软件初始化）

	cout << "log test begin " << endl;

    ConsoleLogTest();

	system("pause");

	return 0;
}