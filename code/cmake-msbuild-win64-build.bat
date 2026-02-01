del /s /q build
mkdir build
cmake -B build -G "Visual Studio 17" -A x64 -DFFMPEG=OFF -DBINKDEC=ON -DUSE_PRECOMPILED_HEADERS=ON .
cmake --build build --config Release
pause
