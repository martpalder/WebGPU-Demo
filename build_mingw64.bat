@echo off
cmake . -B "build-mingw64"
cmake --build "build-mingw64"
pause
