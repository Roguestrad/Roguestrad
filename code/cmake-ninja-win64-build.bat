rmdir /s /q build

rem CMake-Configuration with Ninja + clang-cl
cmake -B build ^
  -G "Ninja Multi-Config" ^
  -DCMAKE_C_COMPILER="clang-cl.exe" ^
  -DCMAKE_CXX_COMPILER="clang-cl.exe" ^
  -DCOMPILE_COMMANDS=ON ^
  -DFFMPEG=OFF ^
  -DBINKDEC=ON ^
  -DSTANDALONE=OFF ^
  -DUSE_PRECOMPILED_HEADERS=ON ^
  .

rem Build in Release-Mode
cmake --build build --config Release

pause
