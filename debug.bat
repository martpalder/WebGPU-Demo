@echo off
set RUST_BACKTRACE=full
cd build-mingw64
gdb .\App
pause
