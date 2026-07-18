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


10-07-2026

**Here is the complete, professional, tutorial-style `README.md` for your project:**

```markdown
# Banking System CI/CD Pipeline

A complete demonstration of modern **CI/CD practices** for a C-based banking system.

This project is built as a **practice and portfolio project** to showcase industry-standard CI/CD practices for C applications.

## Project Overview

This repository demonstrates a full CI/CD pipeline for a simple banking system written in C. It includes quality checks, automated testing, code coverage, static analysis, and professional reporting.

### Key Features

- **Code Formatting** with `clang-format`
- **Static Code Analysis** using `cppcheck`
- **MISRA C Compliance** checking
- **Unit Testing** with **Unity Framework**
- **Code Coverage** using `gcov` + `gcovr`
- **Automated Build** with CMake + GCC
- **GitHub Actions** for continuous integration
- **Automated Releases** with artifacts (`.exe`, `.bin`, reports)
- **Conventional Commits** + Automated Changelog

## Why This Project?

- To learn and demonstrate modern CI/CD practices
- To show how to build a production-ready pipeline for C projects
- To serve as a reference for future embedded/C projects

## Project Structure

```
banking-system-ci-cd-pipeline/
├── src/                    # Main source code (account, main, etc.)
├── include/                # Header files
├── tests/                  # Unit tests
├── unity/                  # Unity test framework
├── static_code_analysis/   # Static analysis tools & MISRA config
├── scripts/                # Automation scripts (run_all.bat, etc.)
├── reports/                # Generated reports (gitignored)
├── release/                # QA executables + .bin/.o files (gitignored)
├── .github/workflows/      # GitHub Actions CI/CD
├── CMakeLists.txt          # Build configuration
├── README.md
├── .gitignore
├── cliff.toml              # Changelog configuration
└── .commitlintrc.json      # Commit linting
```

## How to Run Locally

### Prerequisites

- MSYS2 with MinGW64
- CMake
- Python 3
- LLVM (for clang-format)

### Run Full Pipeline

```powershell
# Full CI/CD Pipeline
scripts\run_all.bat
```

### Run Specific Parts

```powershell
# Static Analysis + MISRA only
scripts\static_code_analysis.bat
```

## CI/CD Flow

1. **Code Formatting** (clang-format)
2. **Static Analysis + MISRA** (cppcheck)
3. **Build** (CMake + GCC)
4. **Unit Testing** (Unity)
5. **Code Coverage** (gcovr)
6. **Artifact Generation** (reports + release folder)
7. **Automated Release** (on tag push)

## GitHub Actions

The pipeline runs automatically on every push and pull request.

**Artifacts** (reports + executables) are available for download after each run.

## Releases

Releases are automated when you push a tag:

```powershell
git tag v1.0.1
git push origin v1.0.1
```

## Conventional Commits

We use Conventional Commits for automatic changelog generation.

**Examples:**

- `feat: add new transaction module`
- `fix: bug in account balance calculation`
- `docs: update README`

## How to Contribute

1. Fork the repository
2. Create a feature branch
3. Follow Conventional Commits
4. Submit a Pull Request

---

**Replace your current `README.md`** with the above content.

This README is complete, tutorial-style, and professional — perfect for public GitHub profile and future reference.

Let me know if you want to add badges or anything else. 

Now your project is ready for showcasing. 

Would you like to start the STM32 project now?



///////////////////////////////////////////////////////////////////////////////////

16-07-2026



feature/* 
     ↓
  develop  ← Integration of new features
     ↓
   tests   ← Dedicated testing + QA
     ↓
  staging  ← Final pre-production validation
     ↓
   main    ← Production



*staging can be renamed to release/* (release/1.0 or release /2.0 etc)


---------------------------------------------------------------
Push to develop → Run unit + integration
Push to tests → Run full automation + HIL
Push to staging → Run smoke + performance

Keep Tests branch updated regularly from develop.
Merge Tests → Staging only after tests pass.
---------------------------------------------------------------

Test Type,Primary Branch,When / How It's Run,Environment,Who Runs It
Unit Tests,Develop (and feature branches),On every commit / PR,Local + CI Pipeline,Developers + CI
Integration Tests,Develop → Tests,"After merge to Develop, or on Tests branch",CI + Test Environment,Dev + QA
"Automation Tests (E2E, UI, API)",Tests (mainly),Continuously on Tests branch,Test / QA Environment,QA + Automation team
HIL Tests (Hardware-in-the-Loop),Tests or Staging,Before promoting to Staging,Specialized Test Bench,QA / Test Engineers
Regression / Smoke,Staging,Before release to Production,Staging Environment,QA
Final Acceptance,Staging,"UAT, Performance, Security",Staging (mirrors Prod),QA + Stakeholders


Detailed Breakdown by Branch
1. Feature Branches (temporary)

Unit tests run locally + in CI on every Pull Request.
Some teams also run basic integration or component tests here.
Goal: Catch issues early.

2. Develop Branch

Main home for Unit Tests and basic Integration Tests.
CI pipeline runs full unit + integration test suite on every push/merge.
Code here is "integration-ready" but not fully QA'd.

3. Tests Branch ← Best place for most testing

This is where heavy testing happens.
Automation Tests (E2E, Selenium, Cypress, Playwright, API automation, etc.).
HIL Tests (especially in embedded, automotive, IoT, or hardware-related projects).
Full regression suites.
Manual exploratory testing.
Why? It isolates testing from active development on develop.

4. Staging Branch

Final validation before production.
Smoke tests, final regression, performance, security scans, UAT (User Acceptance Testing).
Should be very stable.

5. Main Branch

Only the most critical smoke / sanity tests.
Production monitoring and post-deploy tests (not development tests).



---------------------------------------------------------------

Complete flow of version control, development, branching, QA releasing, testing, production release etc as below



Developers

↓

feature/login, feature/new_account, feature/withdraw  ---> after development push and create PR to develop branch

↓
develop

↓

Feature Freeze (once development is done and ready for QA)

↓

release/2.0 (create this branch from develop branch)

↓

Build #301 (add, commit, push)

↓

QA (click on the CI of this build no, go to artifacts, download the artifacts to test) no tags

↓

25 Bugs

↓

Fix

↓

Build #302 (add, commit, push)

↓

QA (click on the CI of this build no, go to artifacts, download the artifacts to test) no tags

↓

10 Bugs

↓

Fix

↓

Build #303 (add, commit, push)

↓

QA (click on the CI of this build no, go to artifacts, download the artifacts to test) no tags

↓

2 Bugs

↓

Fix

↓

Build #304 (add, commit, push)

↓

QA (click on the CI of this build no, go to artifacts, download the artifacts to test) no tags

↓

Looks Stable

↓

Build #305 (add, commit, push, create tag)

↓

RC1 (Tag v2.0.1-rc1) check releases

↓

QA

↓

1 Bug

↓

Fix

↓

Build #306

↓

RC2(Tag v2.0.1-rc2) check releases

↓

QA

↓

1 Bug

↓

Fix

↓

Build #307

↓

RC3(Tag v2.0.1-rc3) check releases

↓

QA Pass

↓

Merge to main

↓

Tag v2.0.1

↓

Release to Customer -- v2.0.1


---------------------------------------------------------------

Explanation:

Every time CI compiles the code, it creates a build.

Example:

Build #301
Build #302
Build #303


These are mostly internal. QA uses them.

The files might be:

banking_system.bin
banking_system.exe
test_account.bin
test_account.exe

These builds are often stored in a CI system (Jenkins, GitLab CI artifacts, GitHub Actions artifacts), not necessarily on GitHub Releases.



2. Release Candidate (RC)

When a build is considered stable enough, it is labeled:

v2.0.1-RC1

This is still usually the same release/2.0 branch, just a specific build.

create a Git tag:

v2.0.1-RC1

Some also create a GitHub Release called v2.0.0-RC1 and attach:

banking_system.bin
banking_system.exe
test_account.bin
test_account.exe

This allows QA to download exactly the build they should test.


Final Release

Suppose RC3 passes all testing.

Now:

release/2.0
        │
        ▼
Merge into main

Create a tag:

v2.0.1

Then create a GitHub Release:

banking_system.bin
banking_system.exe
test_account.bin
test_account.exe
release_notes.pdf
source_code(zip)

Release Name:
v2.0.1


Note:
Build = every compiled output from CI.
Tag = a permanent marker pointing to a specific commit (e.g., v2.0.1-RC2 or v2.0.1).
GitHub Release = a user-friendly page, usually associated with a tag, where you publish binaries, installers, firmware images, and release notes for QA or customers.

Development builds → GitHub Actions Artifacts (internal use).
RC builds → GitHub Releases (QA testing).
Final release → GitHub Releases (customers, manufacturing, or deployment).
---------------------------------------------------------------


Development workflow guide for dev team:



Customer Requirement
        │
        ▼
GitHub Issue
        │
        ▼
Sprint Planning
        │
        ▼
feature/<issue>-<name>
        │
        ▼
Code + Local Testing
        │
        ▼
Pull Request
        │
        ▼
Code Review
        │
        ▼
Merge → develop
        │
        ▼
CI Build + Tests
        │
        ▼
Artifacts (QA)
        │
        ▼
Feature Complete?
        │
        ├── No → Continue development
        │
        └── Yes
               │
               ▼
         release/2.0
               │
               ▼
        QA Testing
               │
        Bug Fixes on release/2.0
               │
               ▼
        Tag v2.0.0-RC1
               │
               ▼
     GitHub Release (RC1)
               │
               ▼
        QA Approval?
        │             │
       No            Yes
        │             │
        ▼             ▼
   RC2/RC3...   Merge → main
                      │
                      ▼
               Tag v2.0.0
                      │
                      ▼
        GitHub Release (Final)
                      │
                      ▼
     Customer / Manufacturing


---------------------------------------------------------------

Permissions

Not everyone should have the same access.

Typical roles:

Developer: Push feature branches, open PRs.
Reviewer/Tech Lead: Approve PRs.
Release Manager: Create release branches, tags, GitHub Releases.
Admin: Manage repository settings.

---------------------------------------------------------------


Artifact Naming

Be consistent.

Example:

Product_v2.0.0_RC1.bin
Product_v2.0.0_RC1.wic
Product_v2.0.0_RC1.zip

Instead of generic names like firmware.bin.


---------------------------------------------------------------

Code Review Checklist

Reviewers typically check:

Does it work?
Is the code readable?
Any memory leaks?
Thread safety?
Error handling?
Documentation updated?
Tests added?

---------------------------------------------------------------

QA Test Plan

QA should know:

What to test
Which board/version
Test cases
Expected results
Pass/Fail status

---------------------------------------------------------------

Versioning

Use Semantic Versioning:

v1.0.0
v1.0.1
v1.1.0
v2.0.0

Where:

Major = breaking changes
Minor = new features
Patch = bug fixes

---------------------------------------------------------------


Issue Tracking

Before creating a feature branch:

Issue #125
Add CG5317 Driver

Then create:

feature/125-cg5317-driver

When merging:

Closes #125

GitHub automatically closes the issue.


---------------------------------------------------------------

Branch Protection ✅ (Must Have)

Protect these branches:

main
develop
release/*

Rules:

No direct pushes
Pull Request required
At least 1–2 approvals
CI must pass
No force pushes
No deleting protected branches


---------------------------------------------------------------

Pull Request Template

Every PR should answer questions like:

## What changed?

## Why is this change needed?

## How was it tested?

## Related Issue

## Screenshots (if UI)

This helps reviewers understand the change quickly.


---------------------------------------------------------------


Changelog

Maintain a CHANGELOG.md.

Example:

v2.0.0

Added
- tla driver
- BQ25672 support

Fixed
- Ethernet timeout

Changed
- Yocto upgraded to Scarthgap

Customers and QA can quickly see what's new

---------------------------------------------------------------


CI Checks

Your CI shouldn't just build. It can also:

Compile the code
Run unit tests
Check code formatting
Run static analysis
Verify licenses
Generate documentation
Package release artifacts

A PR should only be merged if these checks pass.


---------------------------------------------------------------

Release Notes

Every RC and every final release should have release notes.

Include:

Version
New features
Fixed bugs
Known issues
Upgrade instructions
Supported hardware

---------------------------------------------------------------



GitHub Releases

Each release should contain:

Release Notes
Binary files (.bin, .exe, .wic, etc.)
Checksums (SHA256)
Source code (optional)
Documentation (optional)


---------------------------------------------------------------


Milestones (Optional but Useful)

Group issues into releases.

Example:

Milestone

v2.0.0

✓ Issue 12
✓ Issue 13
✓ Issue 15

When all issues in the milestone are closed, you're ready to create the release branch.


---------------------------------------------------------------


If you're using GitHub, I'd also enable:

Protected branches
Required status checks before merging
Required code review approvals
Automatic deletion of merged feature branches
Signed tags for official releases (optional but useful)
Dependabot/security scanning (if applicable)


---------------------------------------------------------------

Hotfix Flow

If production has a critical issue:


main
   │
   ▼
hotfix/2.0.1
   │
   ▼
QA
   │
   ▼
main
   │
   └──► develop


Always merge the hotfix back into both main and develop so the fix isn't lost.
---------------------------------------------------------------   



Notifications:


using GitHub, a clean workflow would be:

Build succeeds → GitHub Actions uploads artifacts.
GitHub Actions automatically sends a notification (email, Slack, or Teams) with the build number and artifact link.
Release Candidate created → GitHub Release is published and QA is notified.
QA completes testing → They update the ticket to "Passed" or "Failed" and attach the test report.
Final release published → Customers or internal stakeholders are notified according to your distribution process.

---------------------------------------------------------------


CI Sends Notifications (Most Common)

When CI finishes successfully, it sends notifications.

Examples:

📧 Email
Slack
Microsoft Teams
Google Chat
Discord (less common in enterprises)

Example email:

Subject: Build #302 Succeeded

Project: TBSI Firmware
Branch: release/2.0
Build: #302

Artifacts:
firmware.bin
Image.wic

Status: SUCCESS

If the build fails:

Subject: Build #302 Failed

Everyone knows immediately.


---------------------------------------------------------------


Example flow:

Developer
     │
     ▼
Merge → release/2.0
     │
     ▼
GitHub Actions
     │
     ▼
Build #302
     │
     ▼
Artifacts Uploaded
     │
     ▼
Email + Slack Notification
     │
     ▼
QA Downloads Artifact
     │
     ▼
Testing

---------------------------------------------------------------


For an RC

Developer
     │
     ▼
Create Tag v2.0.0-RC1
     │
     ▼
GitHub Release Created
     │
     ▼
Email + Slack + Jira Notification
     │
     ▼
QA Starts RC Testing


---------------------------------------------------------------


For the final release


Release Manager
     │
     ▼
Tag v2.0.0
     │
     ▼
GitHub Release Published
     │
     ▼
Customer Notification
     │
     ▼
Download Available


---------------------------------------------------------------

The workflow trigger in your current ci-cd.yml still only listens to branches defined in the ci-cd.yml file



Project files:

Step 1: 

Set up Conventional Commits + Semantic Versioning

commit style.
Using the below format for commits:

feat: add new feature
fix: bug fix
docs: update documentation
chore: maintenance

This enables automatic changelog and versioning.


Step 2: release.yml

Automated Release Setup
GitHub Actions workflow for automated releases.
Create a new file: .github\workflows\release.yml


git add .github\workflows\release.yml
git commit -m "chore: add automated release workflow"
git push

git tag v1.0.0
git push origin v1.0.0

This will automatically create a release with artifacts.



Step 3: commit-lint.yml

Conventional Commits Enforcement
Create .github/workflows/commit-lint.yml



Step 5: 

Create .commitlintrc.json (root folder)



Step 6: 

Automatic Changelog + Release Workflow
Update or create .github\workflows\release.yml


git add .github .commitlintrc.json
git commit -m "chore: add conventional commits and automated release with changelog"
git push



Step 7: cliff.toml

cliff.toml file for automated changelog generation


git add cliff.toml
git commit -m "chore: add automated changelog configuration"
git push



Project follows the below:

Conventional Commits - Added Conventional Commits enforcement
Automated Changelog - Added automated changelog (cliff.toml)
Automated Releases - Added automated release workflow
Semantic Versioning - Tagged and pushed v1.0.0


push commands:

git tag v1.0.0
git push origin v1.0.0

---------------------------

if any fix identified and then do the fix:


git add .github\workflows\release.yml
git commit -m "fix: simplify release workflow with proper permissions"
git push
git tag -f v1.0.0
git push origin v1.0.0 --force


---------------------------

if any feature identified to be implemented:

git add .github\workflows\release.yml
git commit -m "feat: include .bin files in release"
git push
git tag -f v1.0.0
git push origin v1.0.0 --force


---------------------------

Delete the existing tag (or use a new version like v1.0.1)

Run this locally:
PowerShellgit tag -d v1.0.0
git push origin --delete v1.0.0

then implement the feature:

git add .github\workflows\release.yml
git commit -m "feat: include .bin files in automated release"
git push
git tag v1.0.1
git push origin v1.0.1


---------------------------

when there is update in the README file

git add README.md
git commit -m "docs: improve README with full tutorial and documentation"
git push



When to use the tag commands?
Only when you want to trigger an automated release:
You can run them only when you want to create a new release.
git tag v1.0.1
git push origin v1.0.1


---------------------------------------------------------------


git cmds in this project so far:


# to stage all new, modified, and deleted files or changes
git add .

# creates main branch
git branch -M main



# exmaple commit msg
git commit -m "feat: include .bin files in automated release"

git status

# to see list of local branches
git branch 

# to see remote branches
git branch -a 

---------------------------

# Make sure you are on release branch
git checkout release/1.0
 
# Pull latest changes
git pull origin release/1.0
 
# Switch to main
git checkout main
 
# Merge release into main
git merge release/1.0 --no-ff -m "Release v1.0.2"
 
# Push main
git push origin main

# create final release tag
git tag v1.0.2
git push origin v1.0.2


---------------------------

# Switch to develop branch
git checkout develop
 
# Pull latest changes
git pull origin develop


---------------------------


# create and checkout to the branch
git checkout -b develop

# Link your local develop to GitHub's develop
git push -u origin develop

# Switch to develop branch
git checkout develop
# Get latest changes from GitHub          
git pull origin develop       

---------------------------

<<<<<<< HEAD
# cmds for normal release: 
=======
# For normal release: 
>>>>>>> release/3.0
git tag v1.0.0
git push origin v1.0.0

# cmds for Release Candidate:
git tag v1.0.0-rc1
git push origin v1.0.0-rc1

---------------------------


# made some changes in any branch by mistake, and relaised, not commited, and want to restore your working tree to the last commit and all uncommitted changes will be lost.

git restore .

# If you also created new untracked files:

git clean -fd


---------------------------

# Save uncommitted changes
git stash          



git stash

Temporarily saves your modified files and restores your working directory to the last committed state.

Before:

main
 ├── modified file1.c
 ├── modified file2.c

Run:

git stash

After:

Your modifications are saved in a stash.
Your working directory becomes clean.

# Move to develop branch
git switch develop 

# Apply the saved changes on develop
git stash pop      



git stash pop

Restores the most recently stashed changes onto your current branch and removes that stash from the stash list.

git stash pop

After this, your changes appear on develop.

---------------------------

# for merging, releasing
git switch release/3.0
git pull origin release/3.0
git merge develop
git push origin release/3.0
git tag v3.0.1-rc1
git push origin v3.0.1-rc1

---------------------------


git wrokflow I followed in this project

Develop features on feature/*
Merge features into develop
Create release/x.y
Generate RC1, RC2, etc. from release/x.y
Apply QA bug fixes only on release/x.y
Merge release/x.y → main (production)
Tag the production release (vX.Y.Z)
Merge release/x.y → develop
Delete release/x.y



As below:


Important distinction between **internal QA builds**, **Release Candidates (RCs)** and **Production Release**



---

# Phase 1: Development

```text
develop
   │
   ├── Feature A
   ├── Feature B
   ├── Feature C
```

When all planned features for 4.0.1 are complete:

```bash
git switch develop
git switch -c release/4.0
git push -u origin release/4.0
```

At this point, **feature development for 4.0.1 is frozen**. Only bug fixes go into `release/4.0`.

---

# Phase 2: QA Builds

Your CI can generate builds like:

```text
Build #101
Build #102
Build #103
Build #104
```

or

```text
QA-001
QA-002
QA-003
```

These are **internal builds** for testers.

Example:

```text
Build #101
↓
QA finds 20 bugs

Fix bugs

Build #102
↓
QA finds 10 bugs

Fix bugs

Build #103
↓
QA finds 3 bugs

Fix bugs

Build #104
↓
No major bugs
```

These builds usually **are not tagged** because they're just intermediate verification builds.

---

# Phase 3: Release Candidates

Once the team believes the software is stable enough to be a release candidate:


```bash


git add README.md 
git commit -m "fix: updated README.md file"
git push
git tag v4.0.1-rc1
git push origin v4.0.1-rc1
```

```text
v4.0.1-rc1
```

QA performs a much more thorough validation.

If bugs are found:

```text
Fix
↓

v4.0.1-rc2

Fix
↓

v4.0.1-rc3

Fix
↓

v4.0.1-rc4

Fix
↓

v4.0.1-rc5
```

If RC5 passes:

```text
git switch main
git merge release/4.0 --no-ff
git tag v4.0.1
git push origin main --tags/git push origin v4.0.1
```

Production release:

```text
v4.0.1
```

---

# Timeline

```text
develop
    │
    └──────────────┐
                   ▼
             release/4.0
                   │
                   │
            Build #101
                   │
            Build #102
                   │
            Build #103
                   │
            Build #104
                   │
                   ▼
               v4.0.1-rc1
                   │
               v4.0.1-rc2
                   │
               v4.0.1-rc3
                   │
               v4.0.1-rc4
                   │
               v4.0.1-rc5
                   │
                   ▼
                v4.0.1
                   │
             Merge to main
```

---

# Approach

Handling releases:

* One `release/4.0` branch.
* Multiple bug-fix commits on that branch.
* Multiple builds while QA tests.
* Multiple RCs if necessary.
* Final production tag after the last RC is approved.

The only adjustment I'd make is this:

* **QA builds** (Build #101, #102, etc.) are identified by the CI system's build number
* **RCs** (`v4.0.1-rc1`, `v4.0.1-rc2`, ...) are tagged, they represent candidate release snapshots.

---

## GitHub Actions CI/CD pipeline 

Based on the CI/CD pipeline building, a practical convention would be:

* Push to `release/4.0` → CI generates **Build #xxx** artifacts automatically for QA.
* When you decide a build is release-candidate quality, create a tag:

  * `v4.0.1-rc1`
  * `v4.0.1-rc2`
  * …
* When the final RC is approved:

  * Merge `release/4.0` → `main`
  * Create tag `v4.0.1`
  * CI generates the production artifact.


```bash


Create the release branch

git switch develop
git pull origin develop
git switch -c release/4.0
git push -u origin release/4.0

Pulling develop first ensures your release branch starts from the latest integrated code.

-
-
-
-

QA finds bugs

Fix them on release/4.0

git add .
git commit -m "fix: updated README.md"
git push

Your CI can automatically produce a QA build from this push.

-
-
-
-

Create RC1

Once you decide this specific commit is a Release Candidate:

git tag v4.0.1-rc1
git push origin v4.0.1-rc1

Your CI should be configured so that pushing an *-rc* tag generates the RC artifact.

-
-
-
-

QA tests RC1

If bugs are found:

git add .
git commit -m "fix: resolve login issue"
git push

CI generates another QA build.

When you're ready for another release candidate:

git tag v4.0.1-rc2
git push origin v4.0.1-rc2

Repeat as needed:

v4.0.1-rc1
v4.0.1-rc2
v4.0.1-rc3
v4.0.1-rc4
v4.0.1-rc5

```

Final Release

When the last RC is approved:

```bash
git switch main
git pull origin main
git merge release/4.0 --no-ff -m "Release v4.0.1"
git tag v4.0.1
git push origin main
git push origin v4.0.1

Then synchronize the branches:

git switch develop
git merge release/4.0
git push origin develop

Finally, if you no longer need it:

git branch -d release/4.0
git push origin --delete release/4.0


```

Clear separation between everyday QA builds, formally versioned release candidates and production releases

---------------------------

Deleting branches and tags:

# 1. Switch to develop
git checkout develop

# 2. Pull latest from remote develop (just in case)
git pull origin develop

# 3. Delete local release/5.0 branch
git branch -D release/5.0

# 4. Delete remote release/5.0 branch
git push origin --delete release/5.0

# 5. Delete the test tag (optional but clean)
git tag -d v5.0.0-rc1
git tag -d v5.0.0-rc2
git tag -d v5.0.0-rc3
git push origin --delete v5.0.0-rc1
git push origin --delete v5.0.0-rc2
git push origin --delete v5.0.0-rc3

---------------------------

Resetting develop branch to match main:(because develop was modified and pushed to repo, and then releaase/5.0 was created from develop locally and release/5.0 failed and deprecated because of enough bugs, so release/5.0 is deleted everywhere and now develop was synchronized to match main(v4.0.1 - which has a stable production code)
Reset develop branch to match main (since release/4.0 is already merged to main)

Make sure you are on develop
git checkout develop

# Reset develop to main (this will remove all unwanted changes from release/5.0 experiment)
git reset --hard origin/main

# Force push to update remote develop
git push origin develop --force

---------------------------

Alternative (Merge main into develop):

# You can do merge instead, but it's not as clean in this situation.

git checkout develop
git merge main --no-ff -m "Merge main into develop (cleanup)"
git push

# But this will create an extra merge commit and keep unnecessary history.

---------------------------