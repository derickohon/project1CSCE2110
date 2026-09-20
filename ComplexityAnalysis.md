# Campus Resource Reservation System — Complexity Analysis

**Scope:** Milestone 1 core operations (reservations, waiting list, cancellation history)

This document analyzes expected time complexity using **Big-O notation** for the primary data-structure operations in the system. Let **n** = number of active reservations, **w** = waiting-list length, and **h** = cancellation-history size.

Assumptions aligned with the project specification:

- Active reservations are stored in a **singly linked list** (search from head).
- The waiting list is a **queue** (FIFO).
- Cancelled reservations are stored on a **stack** (LIFO) for undo.

---

## 1. Reservation insertion (active linked list)

**Operation:** Add a new reservation after validation (resource exists, not double-booked, etc.).

| Implementation choice | Time complexity | Explanation |
|----------------------|-----------------|-------------|
| Insert at **head** of list | **O(1)** | Allocate one node and link it; no traversal. |
| Insert at **tail** (unsorted list) | **O(n)** | Must traverse to the last node to append. |
| Insert keeping sorted order (e.g., by date or ID) | **O(n)** | Traverse until the correct position is found. |

**Validation overhead (typical):**

- Check duplicate booking for same resource: **O(n)** linear scan of active reservations.
- Lookup resource by ID in a loaded array/vector of **R** resources: **O(R)** or **O(1)** with a hash map (not required for milestone).

**Typical milestone total:** **O(n)** when validation scans the list; **O(1)** extra for stack/queue if only the list is updated on insert.

---

## 2. Reservation removal (cancel active reservation)

**Operation:** Find a reservation in the active list, remove it, optionally push copy onto cancellation stack.

| Step | Complexity | Explanation |
|------|------------|-------------|
| Find reservation by ID (unsorted linked list) | **O(n)** | Worst case examine every node. |
| Remove node (after predecessor found) | **O(1)** | Pointer rewiring once located. |
| Push cancelled record onto stack | **O(1)** | Push one node onto stack top. |

**Typical milestone total:** **O(n)** dominated by search; removal and history push are constant time.

---

## 3. Waiting-list processing (queue)

**Operation:** Enqueue when resource unavailable; dequeue when resource frees.

| Operation | Complexity | Explanation |
|-----------|------------|-------------|
| Enqueue (add student) | **O(1)** | Tail pointer + new node, or circular buffer index update. |
| Dequeue (serve next student) | **O(1)** | Remove from head / advance front index. |
| Display entire queue | **O(w)** | Visit each waiting entry once. |
| Check if student already in queue | **O(w)** | Linear scan if no auxiliary index. |

**Typical milestone total:** core enqueue/dequeue are **O(1)**; full display is **O(w)**.

---

## 4. Undo cancellation (stack)

**Operation:** Pop most recently cancelled reservation from history and restore it to the active list.

| Step | Complexity | Explanation |
|------|------------|-------------|
| `pop` from cancellation stack | **O(1)** | Read top, update top pointer, delete one node. |
| Re-insert into active linked list | **O(1)** or **O(n)** | **O(1)** if insert-at-head; **O(n)** if tail insert or validation scan before restore. |
| Display cancellation history | **O(h)** | Traverse stack from top to bottom. |

**Typical milestone total:** undo pop is **O(1)**; full restore is **O(n)** if validation requires scanning active reservations.

---

## 5. Summary table

| Operation | Big-O | Dominant factor |
|-----------|-------|-----------------|
| Insert reservation | **O(n)** | Validation / tail insert |
| Remove reservation | **O(n)** | Find node in linked list |
| Waiting list enqueue/dequeue | **O(1)** | Queue with head/tail |
| Undo last cancellation (stack pop) | **O(1)** | Stack top access |
| Display cancellation history | **O(h)** | Stack traversal |

---

## 6. Space complexity (brief)

- Active list: **O(n)** nodes.
- Waiting queue: **O(w)** entries.
- Cancellation stack: **O(h)** stored cancellations (bounded by number of cancellations performed).
- Loaded resources file: **O(R)** for **R** resources.

Overall auxiliary structure storage is linear in the size of each collection.

---

## 7. Module ownership note

**Cancellation history (stack)** is implemented in:

- `include/CancellationHistoryStack.h`
- `src/CancellationHistoryStack.cpp`
- Integration API on `ReservationManager` (`recordCancellation`, `undoLastCancellation`, `displayCancellationHistory`).

Teammates should call `recordCancellation()` from their cancel-reservation flow and re-link the returned reservation from `undoLastCancellation()` into the active reservation list.
