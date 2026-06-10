@echo off
SETLOCAL

SET PROJECT_DIR=%~dp0
SET VCPKG_DIR=%PROJECT_DIR%vcpkg

IF NOT EXIST "%VCPKG_DIR%" (
    echo Cloning vcpkg into %VCPKG_DIR%
    git clone https://github.com/Microsoft/vcpkg.git "%VCPKG_DIR%"
    IF %ERRORLEVEL% NEQ 0 (
        echo Failed to clone vcpkg
        exit /b %ERRORLEVEL%
    )

    echo Bootstrapping vcpkg
    CALL "%VCPKG_DIR%\bootstrap-vcpkg.bat"
    IF %ERRORLEVEL% NEQ 0 (
        echo Failed to bootstrap vcpkg
        exit /b %ERRORLEVEL%
    )
) ELSE (
    echo vcpkg already installed
)

REM Dependencies are declared in vcpkg.json (manifest mode) and are installed
REM automatically the first time you configure with CMake:
REM
REM   cmake --preset x64-debug
REM   cmake --build --preset x64-debug

echo Setup completed. Configure with: cmake --preset x64-debug
ENDLOCAL
