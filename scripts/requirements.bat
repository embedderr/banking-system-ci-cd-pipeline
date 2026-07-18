@echo off
echo ================================================
echo     Checking Requirements
echo ================================================

echo Checking GCC...
gcc --version >nul 2>&1 || echo [WARNING] GCC not found. Install MSYS2 MinGW.

echo Checking CMake...
cmake --version >nul 2>&1 || echo [WARNING] CMake not found.

echo Checking gcovr...
python -c "import gcovr" >nul 2>&1 || (
    echo Installing gcovr...
    python -m pip install gcovr
)

echo Checking clang-format...
clang-format --version >nul 2>&1 || echo [WARNING] clang-format not found. Install LLVM.

echo Checking cppcheck...
cppcheck --version >nul 2>&1 || echo [WARNING] cppcheck not found.

echo.
echo Requirements check completed.
echo ================================================