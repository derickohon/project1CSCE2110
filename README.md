# project1CSCE2110
Campus Resource Reservation System
Cancellation history & Complexity analysis

### Code (stack / reservation history)

| File | Purpose |
|------|---------|
| `include/CancellationHistoryStack.h` | Stack ADT for cancelled reservations |
| `src/CancellationHistoryStack.cpp` | `push`, `pop` (undo), `displayHistory`, `size` |
| `include/ReservationManager.h` | Team integration API |
| `src/ReservationManager.cpp` | `recordCancellation`, `undoLastCancellation`, `displayCancellationHistory` |

**When a teammate cancels a reservation:** remove it from the active linked list, then call:

```cpp
manager.recordCancellation(cancelledReservation);
```

**Undo:** pop the latest cancellation and re-insert into the active list:

```cpp
Reservation restored;
if (manager.undoLastCancellation(restored)) {
    // activeList.insert(restored);  // teammate's linked-list module
}
```

**Show history:** `manager.displayCancellationHistory();`

Run the module demo from `main.cpp` (menu option 1).

### Complexity analysis

See `docs/ComplexityAnalysis.md` (Big-O for insert/remove, queue, undo, display).

### Build (MinGW / g++)

```bat
compile.bat
campus_reservation.exe
```

Or:

```bat
g++ -std=c++17 -I include src\main.cpp src\Reservation.cpp src\Resource.cpp src\CancellationHistoryStack.cpp src\ReservationManager.cpp -o campus_reservation.exe
```
