@echo off
cd build-mingw64
cmake -G"MinGW Makefiles" .. ^
-DWEBGPU_BACKEND_WGPU=1 ^
-DCMAKE_BUILD_TYPE=Debug
pause
