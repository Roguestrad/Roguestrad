astyle.exe -v --formatted --options=astyle-options.ini --exclude="libs" --exclude="extern" --exclude="game/gamesys/GameTypeInfo.h" --recursive *.h
astyle.exe -v --formatted --options=astyle-options.ini --exclude="libs" --exclude="extern" --exclude="game/gamesys/SysCvar.cpp" --exclude="game/gamesys/Callbacks.cpp" --exclude="game/gamesys/GameTypeInfo.cpp" --exclude="engine/sys/win32/win_cpu.cpp" --recursive *.cpp

astyle.exe -v -Q --options=astyle-options.ini --recursive engine/shaders/*.hlsl
pause
