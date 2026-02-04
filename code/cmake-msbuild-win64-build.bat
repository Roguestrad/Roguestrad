del /s /q build
mkdir build
cmake -B build -G "Visual Studio 17" -A x64 -DFFMPEG=OFF -DBINKDEC=ON -DSTANDALONE=OFF .
cmake --build build --config Release
pause
