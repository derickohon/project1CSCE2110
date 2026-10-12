# Complexity Analysis

Time complexity of the main operations, as implemented.

- **n** = active reservations
- **w** = students on the waiting list
- **h** = cancellations in the history
- **R** = resources

## Data structures

| Structure | Type | Notes |
|-----------|------|-------|
| Active reservations | Linked list (`ReservationManager`) | Has `head` and `tail` pointers |
| Waiting list | Queue (`WaitingList`) | Has `front` and `rear` pointers |
| Cancellation history | Stack (`CancellationHistoryStack`) | Has only a `top` pointer |
| Resources | `vector<Resource>` | Searched linearly |

## Operations

| Operation | Time | Why |
|-----------|------|-----|
| Add reservation to the list | **O(1)** | Append at `tail`, no traversal |
| Create reservation (full action) | **O(R + n)** | Find the resource, then rewrite both data files |
| Find / remove a reservation | **O(n)** | Walk the list from `head` |
| Cancel reservation (full action) | **O(n + w + R)** | Walk the list, process the waiting queue, find the resource, rewrite files |
| Waiting list: add or remove | **O(1)** | Use the `rear` / `front` pointers |
| Waiting list: display | **O(w)** | Visit each student once |
| Stack: push / pop | **O(1)** | Only touch `top` |
| Undo cancellation (full action) | **O(R + n)** | Stack pop and list append are O(1); the rest is the resource lookup and file rewrite |
| Display cancellation history | **O(h)** | Walk the stack |
| Search reservations | **O(n)** | Linear search (list is unsorted, so no binary search) |
| Sort resources (Quick Sort) | **O(R log R)** average, **O(R²)** worst | Pivot is the last element, so already-sorted data is the worst case |
| Reports | **O(R + n log k + w)** | One pass over each collection (k = resources with reservations) |

## Notes

- **Double-booking** is prevented by checking the resource's status, not by scanning the reservation list.
- **Cancelling** scans the list twice (find, then remove), which is still O(n).
- **Promoting a waiting student** removes every student from the queue and puts back the ones that don't match, so it costs O(w).
- **Stack size** is O(h) because the stack keeps no counter and has to count nodes.

## Space

Each structure uses space proportional to its own size: **O(n)** reservations, **O(w)** waiting list, **O(h)** history, **O(R)** resources.

## Module ownership
 
| Files | Responsibility |
|-------|----------------|
| `ReservationManager.h/.cpp` | Owns the active-reservation linked list **and** the cancellation stack. `cancelReservation` removes from the list and pushes onto the stack; `undoLastCancellation` pops it back and re-adds it to the list. |
| `CancellationHistoryStack.h/.cpp` | The stack of cancelled reservations. |
| `WaitingList.h/.cpp` | The waiting-list queue (student, resource, requested date). |
| `main.cpp` | Menu, resource vector, sorting, searching, reports, and file loading/saving. Before an undo, it checks the resource is still Available and then sets it back to Reserved. |