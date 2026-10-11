#pragma once

#include "CancellationHistoryStack.h"
#include "Reservation.h"

#include <functional>
#include <string>

// Owns the ACTIVE reservations (singly linked list) and the
// cancellation history (stack). Cancelling moves a reservation from the
// list onto the stack; undo moves the most recent one back to the list.
class ReservationManager {
private:
	CancellationHistoryStack cancellationHistory;

	struct ReservationNode {
		Reservation data;
		ReservationNode* next;

		ReservationNode(const Reservation& reservation, ReservationNode* nextNode = nullptr)
			: data(reservation), next(nextNode) {
		}
	};

	ReservationNode* head = nullptr;
	ReservationNode* tail = nullptr;
	int reservationTotal = 0;

public:
	ReservationManager() = default;
	~ReservationManager();

	ReservationManager(const ReservationManager&) = delete;
	ReservationManager& operator=(const ReservationManager&) = delete;

	// --- Cancellation history (stack) ---
	void recordCancellation(const Reservation& reservation);
	bool lastCancellation(Reservation& out);
	bool undoLastCancellation(Reservation& restoredReservation);
	void displayCancellationHistory() const;
	bool hasCancellationHistory() const;
	int cancellationHistoryCount() const;

	// --- Active reservations (linked list) ---
	void addReservation(const Reservation& reservation);
	Reservation* findReservation(const string& reservationID);
	bool cancelReservation(const string& reservationID);
	void forEachReservation(const std::function<void(Reservation&)>& action);
	void displayReservations() const;
	int reservationCount() const;
	void clearReservations();
};