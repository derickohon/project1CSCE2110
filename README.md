# Campus Resource Reservation System

A menu-driven C++ console app for reserving campus resources (study rooms, laptops, calculators, lab equipment, tutoring) and managing reservations, waiting lists, cancellations, and reports.

## Features

- **View / Sort Resources** — displays all resources; sorting is done with a hand-implemented Quick Sort (by ID or Name).
- **Create / Cancel Reservation** — reserves an available resource for a student, or cancels an existing reservation by ID. Cancelling frees the resource and automatically assigns it to the next student on the waiting list who requested it.
- **Waiting List** — if a resource is unavailable, a student can join a FIFO queue with their requested date. When the resource is freed, the first matching student is promoted to an active reservation using the date they requested.
- **Cancellation History / Undo** — every cancellation is pushed onto a stack. Undo restores the most recently cancelled reservation to the active list and marks its resource Reserved again. Undo is refused if the resource has since been taken (for example, by a waiting-list student).
- **Search Reservations** — linear search by Reservation ID, Student ID, or Student Name.
- **Reports** — current availability, active reservation count, most frequently reserved resources, and waiting-list summary.

## Data Structures

| Structure | Where | Used for |
|-----------|-------|----------|
| Singly linked list (head/tail) | `ReservationManager` | Active reservations |
| Linked-list stack | `CancellationHistoryStack` (owned by `ReservationManager`) | Cancellation history and undo |
| Linked-list queue (front/rear) | `WaitingList` | FIFO waiting requests (student, resource, requested date) |
| `vector<Resource>` | `main.cpp` | Resource inventory |

`ReservationManager` owns both the active-reservation list and the cancellation stack. Cancelling moves a reservation from the list onto the stack; undo moves it back.

## Data Files

Resources and reservations are stored as pipe-delimited text files (`data/resources.txt`, `data/reservations.txt`) and are rewritten automatically whenever a reservation is created, cancelled, or restored with undo. The waiting list and cancellation history are kept in memory only and are not saved between runs.

## Build & Run

From the project directory (so relative paths to `data/` resolve correctly):

**Windows**

```bat
compile.bat
campus_reservation.exe
```

**Linux / macOS**

```bash
g++ -std=c++17 -I include -o app src/*.cpp
./app
```

## Complexity

See [ComplexityAnalysis.md](ComplexityAnalysis.md) for the Big-O analysis of the operations as implemented.

## Known Limitations

- Most menu logic still lives in `main.cpp` rather than dedicated classes.
- Waiting-list entries and cancellation history are not persisted to disk.
- Dates are stored as plain text and are not validated.