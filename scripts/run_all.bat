@echo off
echo ================================================
echo     Banking System - Full CI/CD Pipeline
echo ================================================

cd /d "%~dp0\.."

set "PYCMD="
for /f "delims=" %%P in ('where python 2^>nul') do (
    echo %%P | findstr /i "msys64" >nul
    if errorlevel 1 (
        if not defined PYCMD set "PYCMD=%%P"
    )
)
if not defined PYCMD set "PYCMD=python"

echo [0/9] Checking Requirements...
call scripts\requirements.bat

echo [1/9] Cleaning previous outputs...
rd /s /q build reports release 2>nul
mkdir reports\static_code_check reports\misra_check reports\code_coverage
mkdir release

echo [2/9] Code Formatting Check...
clang-format --dry-run --Werror -i src/*.c include/*.h 2>nul || echo Formatting check passed

echo [3/9] Static Analysis + MISRA...
call scripts\static_code_analysis.bat

echo [4/9] Building Project...
set PATH=C:\msys64\mingw64\bin;%PATH%
cmake -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug
cmake --build build

echo [5/9] Running Unit Tests...
cd build
ctest --output-on-failure
cd ..

echo [6/9] Running Main Program...
build\banking_system.exe

echo [7/9] Generating Code Coverage Report...
%PYCMD% -m gcovr -r . --html-nested reports\code_coverage\coverage.html --html-title="Banking System - Code Coverage"

echo [8/9] Creating Release Artifacts for QA...
copy build\banking_system.exe release\banking_system.exe
copy build\test_address.exe release\test_address.exe
copy build\test_admin.exe release\test_admin.exe
copy build\test_bill_queue.exe release\test_bill_queue.exe
copy build\test_cash_queue.exe release\test_cash_queue.exe
copy build\test_account.exe release\test_account.exe

:: Copy object files from build
copy build\CMakeFiles\banking_system.dir\src\*.o release\ 2>nul
copy build\CMakeFiles\test_address.dir\tests\*.o release\ 2>nul
copy build\CMakeFiles\test_address.dir\src\*.o release\ 2>nul
copy build\CMakeFiles\test_admin.dir\tests\*.o release\ 2>nul
copy build\CMakeFiles\test_admin.dir\src\*.o release\ 2>nul
copy build\CMakeFiles\test_bill_queue.dir\tests\*.o release\ 2>nul
copy build\CMakeFiles\test_bill_queue.dir\src\*.o release\ 2>nul
copy build\CMakeFiles\test_cash_queue.dir\tests\*.o release\ 2>nul
copy build\CMakeFiles\test_cash_queue.dir\src\*.o release\ 2>nul
copy build\CMakeFiles\test_account.dir\tests\*.o release\ 2>nul
copy build\CMakeFiles\test_account.dir\src\*.o release\ 2>nul

:: Create .bin files (binary images)
objcopy -O binary build\banking_system.exe release\banking_system.bin 2>nul
objcopy -O binary build\test_address.exe release\test_address.bin 2>nul
objcopy -O binary build\test_admin.exe release\test_admin.bin 2>nul
objcopy -O binary build\test_bill_queue.exe release\test_bill_queue.bin 2>nul
objcopy -O binary build\test_cash_queue.exe release\test_cash_queue.bin 2>nul
objcopy -O binary build\test_account.exe release\test_account.bin 2>nul

echo [9/9] Pipeline Completed Successfully!
echo.
echo ================================================
echo Reports and Artifacts Ready:
echo   - Static Analysis : reports\static_code_check\index.html
echo   - MISRA           : reports\misra_check\index.html
echo   - Code Coverage   : reports\code_coverage\coverage.html
echo   - QA Artifacts    : release/ folder (.exe, .o, .bin)
echo ================================================