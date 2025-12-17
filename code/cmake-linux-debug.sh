rm -f idlib/precompiled.h.gch
rm -f tools/compilers/precompiled.h.gch
rm -f tools/typeinfo/precompiled.h.gch
rm -rf build
mkdir build
cmake -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DFFMPEG=ON -DBINKDEC=OFF -DCMAKE_CXX_COMPILER=g++ -DUSE_PRECOMPILED_HEADERS=OFF .
