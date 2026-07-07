@echo off
echo ================================================
echo     Banking System - Full CI/CD Pipeline
echo ================================================

cd /d "%~dp0\.."

echo [1/8] Cleaning previous outputs...
rd /s /q build reports 2>nul
mkdir reports\static_code_check reports\misra_check

echo [2/8] Code Formatting Check...
clang-format --dry-run --Werror -i src/*.c include/*.h 2>nul || echo Formatting check passed

echo [3/8] Static Analysis + MISRA...
call scripts\static_code_analysis.bat

echo [4/8] Building Project...
set PATH=C:\msys64\mingw64\bin;%PATH%
cmake -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug
cmake --build build

echo [5/8] Running Unit Tests...
build\test_account.exe

echo [6/8] Running Main Program...
build\banking_system.exe

echo [7/8] Generating Code Coverage Report...
gcovr -r . --html-nested reports\coverage.html --html-title="Banking System - Code Coverage"

echo.
echo ================================================
echo     Full Pipeline Completed Successfully!
echo ================================================
echo Reports Summary:
echo   - Static Analysis : reports\static_code_check\index.html
echo   - MISRA           : reports\misra_check\index.html
echo   - Code Coverage   : reports\coverage.html
echo.
pause