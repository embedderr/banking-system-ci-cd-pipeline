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



08-07-2026 - updated README.md
# Banking System CI/CD Pipeline

A complete demonstration of modern **CI/CD practices** for a C-based banking system.

## Project Overview

This repository demonstrates a full CI/CD pipeline for a simple banking system written in C. It includes quality checks, automated testing, code coverage, and professional reporting.

### Key Features

- **Code Formatting** with `clang-format`
- **Static Code Analysis** using `cppcheck`
- **MISRA C Compliance** checking
- **Unit Testing** with **Unity Framework**
- **Code Coverage** using `gcov` + `gcovr`
- **Automated Build** with CMake + GCC
- **GitHub Actions** for continuous integration
- Release artifacts for QA/testing

## Why CI/CD?

- Ensures high code quality
- Catches bugs early
- Automates repetitive tasks
- Provides confidence before deployment
- Makes the project portable and maintainable

## Project Structure

banking-system-ci-cd-pipeline/
├── src/                    # Main source code
├── include/                # Header files
├── tests/                  # Unit tests
├── unity/                  # Unity test framework
├── static_code_analysis/   # Static analysis tools & config
├── scripts/                # Automation scripts
├── reports/                # Generated reports (gitignored)
├── release/                # QA executables (gitignored)
├── .github/workflows/      # GitHub Actions CI/CD
├── CMakeLists.txt
├── README.md
└── .gitignore






## How to Run Locally

```powershell
# Full Pipeline (recommended)
scripts\run_all.bat

# Static Analysis only
scripts\static_code_analysis.bat



CI/CD Flow

Code Formatting Check
Static Analysis + MISRA
Build
Unit Testing
Code Coverage
Artifact Generation (reports + executables)


//////////////////////////////////////////////////////////////////////////////////

