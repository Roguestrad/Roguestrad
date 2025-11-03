del /s /q build
mkdir build
cmake -B build -G "Visual Studio 17" -A x64 -DFFMPEG=ON -DBINKDEC=OFF -DRETAIL=ON .
pause