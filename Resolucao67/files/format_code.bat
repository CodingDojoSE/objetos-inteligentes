@echo off


SET ERROR_LOG_FILE=uncrustify_log.txt
SET CONFIG_FILE=D:\User\Dropbox\Applications\SoftwareVersioning\MyUncrustifyConfigs\cpp.cfg
SET FILES_TYPES=*.cc




for /f %%i in ('date /T') do set CURRENT_DATE=%%i

echo.
echo Parsing file... Current time is: %time% - %CURRENT_DATE%

>%ERROR_LOG_FILE% 2>&1 (
for %%i in (%FILES_TYPES%) do (
    echo.
    echo uncrustify.exe -c %CONFIG_FILE% --no-backup %%i... Current time is: %time% - %CURRENT_DATE%
    uncrustify.exe -c %CONFIG_FILE% --no-backup %%i || goto error
    
    echo intend_empty_lines.lua %%i > temp_file.txt
    intend_empty_lines.lua %%i > temp_file.txt
    del %%i
    
    :: convert the EOF from CRLF to LF due the loss by the " > " above. 
    echo tr -d '\r' < temp_file.txt > %%i
    tr -d '\r' < temp_file.txt > %%i
    del temp_file.txt
)
echo.
)
goto successfully

:error
echo.
echo There is an ERROR! See the error log file on:
echo %ERROR_LOG_FILE%
echo.
pause
start %ERROR_LOG_FILE%
goto exit

:successfully
echo.
echo Successfully parsed the file! See the file logs on:
echo %ERROR_LOG_FILE%
if "%1"=="" goto exit
pause

:exit
