#include "LogLL.h"
using namespace LOG_LL;

#include "./spdlog/include/spdlog/async.h"
#include "./spdlog/include/spdlog/sinks/stdout_color_sinks.h"
#include "./spdlog/include/spdlog/sinks/basic_file_sink.h"
#include "./spdlog/include/spdlog/sinks/rotating_file_sink.h"
#include "./spdlog/include/spdlog/sinks/daily_file_sink.h"
#include "./spdlog/include/spdlog/details/thread_pool.h"

#include <iostream>
#include <thread>
#ifdef _WIN32
#include <Windows.h>
#endif
using namespace std;

LogLL::LogLL()
{
}

LogLL::~LogLL()
{
    if (m_bInit)
    {
        this->UnInit();
    }
}

bool LogLL::AddColorConsole(const char* pLoggerName, const OutLevel level)
{
    cout    << "[system-log-add-logger]: AddColorConsole, logName=" 
            << pLoggerName << " level= " 
            << OutLevelEnumToString(level) 
            << endl;

    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console->set_level((spdlog::level::level_enum)level);
    AddLogger(pLoggerName, console);
    return true;
}

bool LogLL::AddRotatingFile(const char* pLoggerName, const char* pFileName, const int nMaxFileSize, const int nMaxFile, const OutLevel level)
{
    
    cout    << "[system-log-add-logger]: AddRotatingFile, logName=" << pLoggerName 
            << " level=" << OutLevelEnumToString(level) 
            << " fileName=" << pFileName 
            << " maxFileSize=" << nMaxFileSize 
            << " maxFile=" << nMaxFile
            << endl;

    auto rFile = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(pFileName, nMaxFileSize, nMaxFile);
    rFile->set_level((spdlog::level::level_enum)level);
    AddLogger(pLoggerName, rFile);
    return true;
}

bool LogLL::AddDailyFile(const char* pLoggerName, const char* pFileName, const int nHour, const int nMinute, const OutLevel level)
{ 
    cout    << "[system-log-add-logger]: AddDailyFile, logName=" << pLoggerName 
            << " level=" << OutLevelEnumToString(level) 
            << " fileName=" << pFileName 
            << " update hour=" << nHour 
            << " update minute=" << nMinute
            << endl;

    auto dFile = std::make_shared<spdlog::sinks::daily_file_sink_mt>(pFileName, nHour, nMinute);
    dFile->set_level((spdlog::level::level_enum)level);
    AddLogger(pLoggerName, dFile);
    return true;
}

bool LogLL::InitConfig_ini(const char* pFileName)
{
    /*
    try
    {
    #ifdef _WIN32
        char wsz[256] = {0};
        _snprintf_s(wsz, 256, 255, "%s", pFileName);
        LPCSTR path = wsz;

        int nLoggerNum = GetPrivateProfileInt(L"CONFIG", L"LoggerNum", 0, path);
        char szTemp[256] = { 0 };

        for(size_t i = 0; i < nLoggerNum; ++i)
        {
            char wTemp[16] = {0};
            _snprintf_s(wTemp, 16, 15, "Logger_%d", i);
            LPCSTR wLog = wTemp;
            int nSinks = GetPrivateProfileInt(wLog, "sinksNum", 0, path);
            GetPrivateProfileString(wLog, "name", "", szTemp, 256, path);
            string logName = szTemp;
            memset(szTemp, 0, 255);

            for(size_t j = 0; j < nSinks; ++j)
            {
                memset(wTemp, 0, 16);
                _snprintf_s(wTemp, 16, 15, "sinks_%d_type", j);
                LPCSTR wSinkType = wTemp;
                GetPrivateProfileString(wLog, wSinkType, "", szTemp, 256, path);
                string strSinkType = szTemp;
                memset(szTemp, 0, 255);

                memset(wTemp, 0, 16);
                _snprintf_s(wTemp, 16, 15, "sinks_%d_level", j);
                LPCSTR wSinkLevel = wTemp;
                GetPrivateProfileString(wLog, wSinkLevel, "", szTemp, 256, path);
                auto SinkLevel = StringToOutLevelEnum(szTemp);
                memset(szTemp, 0, 255);

                memset(wTemp, 0, 16);
                _snprintf_s(wTemp, 16, 15, "sinks_%d_fileName", j);
                LPCSTR wSinkName = wTemp;

                if (strSinkType == "console") 
                {
                    AddColorConsole(logName.c_str(), SinkLevel);
                }
                else if (strSinkType == "r_file") 
                {
                    GetPrivateProfileString(wLog, wSinkName, "", szTemp, 256, path);
                    string strFileName = szTemp;

                    memset(wTemp, 0, 16);
                    _snprintf_s(wTemp, 16, 15, "sinks_%d_maxFileSize", j);
					int nMaxFileSize = GetPrivateProfileInt(wLog, wTemp, 0, path);

                    memset(wTemp, 0, 16);
                    _snprintf_s(wTemp, 16, 15, "sinks_%d_maxSize", j);
					int nMaxFile = GetPrivateProfileInt(wLog, wTemp, 0, path);

					AddRotatingFile(logName.c_str(), strFileName.c_str(), nMaxFileSize, nMaxFile, SinkLevel);
                }
                else if (strSinkType == "d_file") 
                {
                    GetPrivateProfileString(wLog, wSinkName, "", szTemp, 256, path);
                    string strFileName = szTemp;

                    memset(wTemp, 0, 16);
                    _snprintf_s(wTemp, 16, 15, "sinks_%d_hour", j);
					int nHour = GetPrivateProfileInt(wLog, wTemp, 0, path);

                    memset(wTemp, 0, 16);
                    _snprintf_s(wTemp, 16, 15, "sinks_%d_minute", j);
					int nMinute = GetPrivateProfileInt(wLog, wTemp, 0, path);

					AddDailyFile(logName.c_str(), strFileName.c_str(), nHour, nMinute, SinkLevel);
                }
            }

        }

        GetPrivateProfileString("CONFIG", "outputMode", "", szTemp, 256, path);
        string OutputMode = szTemp;
        memset(szTemp, 0, 255);

        GetPrivateProfileString("CONFIG", "outputFormat", "", szTemp, 256, path);
        string OutputFormat = szTemp;

        return Init(StringToOutModeEnum(OutputMode), OutputFormat);

    #else
        cout << " [system-init-config-error] init config Linux not support ! " << endl;
        return false;
    #endif
    }
    catch (std::exception& e)
    {
        cout << "[system-log-init-error]: AddConfig failed!, err:" << e.what() << endl;;
        return false;
    }
    catch (...)
    {
        cout << "[system-log-init-error]: AddConfig failed!, exception" << endl;
        return false;
    }*/
    return true;
}

bool LogLL::Init(const OutMode outMode, const string strLogFormat)
{
    if (m_bInit)
    {
        cout << "[system-log-init-error]: It's already initialized" << endl;;
        return false;
    }

    if (outMode == OutMode::ASYNC)//异步
    {
        cout << "[system-log-init-error]: mode=ASYNC" << endl;
        for (auto e : m_mapLoggerParam)
        {
            std::string strLogName = e.first;
            std::vector<spdlog::sink_ptr> vecSink(e.second);
            auto tp = std::make_shared<spdlog::details::thread_pool>(std::thread::hardware_concurrency(), 1);
            auto logger = std::make_shared<spdlog::async_logger>(strLogName, begin(vecSink), end(vecSink), tp, spdlog::async_overflow_policy::block);
            UpdateThreadPoolMap(strLogName, tp);
            //设置根日志输出等级
            logger->set_level(spdlog::level::trace);
            //遇到warn级别，立即flush到文件
            logger->flush_on(spdlog::level::warn);
            spdlog::register_logger(logger);
        }
    }
    else//同步
    {
        cout << "[system-log-init-error]: mode=SYNC" << endl;
        for (auto e : m_mapLoggerParam)
        {
            std::string strLogName = e.first;
            std::vector<spdlog::sink_ptr> vecSink(e.second);
            auto logger = std::make_shared<spdlog::logger>(strLogName, begin(vecSink), end(vecSink));
            //设置根日志输出等级
            logger->set_level(spdlog::level::trace);
            //遇到warn级别，立即flush到文件
            logger->flush_on(spdlog::level::warn);
            spdlog::register_logger(logger);
        }
    }

    //定时flush到文件，每三秒刷新一次
    spdlog::flush_every(std::chrono::seconds(3));
    //设置全局记录器的输出格式
    spdlog::set_pattern(strLogFormat);

    m_bInit = true;

    return true;
}

void LogLL::UnInit()
{
    spdlog::drop_all();
    spdlog::shutdown();
}

void LogLL::AddLogger(std::string strLoggerName, spdlog::sink_ptr pSink)
{
    auto iter = m_mapLoggerParam.find(strLoggerName);
    if (iter != m_mapLoggerParam.end())
    {
        iter->second.push_back(pSink);
    }
    else
    {
        std::vector<spdlog::sink_ptr> vecSink;
        vecSink.push_back(pSink);
        m_mapLoggerParam[strLoggerName] = vecSink;
    }
}

void LogLL::UpdateThreadPoolMap(std::string strLoggerName, std::shared_ptr<spdlog::details::thread_pool> pThreadPool)
{
    auto iter = m_mapAsyncThreadPool.find(strLoggerName);
    if (iter != m_mapAsyncThreadPool.end())
    {
        iter->second = (pThreadPool);
    }
    else
    {
        m_mapAsyncThreadPool[strLoggerName] = pThreadPool;
    }
}