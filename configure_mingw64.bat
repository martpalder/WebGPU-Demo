@echo off
cd build-mingw64
cmake -G"MinGW Makefiles" .. ^
-DWEBGPU_BACKEND=WGPU ^
-DCMAKE_BUILD_TYPE=Debug
pause
