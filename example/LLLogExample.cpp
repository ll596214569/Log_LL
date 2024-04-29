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

void RotatingFileLogTest()
{
    LogLL log;
    log.AddRotatingFile("logger-Rotating", "rotating.log");
    log.Init();

    LOG_LL_TRACE("logger-Rotating", "TRACE log data:{}", "这是TRACE");
    LOG_LL_DEBUG("logger-Rotating", "DEBUG log data:{}", "这是DEBUG");
    LOG_LL_INFO("logger-Rotating", "INFO log data:{}", "这是INFO");
    LOG_LL_WARN("logger-Rotating", "WARN log data:{}", "这是WARN");
    LOG_LL_ERROR("logger-Rotating", "ERROR log data:{}", "这是ERROR");
    LOG_LL_CRITI("logger-Rotating", "CRITI log data:{}", "这是CRITI");
}

void DailyFileLogTest()
{
    LogLL log;
    log.AddDailyFile("logger-Daily", "daily.log", 9, 0);
    log.Init();

    LOG_LL_TRACE("logger-Daily", "TRACE log data:{}", "这是TRACE");
    LOG_LL_DEBUG("logger-Daily", "DEBUG log data:{}", "这是DEBUG");
    LOG_LL_INFO("logger-Daily", "INFO log data:{}", "这是INFO");
    LOG_LL_WARN("logger-Daily", "WARN log data:{}", "这是WARN");
    LOG_LL_ERROR("logger-Daily", "ERROR log data:{}", "这是ERROR");
    LOG_LL_CRITI("logger-Daily", "CRITI log data:{}", "这是CRITI");
}

void IniConfigLogTest()
{
    LogLL log;
    log.InitConfig_ini("test2.ini");
}

int main()
{
	//     //控制台（软件初始化）

	cout << "log test begin " << endl;
    /*
    ConsoleLogTest();

    RotatingFileLogTest();

    DailyFileLogTest();
    */
    IniConfigLogTest();
	system("pause");

	return 0;
}