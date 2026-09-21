# Campus Resource Reservation System

A menu-driven C++ console app for reserving campus resources (study rooms, laptops, calculators, lab equipment, tutoring) and managing reservations, waiting lists, cancellations, and reports.

## Features

- **View / Sort Resources** — displays all resources; sorting is done with a hand-implemented Quick Sort (by ID or Name).
- **Create / Cancel Reservation** — reserves an available resource for a student, or cancels an existing reservation by ID.
- **Waiting List** — if a resource is unavailable, students can join a FIFO queue; cancelling a reservation automatically pulls the next matching student off the waitlist.
- **Cancellation History / Undo** — every cancellation is pushed onto a stack, so the most recent one can be undone.
- **Search Reservations** — linear search by Reservation ID, Student ID, or Student Name.
- **Reports** — current availability, active reservation count, most frequently reserved resource, and waitlist summary.

## Data Structures

- `vector<Resource>` and `vector<Reservation>` — resource inventory and active reservations.
- `CancellationHistoryStack` — custom linked-list stack for cancellation/undo.
- `WaitingList` — custom linked-list queue for FIFO waiting requests.

## Data Files

Resources and reservations are stored as pipe-delimited text files (`data/resources.txt`, `data/reservations.txt`) and are rewritten automatically whenever a reservation is created or cancelled.

## Build & Run

From the `Project1/` directory (so relative paths to `data/` resolve correctly):

```bash
g++ -std=c++17 -o app src/*.cpp
./app
```

## Known Limitation

Most logic currently lives in `main.cpp` rather than being split into dedicated classes — a cleaner version would move this into the relevant class files.