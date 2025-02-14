@echo off
call "%EMSDK%\\emsdk_env.bat"
call emcmake cmake . -B "build-em"
call emcmake cmake --build "build-em"
pause
