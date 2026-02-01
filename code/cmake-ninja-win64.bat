rmdir /s /q build

rem CMake-Configuration with Ninja + Clang
cmake -B build ^
  -G "Ninja Multi-Config" ^
  -DCMAKE_C_COMPILER="clang.exe" ^
  -DCMAKE_CXX_COMPILER="clang++.exe" ^
  -DFFMPEG=OFF ^
  -DBINKDEC=ON ^
  -DSTANDALONE=OFF ^
  -DUSE_PRECOMPILED_HEADERS=OFF ^
  .

rem Build im Release-Mode
::cmake --build build --config Release

pause
