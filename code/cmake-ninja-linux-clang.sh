rm -f idlib/precompiled.h.gch
rm -f tools/compilers/precompiled.h.gch
rm -f tools/typeinfo/precompiled.h.gch
rm -rf build
mkdir build

# CMake-Configuration with Ninja + clang
cmake -B build \
  -G "Ninja Multi-Config" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DCOMPILE_COMMANDS=ON \
  -DFFMPEG=OFF \
  -DBINKDEC=ON \
  -DSTANDALONE=OFF \
  -DUSE_PRECOMPILED_HEADERS=OFF \
  .
