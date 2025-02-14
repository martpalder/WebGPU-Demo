@echo off
set RUST_BACKTRACE=full
gdb ".\build-mingw64\App.exe"
pause
