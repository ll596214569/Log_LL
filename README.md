# Log_LL
基于spdlog的二次封装，目前仅支持windows

## Log_LL

基于spdlog的二次封装，目前仅支持windows使用，环境为 windows + VS2019 工程，生成对象为MD/MDD静态库；使用时需要引用LogLL.h & spdlog/include

* windows版本：Windows 10 专业版 22H2
* VS2019: Microsoft Visual Studio Community 2019 版本 16.11.26

引用开源项目： 
- [spdlogv1.x：C++ 日志库](https://github.com/gabime/spdlog.git)
- [SimpleIni：C++跨平台ini解析库；](https://github.com/brofield/simpleini)

## example

LogLL 静态库单元测试，VS2019 工程，通过项目属性引入 .lib
