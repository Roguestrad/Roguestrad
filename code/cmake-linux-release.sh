rm -f idlib/precompiled.h.gch
rm -f tools/compilers/precompiled.h.gch
rm -f tools/typeinfo/precompiled.h.gch
rm -rf build
mkdir build
CC=clang CXX=clang++ cmake -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DONATIVE=ON -DFFMPEG=OFF -DBINKDEC=ON .
