@echo off
rem Build from the project folder so relative paths (include\, src\, data\) resolve.
cd /d "%~dp0"

g++ -std=c++17 -Wall -Wextra -I include ^
  src\main.cpp ^
  src\Reservation.cpp ^
  src\Resource.cpp ^
  src\CancellationHistoryStack.cpp ^
  src\ReservationManager.cpp ^
  src\WaitingList.cpp ^
  -o campus_reservation.exe

if %ERRORLEVEL% EQU 0 (
  echo Build OK: campus_reservation.exe
  echo Run it from this folder so data\resources.txt and data\reservations.txt are found.
) else (
  echo Build failed.
  exit /b 1
)
