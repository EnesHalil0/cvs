@echo off

set "PATH=%PATH%;C:\msys64\usr\bin"
set "CFLAGS=-Wall -Wextra -Werror -Wdouble-promotion"
set "OUT=cvs_model.exe"

echo Building...

gcc %CFLAGS% -o %OUT% cvs_model.c

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build finished successfully.

echo Running...

%OUT%

echo Opening MATLAB plot script...
matlab -r "plot_output"
