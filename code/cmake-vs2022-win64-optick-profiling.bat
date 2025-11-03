del /s /q build
mkdir build
cmake -B build -G "Visual Studio 17" -A x64 -DOPTICK=ON .
pause