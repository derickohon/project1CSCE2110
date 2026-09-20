@echo off
g++ -std=c++17 -I include src\main.cpp src\Reservation.cpp src\Resource.cpp src\CancellationHistoryStack.cpp src\ReservationManager.cpp -o campus_reservation.exe
if %ERRORLEVEL% EQU 0 (
  echo Build OK: campus_reservation.exe
) else (
  echo Build failed.
  exit /b 1
)
