rm -f idlib/precompiled.h.gch
rm -f tools/compilers/precompiled.h.gch
rm -f tools/typeinfo/precompiled.h.gch
rm -rf build
mkdir build

# CMake-Configuration with Ninja + clang
cmake -B build \
  -G "Ninja Multi-Config" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=gcc \
  -DCMAKE_CXX_COMPILER=g++ \
  -DCOMPILE_COMMANDS=ON \
  -DFFMPEG=OFF \
  -DBINKDEC=ON \
  -DSTANDALONE=OFF \
  -DUSE_PRECOMPILED_HEADERS=ON \
  -DBUILD_FREETYPE=ON \
  .
