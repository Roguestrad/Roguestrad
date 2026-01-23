del /s /q build
mkdir build
cmake -B build -G "Visual Studio 17" -A x64 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
pause
