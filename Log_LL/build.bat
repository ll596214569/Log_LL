mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A Win32 ..
cmake --build . --config Release

mkdir include lib
copy ..\LogLL.h .\include
xcopy ..\spdlog\include .\include /s
copy .\Release\LogLL_st.lib .\lib