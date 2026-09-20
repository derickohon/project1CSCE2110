#include "ReservationManager.h"

#include <iostream>

// Records a cancelled reservation by pushing it onto
// the cancellation history stack.
void ReservationManager::recordCancellation(const Reservation& reservation) {
	cancellationHistory.push(reservation);
}

// Restores the most recently cancelled reservation.
// The restored reservation is returned through the reference parameter.
bool ReservationManager::undoLastCancellation(Reservation& restoredReservation) {
	if (cancellationHistory.isEmpty()) {
		return false;
	}

	return cancellationHistory.pop(restoredReservation);
}

// Displays all cancelled reservations, with the
// most recently cancelled reservation shown first.
void ReservationManager::displayCancellationHistory() const {
	cancellationHistory.displayHistory();
}

// Returns true if there is at least one cancelled
// reservation in the history.
bool ReservationManager::hasCancellationHistory() const {
	return !cancellationHistory.isEmpty();
}

// Returns the number of reservations currently
// stored in the cancellation history.
int ReservationManager::cancellationHistoryCount() const {
	return cancellationHistory.size();
}
