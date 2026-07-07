# Banking System CI/CD Pipeline

A complete CI/CD demonstration for a C-based banking system with quality checks, code coverage, and automation.

## Features

- Code Formatting with `clang-format`
- Static Analysis (`cppcheck` + `clang-tidy`)
- MISRA C Compliance Checking
- Unit Testing(`Gtest`)
- Code Coverage (`gcov` + `gcovr`)
- Automated Build
- GitHub Actions Pipeline
- Professional HTML Reports

## Project Structure

- `src/` → Source code
- `include/` → Header files
- `tests/` → Unit tests
- `scripts/` → Automation scripts
- `reports/` → Generated reports

## How to Run Locally

```powershell
# Run full pipeline
scripts\run_all.bat



/////////////////////////////////////////////////////////////////////////////////////

Write-Host "=== System Environment Check ===" -ForegroundColor Green

Write-Host "`n1. GCC:" -ForegroundColor Cyan
gcc --version

Write-Host "`n2. G++:" -ForegroundColor Cyan
g++ --version

Write-Host "`n3. Gcovr:" -ForegroundColor Cyan
gcovr --version

Write-Host "`n4. Cppcheck:" -ForegroundColor Cyan
cppcheck --version

Write-Host "`n5. CMake:" -ForegroundColor Cyan
cmake --version

Write-Host "`n6. Python:" -ForegroundColor Cyan
python --version

Write-Host "`n7. Pip:" -ForegroundColor Cyan
pip --version

Write-Host "`n8. Clang-format:" -ForegroundColor Cyan
clang-format --version

Write-Host "`n9. Clang-tidy:" -ForegroundColor Cyan
clang-tidy --version

Write-Host "`n=== Check Completed ===" -ForegroundColor Green

////////////////////////////////////////////////////////////////////////////////////////




# Banking System CI/CD Pipeline

A complete demonstration of modern CI/CD practices for a C-based banking system.

## Features

- **Code Formatting** with clang-format
- **Static Analysis** using cppcheck
- **MISRA C Compliance** checking
- **Unit Testing** with Unity Framework
- **Code Coverage** using gcov + gcovr
- **Automated Build** with CMake + GCC
- **GitHub Actions** CI/CD workflow

## Project Structure

- `src/` - Source code
- `include/` - Header files
- `tests/` - Unit tests
- `unity/` - Unity test framework
- `static_code_analysis/` - Static analysis configuration
- `reports/` - Generated reports
- `scripts/` - Automation scripts

## How to Run Locally

```powershell
# Full pipeline
scripts\run_all.bat


////////////////////////////////////////////////////////////////////////////////////////