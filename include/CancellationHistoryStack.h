#ifndef CANCELLATION_HISTORY_STACK_H
#define CANCELLATION_HISTORY_STACK_H

#include "Reservation.h"

// Stack ADT: stores cancelled reservations (most recent at top).
// Supports undo by popping the latest cancellation.
class CancellationHistoryStack {
private:
	struct StackNode {
		Reservation data;
		StackNode* next;
		StackNode(const Reservation& reservation, StackNode* link)
			: data(reservation), next(link) {}
	};

	StackNode* top;

public:
	CancellationHistoryStack();
	~CancellationHistoryStack();

	CancellationHistoryStack(const CancellationHistoryStack&) = delete;
	CancellationHistoryStack& operator=(const CancellationHistoryStack&) = delete;

	void push(const Reservation& reservation);
	bool pop(Reservation& outReservation);
	bool isEmpty() const;
	int size() const;
	void displayHistory() const;
	void clear();
};

#endif // CANCELLATION_HISTORY_STACK_H
