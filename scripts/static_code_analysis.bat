@echo on
echo ================================================
echo     Static Code Analysis + MISRA - Banking System
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

:: Clean old reports
rd /s /q reports\static_code_check reports\misra_check 2>nul
mkdir reports\static_code_check reports\misra_check

set files=./src
set includes=-I./include
set platfrm=win64

echo [1/3] Running Static Analysis...
cppcheck --xml --enable=all --clang --std=c99 %includes% --platform=%platfrm% %files% --output-file=reports\static_code_check\static_file.xml

echo [2/3] Running MISRA Check...
cppcheck --xml --enable=all --std=c99 %includes% --platform=%platfrm% --addon=./static_code_analysis/misra-config.json %files% --output-file=reports\misra_check\MISRA_file.xml

echo [3/3] Generating HTML Reports...
%PYCMD% ./static_code_analysis/htmlreport/cppcheck-htmlreport ^
  --file=reports\misra_check\MISRA_file.xml ^
  --title=Banking_System ^
  --report-dir=./reports/misra_check ^
  --source-dir=.

%PYCMD% ./static_code_analysis/htmlreport/cppcheck-htmlreport ^
  --file=reports\static_code_check\static_file.xml ^
  --title=Banking_System ^
  --report-dir=./reports/static_code_check ^
  --source-dir=.

echo.
echo Static Analysis + MISRA for Banking System Completed!
echo ================================================
echo Reports available at:
echo   - reports\static_code_check\index.html
echo   - reports\misra_check\index.html