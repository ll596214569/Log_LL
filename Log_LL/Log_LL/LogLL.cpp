#include "LogLL.h"
using namespace LOG_LL;

#include "./spdlog/include/spdlog/async.h"
#include "./spdlog/include/spdlog/sinks/stdout_color_sinks.h"
#include "./spdlog/include/spdlog/sinks/basic_file_sink.h"
#include "./spdlog/include/spdlog/sinks/rotating_file_sink.h"
#include "./spdlog/include/spdlog/sinks/daily_file_sink.h"
#include "./spdlog/include/spdlog/details/thread_pool.h"

#include "./include/SimpleIni/Simpleini.h"

#include <iostream>
#include <thread>
#ifdef _WIN32
#include <Windows.h>
#endif
using namespace std;

static const std::string _STR_INT_MIN = "-2147483647"; // INT_MIN + 1
static const std::string _STR_EMPTY = "";

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
    try
    {
        CSimpleIniA ini;
        SI_Error rc = ini.LoadFile(pFileName);
        if (rc < 0) 
        {
            cout << "[system-log-init-iniConfig-error]: open ini config file failed!, err file :" << pFileName << endl;;
            return false;
        }
        ini.SetMultiKey(false);

        auto GetIniInt = [&](const string& config, const string& key) -> int {
            return std::stoi(ini.GetValue(config.c_str(), key.c_str(), _STR_INT_MIN.c_str()));
        };

        auto GetIniString = [&](const string& config, const string& key) -> std::string {
            return ini.GetValue(config.c_str(), key.c_str(), _STR_EMPTY.c_str());
        };

        int nLoggerNum = GetIniInt("CONFIG", "LoggerNum");
        for (auto i = 0; i < nLoggerNum; ++i)
        {
            char szTemp[16] = { 0 };
            _snprintf_s(szTemp, 16, 15, "Logger_%d", i);
            std::string strLogger(szTemp);
            int nSinks = GetIniInt(strLogger, "sinksNum");
            std::string name = GetIniString(strLogger, "outputMode");

            for (auto j = 0; j < nSinks; ++j)
            {
                ::memset(szTemp, 0, 16);
                _snprintf_s(szTemp, 16, 15, "sinks_%d_type", j);
                std::string strSinkType = szTemp;

                ::memset(szTemp, 0, 16);
                _snprintf_s(szTemp, 16, 15, "sinks_%d_level", j);
                auto SinkLevel = StringToOutLevelEnum(std::string(szTemp));

                ::memset(szTemp, 0, 16);
                _snprintf_s(szTemp, 16, 15, "sinks_%d_fileName", j);
                auto SinkNameKey = std::string(szTemp);

                if (strSinkType == "console")
                {
                    AddColorConsole(name.c_str(), SinkLevel);
                }
                else if (strSinkType == "r_file")
                {
                    std::string strSinkName = GetIniString(strLogger, SinkNameKey.c_str());

                    ::memset(szTemp, 0, 16);
                    _snprintf_s(szTemp, 16, 15, "sinks_%d_maxFileSize", j);
                    int nMaxFileSize = GetIniInt(strLogger, szTemp);

                    ::memset(szTemp, 0, 16);
                    _snprintf_s(szTemp, 16, 15, "sinks_%d_maxSize", j);
                    int nMaxFile = GetIniInt(strLogger, szTemp);

                    AddRotatingFile(strLogger.c_str(), strSinkName.c_str(), nMaxFileSize, nMaxFile, SinkLevel);
                }
                else if (strSinkType == "d_file")
                {
                    std::string strSinkName = GetIniString(strLogger, SinkNameKey.c_str());
                    string strFileName = szTemp;

                    ::memset(szTemp, 0, 16);
                    _snprintf_s(szTemp, 16, 15, "sinks_%d_hour", j);
                    int nHour = GetIniInt(strLogger, szTemp);

                    ::memset(szTemp, 0, 16);
                    _snprintf_s(szTemp, 16, 15, "sinks_%d_minute", j);
                    int nMinute = GetIniInt(strLogger, szTemp);

                    AddDailyFile(strLogger.c_str(), strFileName.c_str(), nHour, nMinute, SinkLevel);
                }
            }
        }

        std::string OutputMode = GetIniString("CONFIG", "outputMode");
        std::string OutputFormat = GetIniString("CONFIG", "outputFormat");
        return this->Init(StringToOutModeEnum(OutputMode), OutputFormat);
    }
    catch (std::exception& e)
    {
        cout << "[system-log-init-error]: AddConfig failed!, err:" << e.what() << endl;
        return false;
    }
    catch (...)
    {
        cout << "[system-log-init-error]: AddConfig failed!, exception" << endl;
        return false;
    }
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