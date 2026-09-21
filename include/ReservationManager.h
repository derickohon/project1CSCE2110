#pragma once

#include "CancellationHistoryStack.h"
#include "Reservation.h"

// Teammates call recordCancellation() when a reservation is cancelled;
// undoLastCancellation() restores the most recently cancelled reservation.
class ReservationManager {
private:
	CancellationHistoryStack cancellationHistory;

public:
	ReservationManager() = default;

	void recordCancellation(const Reservation& reservation);
	bool undoLastCancellation(Reservation& restoredReservation);
	void displayCancellationHistory() const;
	bool hasCancellationHistory() const;
	int cancellationHistoryCount() const;
};
