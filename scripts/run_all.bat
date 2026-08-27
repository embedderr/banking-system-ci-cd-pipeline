#!/usr/bin/env bash
#
# Linux/Codespaces equivalent of scripts/run_all.bat.
#
# Note on scope: this script intentionally does NOT run the MISRA check.
# The apt-installed cppcheck package on Linux is not guaranteed to ship
# with the misra.py addon the way the official Windows MSI release does
# (see ci-cd.yml's static_analysis job, which explicitly re-syncs the
# addons folder from the matching source release for exactly this
# reason). Plain static analysis (no MISRA) is still run here as a
# best-effort local check; the authoritative static analysis + MISRA
# result for this project remains the Windows CI job.

set -e

cd "$(dirname "$0")/.."

echo "================================================"
echo "    Banking System - Full Pipeline (Linux/Codespaces)"
echo "================================================"

echo "[1/7] Cleaning previous outputs..."
rm -rf build reports release
mkdir -p reports/code_coverage reports/static_code_check release

echo "[2/7] Code Formatting Check..."
clang-format --dry-run --Werror src/*.c include/*.h || echo "[WARNING] Formatting issues found (see above)"

echo "[3/7] Static Analysis (best-effort, no MISRA -- see script header)..."
cppcheck --xml --enable=all --std=c99 -I./include --platform=unix64 ./src \
  --output-file=reports/static_code_check/static_file.xml || echo "[WARNING] cppcheck reported issues"

echo "[4/7] Building Project (Debug)..."
cmake -B build -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Debug
cmake --build build

echo "[5/7] Running Unit Tests..."
(cd build && ctest --output-on-failure)

echo "[6/7] Running Main Program (smoke test, piped empty input)..."
echo | timeout 5 ./build/banking_system || echo "Program exited (expected on closed stdin)"

echo "[7/7] Generating Code Coverage Report..."
gcovr -r . --html-nested reports/code_coverage/coverage.html --html-title="Banking System Coverage" || echo "[WARNING] gcovr failed"

echo "Creating Release Artifacts..."
cp build/banking_system release/banking_system
for f in build/test_*; do
  [ -f "$f" ] && [ -x "$f" ] && cp "$f" release/
done

echo ""
echo "================================================"
echo "Pipeline complete."
echo "  - Static analysis : reports/static_code_check/static_file.xml"
echo "  - Coverage         : reports/code_coverage/coverage.html"
echo "  - Artifacts        : release/"
echo "================================================"
