@echo off
set file=version.h
set major=0
set minor=0

::: Begin set date
echo Build Date: %date%
for /f "tokens=1-4 delims=/-. " %%i in ('date /t') do (call :set_date %%i %%j %%k %%l)
goto :end_set_date

:set_date
if "%1:~0,1%" gtr "9" shift
for /f "skip=1 tokens=2-4 delims=(-)" %%m in ('echo,^|date') do (set %%m=%1&set %%n=%2&set %%o=%3)
goto :eof

:end_set_date
::: End set date

::: Begin set time
echo Build Time: %time%
set hour=%time:~0,2%
set /a hour=%hour%+100
set hour=%hour:~1%
set minute=%time:~3,2%
::: End set time

::: Write the file
echo ///>%file%
echo /// @file>>%file%
echo ///>>%file%
echo /// @details>>%file%
echo /// This file contains the build information for the project.>>%file%
echo ///>>%file%
echo /// @copyright>>%file%
echo /// Copyright 2021 Syndaver Inc. as unpublished work. All rights reserved.>>%file%
echo ///>>%file%
echo /// The information contained herein is confidential property of Syndaver Inc. The>>%file%
echo /// use, copying, transfer or disclosure of such information is prohibited>>%file%
echo /// except by express written agreement with Syndaver Inc.>>%file%
echo.>>%file%
echo #if !defined(VERSION_H)>>%file%
echo #define VERSION_H>>%file%
echo.>>%file%
echo //>>%file%
echo // Shared Includes>>%file%
echo //>>%file%
echo.>>%file%
echo //>>%file%
echo // Shared Definitions>>%file%
echo //>>%file%
echo.>>%file%
echo /// @brief>>%file%
echo /// Major version number>>%file%
echo ///>>%file%
echo /// @details>>%file%
echo /// This defines the major version number of the software.>>%file%
echo #define VERSION_NUMBER_MAJOR                0x%major%>>%file%
echo.>>%file%
echo /// @brief>>%file%
echo /// Minor version number>>%file%
echo ///>>%file%
echo /// @details>>%file%
echo /// This defines the minor version number of the software.>>%file%
echo #define VERSION_NUMBER_MINOR                0x%minor%>>%file%
echo.>>%file%
echo /// @brief>>%file%
echo /// Build date>>%file%
echo ///>>%file%
echo /// @details>>%file%
echo /// This defines the build date of the software (year, month, day, with no spaces)>>%file%
echo #define VERSION_NUMBER_BUILD_DATE           0x%yy%%mm%%dd%>>%file%
echo.>>%file%
echo /// @brief>>%file%
echo /// Build time>>%file%
echo ///>>%file%
echo /// @details>>%file%
echo /// This defines the build time of the software (hour, minutes, with no spaces)>>%file%
echo #define VERSION_NUMBER_BUILD_TIME           0x%hour%%minute%>>%file%
echo.>>%file%
echo //>>%file%
echo // Shared Structures / Enumerations>>%file%
echo //>>%file%
echo.>>%file%
echo //>>%file%
echo // Shared Typedefs>>%file%
echo //>>%file%
echo.>>%file%
echo //>>%file%
echo // Shared Method Definitions>>%file%
echo //>>%file%
echo.>>%file%
echo //>>%file%
echo // Shared Global Variables>>%file%
echo //>>%file%
echo.>>%file%
echo #endif // VERSION_H>>%file%