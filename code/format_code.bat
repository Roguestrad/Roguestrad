@echo off
setlocal

:: Path to Git Bash (adjust if necessary)
set "GIT_BASH=C:\Program Files\Git\bin\sh.exe"

:: Check if Git Bash is available
if not exist "%GIT_BASH%" (
    echo ERROR: Git Bash not found at "%GIT_BASH%". Please install Git or adjust the path.
    exit /b 1
)

:: Make sure if / else statements are wrapped with braces
::astyle.exe -v --formatted --options=astyle-header.ini --exclude="libs" --exclude="extern" --recursive *.h
::astyle.exe -v --formatted --options=astyle-cpp.ini --exclude="libs" --exclude="extern" --recursive *.c
::astyle.exe -v --formatted --options=astyle-cpp.ini --exclude="libs" --exclude="extern" --exclude="game/gamesys/SysCvar.cpp" --exclude="game/gamesys/Callbacks.cpp" --exclude="game/win32/win_cpu.cpp" --recursive *.cpp

astyle.exe -v -Q --options=astyle-cpp.ini --recursive engine/shaders/*.hlsl

:: Run the shell script using Git Bash
echo Running clang-format via format_code.sh...
"%GIT_BASH%" -c "./format_code.sh"

if %ERRORLEVEL% neq 0 (
    echo ERROR: format_code failed to execute.
    exit /b 1
)

echo Done!
endlocal
pause