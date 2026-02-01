rmdir /s /q build-clang

rem CMake-Configuration with Ninja + clang-cl
cmake -B build-clang ^
  -G "Ninja Multi-Config" ^
  -DCMAKE_C_COMPILER="clang-cl.exe" ^
  -DCMAKE_CXX_COMPILER="clang-cl.exe" ^
  -DCOMPILE_COMMANDS=ON ^
  -DFFMPEG=OFF ^
  -DBINKDEC=ON ^
  -DSTANDALONE=OFF ^
  -DUSE_PRECOMPILED_HEADERS=OFF ^
  .

rem Build im Release-Mode
cmake --build build-clang --config Release > build.log 2>&1

pause
