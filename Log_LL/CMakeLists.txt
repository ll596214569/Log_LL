cmake_minimum_required(VERSION 3.15)
project(LogLL VERSION 1.0)

# 添加第三方库源码
add_subdirectory(./spdlog)
 
# 定义源码
set(SOURCES ./LogLL.cpp)

#添加头文件路径
include_directories(./spdlog/include)
 
#windows下msvc编译时，使用动态运行时库
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
 
# 添加源文件到静态库
add_library(${PROJECT_NAME}_st STATIC ${SOURCES})

# 链接第三方库
target_link_libraries(${PROJECT_NAME}_st spdlogd)

# install规则
if(WIN32)
    message("Running on Windows")
else()
    message("Not running on Windows")

    # 将include目录添加到静态库的头文件路径中
    target_include_directories(${PROJECT_NAME}_st PUBLIC ./spdlog/include)
    target_include_directories(${PROJECT_NAME}_st PUBLIC .LogLL.h)

    # 创建文件夹
    message(out_dir: ${PROJECT_BINARY_DIR})

    # 库安装路径
    install(FILES ./LogLL.h DESTINATION ${PROJECT_BINARY_DIR}/include)
    install(DIRECTORY ./spdlog/include DESTINATION ${PROJECT_BINARY_DIR})
    install(TARGETS ${PROJECT_NAME}_st ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/lib)
endif()



