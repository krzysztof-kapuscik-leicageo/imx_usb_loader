set EXENAME=%1
set TESTNAME=%2
set CONFIGDIR=%3

del /Q %TESTNAME%.result
del /Q %TESTNAME%.result.filtered
del /Q %TESTNAME%.output.filtered

%EXENAME% -c %CONFIGDIR% -v -S 0x15a2:0x0061 %TESTNAME% > %TESTNAME%.result
echo EXERESULT=%ERRORLEVEL%

findstr /v /c:"config file " /c:"parse " /c:"global configuration directory:" ^
    %TESTNAME%.result > %TESTNAME%.result.filtered
findstr /v /c:"config file " /c:"parse " /c:"global configuration directory:" ^
    %TESTNAME%.output > %TESTNAME%.output.filtered

FC /L /N /T %TESTNAME%.result.filtered %TESTNAME%.output.filtered
set RESULT=%ERRORLEVEL%
echo DIFFRESULT=%RESULT%

exit /B %RESULT%
