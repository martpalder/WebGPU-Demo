@echo off
call "%EMSDK%\\emsdk_env.bat"
cd build-em
call emcmake cmake .. ^
-DWEBGPU_BACKEND=EMSCRIPTEN
pause
