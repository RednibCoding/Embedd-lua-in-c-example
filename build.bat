@echo off
gcc -o demo.exe main.c -L"deps/lua/win32" -llua54 -lm