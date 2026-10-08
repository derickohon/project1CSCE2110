#pragma once

#include "CancellationHistoryStack.h"
#include "Reservation.h"

// Teammates call recordCancellation() when a reservation is cancelled;
// undoLastCancellation() restores the most recently cancelled reservation.
class ReservationManager {
private:
	CancellationHistoryStack cancellationHistory;
   struct ReservationNode {
        Reservation data;
        ReservationNode* next;

        ReservationNode(const Reservation& reservation, ReservationNode* nextNode = nullptr)
            : data(reservation), next(nextNode) {}
    };

    ReservationNode* head = nullptr;
    ReservationNode* tail = nullptr;
    int reservationTotal = 0;

public:
	ReservationManager() = default;

	void recordCancellation(const Reservation& reservation);
	bool undoLastCancellation(Reservation& restoredReservation);
	void displayCancellationHistory() const;
	bool hasCancellationHistory() const;
	int cancellationHistoryCount() const;

	void addReservation(const Reservation& reservation);
    bool cancelReservation(const string& reservationID);
    void displayReservations() const;
    int reservationCount() const;
    void clearReservations();
};

 
