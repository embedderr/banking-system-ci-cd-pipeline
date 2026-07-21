@echo off
echo ================================================
echo     Checking Requirements
echo ================================================

set "PYCMD="
for /f "delims=" %%P in ('where python 2^>nul') do (
    echo %%P | findstr /i "msys64" >nul
    if errorlevel 1 (
        if not defined PYCMD set "PYCMD=%%P"
    )
)
if not defined PYCMD set "PYCMD=python"

echo Checking GCC...
gcc --version >nul 2>&1 || echo [WARNING] GCC not found. Install MSYS2 MinGW.

echo Checking CMake...
cmake --version >nul 2>&1 || echo [WARNING] CMake not found.

echo Checking pygments...
%PYCMD% -c "import pygments" >nul 2>&1 || (
    echo Installing pygments...
    %PYCMD% -m pip install pygments
)

echo Checking gcovr...
%PYCMD% -c "import gcovr" >nul 2>&1 || (
    echo Installing gcovr...
    %PYCMD% -m pip install gcovr
)

echo Checking clang-format...
clang-format --version >nul 2>&1 || echo [WARNING] clang-format not found. Install LLVM.

echo Checking cppcheck...
cppcheck --version >nul 2>&1 || echo [WARNING] cppcheck not found.

echo.
echo Requirements check completed.
echo ================================================