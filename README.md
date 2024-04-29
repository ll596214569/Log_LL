# Log_LL
基于spdlog的二次封装，目前仅支持windows

## Log_LL

基于spdlog的二次封装，目前仅支持windows使用，环境为 windows + VS2019 工程，生成对象为MD/MDD静态库；使用时需要导入 LogLL.h & spdlog/include

* windows版本：Windows 10 专业版 22H2
* VS2019: Microsoft Visual Studio Community 2019 版本 16.11.26

引用开源项目： 
- [spdlogv1.x：C++ 日志库](https://github.com/gabime/spdlog.git)
- [SimpleIni：C++跨平台ini解析库；](https://github.com/brofield/simpleini)

目前提供三种记录日志方式：
* 控制台输出：接口为 AddColorConsole，将日志都输出到控制台；
* 按大小生成日志文件： 接口为 AddRotatingFile，每当日志文件到了指定的大小就会重新生成文件；
* 按时间生成日志文件： 接口为 AddDailyFile，每当日志文件到了指定的时间就会重新生成文件；

提供两种初始化启动接口：
* 指定参数初始化： 接口为 Init；
* 通过ini配置文件初始化： 接口为 InitConfig_ini；

使用ini主要是为了兼容目前做的一个项目的旧配置文件，后续会考虑增加支持json等文件类型；

## example

LogLL 静态库单元测试，VS2019 工程，通过项目属性引入 .lib

*test2.ini 为 InitConfig_ini 接口所支持的ini配置文件示例，test.ini 与 test2.ini 区别主要在于注释的位置，测试发现 SimpleIni 似乎无法区分注释与值相连的情况，例如："sinks_1_fileName = fileName; [string] 文件名称"， 此时 sinks_1_fileName 读出来的 value 是"fileName; [string] 文件名称"*
