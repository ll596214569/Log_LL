

#ifndef _LOG_LL_H
#define _LOG_LL_H

#include "spdlog/spdlog.h"

#include <map>
#include <string>
#include <atomic>

namespace LOG_LL {

    // 日志输出模式
    enum class OutMode : size_t
    {
        SYNC    = 0,    // 同步模式
        ASYNC    = 1,    // 异步模式
    };
    static OutMode StringToOutModeEnum(const std::string& strMode)
    {
        if (strMode == "SYNC") 
        {   
            return OutMode::SYNC;
        }
        if (strMode == "ASYNX")
        {
            return OutMode::ASYNC;
        }
        return OutMode::SYNC;
    }

    static std::string OutModeEnumToString(const OutMode mode) 
    {
        if (mode == OutMode::ASYNC) 
        {   
            return "SYNC";
        }
        return "ASYNC";
    }

    // 日志输出等级
    enum class OutLevel : size_t 
    {
        LEVEL_TRACE = 0,
        LEVEL_DEBUG = 1,
        LEVEL_INFO = 2,
        LEVEL_WARN = 3,
        LEVEL_ERROR = 4,
        LEVEL_CRITI = 5,
        LEVEL_OFF = 6,
    };
    static OutLevel StringToOutLevelEnum(const std::string& strLevel)
    {
        if (strLevel == "TRACE")
        {
           return OutLevel::LEVEL_TRACE;
        }
        else if (strLevel == "DEBUG")
        {
            return OutLevel::LEVEL_DEBUG;
        }
        else if (strLevel == "INFO")
        {
            return OutLevel::LEVEL_INFO;
        }
        else if (strLevel == "WARN")
        {
            return OutLevel::LEVEL_WARN;
        }
        else if (strLevel == "ERROR")
        {
            return OutLevel::LEVEL_ERROR;
        }
        else if (strLevel == "CRITI")
        {
            return OutLevel::LEVEL_CRITI;
        }

        return OutLevel::LEVEL_TRACE;
    }

    static std::string OutLevelEnumToString(const OutLevel level) 
    {
        if (level == OutLevel::LEVEL_TRACE) 
        {   
            return "TRACE";
        }
        else if (level == OutLevel::LEVEL_DEBUG) 
        {   
            return "DEBUG";
        }
        else if (level == OutLevel::LEVEL_INFO) 
        {   
            return "INFO";
        }
        else if (level == OutLevel::LEVEL_WARN) 
        {   
            return "WARN";
        }
        else if (level == OutLevel::LEVEL_ERROR) 
        {   
            return "ERROR";
        }
        else if (level == OutLevel::LEVEL_CRITI) 
        {   
            return "CRITI";
        }
        else if (level == OutLevel::LEVEL_OFF) 
        {   
            return "OFF";
        }
        return "LEVEL_ERROR";
    }

class LogLL
{

public:
    LogLL();
    ~LogLL();

    // 控制台
    //@param[in]        pLoggerName: 记录器名称
    //@param[in]        eLevel: 日志输出等级
    //@return           添加是否成功
    bool AddColorConsole(const char* pLoggerName, const OutLevel level = OutLevel::LEVEL_TRACE);

    // 旋转文件（到了指定的大小就会重新生成文件）
    //@param[in]        pLoggerName: 记录器名称
    //@param[in]        pFileName: 日志名称
    //@param[in]        nMaxFileSize: 生成的文件内容最大容量，单位byte
    //@param[in]        nMaxFile: 生成的文件最大数量
    //@param[in]        eLevel: 日志输出等级
    //@return           添加是否成功
    bool AddRotatingFile(const char* pLoggerName, const char* pFileName, const int nMaxFileSize = 1024 * 1024 * 10,
                            const int nMaxFile = 10, const OutLevel level = OutLevel::LEVEL_TRACE);

    // 日期文件（在每天的指定时间生成一个日志文件, 文件名以日期命名）
    //@param[in]        pLoggerName: 记录器名称
    //@param[in]        pFileName: 日志名称
    //@param[in]        nHour: 指定生成时间的时
    //@param[in]        nMinute: 指定生成时间的分
    //@param[in]        eLevel: 日志输出等级
    //@return           添加是否成功
    bool AddDailyFile(const char* pLoggerName, const char* pFileName, const int nHour, const int nMinute, const OutLevel eLevel = OutLevel::LEVEL_TRACE);

    // 配置文件初始化
    //@param[in]        pFileName: 配置文件路径
    bool InitConfig_ini(const char* pFileName);

    // 参数配置初始化
    //@param[in]        outMode: 日志输出模式
    //@param[in]        strLogFormat: 日志输出等级
    bool Init(const OutMode outMode = OutMode::SYNC, const std::string strLogFormat = "%^[ %Y-%m-%d %H:%M:%S.%e ] <thread %t> [%n] [%l]\n%@,%!\n%v%$\n");

private:
    // 回收
    void UnInit();

    // 更新记录器需要的sink容器
    void AddLogger(std::string strLoggerName, spdlog::sink_ptr pSink);

    // 更新异步记录器需要的线程池
    void UpdateThreadPoolMap(std::string strLoggerName, std::shared_ptr<spdlog::details::thread_pool> pThreadPool);

private:
    std::atomic<bool> m_bInit;

    //<logger名称，logger需要初始化的sinks>：存储初始化前的sink（存在一个logger有多个sink，且有多个logger的情况）
    std::map<std::string, std::vector<spdlog::sink_ptr>> m_mapLoggerParam; 

    //<logger名称，logger异步需要的线程池>：由于记录器获取线程池的weak_ptr，所以线程池对象必须比记录器对象的寿命长
    std::map<std::string, std::shared_ptr<spdlog::details::thread_pool>> m_mapAsyncThreadPool;
};

#define LOG_LL_TRACE(loggerName, ...) SPDLOG_LOGGER_CALL(spdlog::get(loggerName), spdlog::level::trace, __VA_ARGS__)
#define LOG_LL_DEBUG(loggerName, ...) SPDLOG_LOGGER_CALL(spdlog::get(loggerName), spdlog::level::debug, __VA_ARGS__)
#define LOG_LL_INFO(loggerName, ...) SPDLOG_LOGGER_CALL(spdlog::get(loggerName), spdlog::level::info, __VA_ARGS__)
#define LOG_LL_WARN(loggerName, ...) SPDLOG_LOGGER_CALL(spdlog::get(loggerName), spdlog::level::warn, __VA_ARGS__)
#define LOG_LL_ERROR(loggerName, ...) SPDLOG_LOGGER_CALL(spdlog::get(loggerName), spdlog::level::err, __VA_ARGS__)
#define LOG_LL_CRITI(loggerName, ...) SPDLOG_LOGGER_CALL(spdlog::get(loggerName), spdlog::level::critical, __VA_ARGS__)

};

#endif
