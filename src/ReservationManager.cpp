#include "../include/ReservationManager.h"

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

ReservationManager::~ReservationManager() {
    clearReservations();
}

// Adds a reservation to the end of the list.
void ReservationManager::addReservation(const Reservation& reservation) {
    ReservationNode* newNode = new ReservationNode(reservation);

    if (head == nullptr) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }
    ++reservationTotal;
}

// Removes a reservation from the list by ID and records it in the cancellation history.
bool ReservationManager::cancelReservation(const string& reservationID) {
    ReservationNode* previous = nullptr;
    ReservationNode* current = head;

    while (current != nullptr && current->data.getReservationID() != reservationID) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        return false;   // not found
    }

    recordCancellation(current->data);

    if (previous == nullptr) {
        head = current->next;
    }
    else {
        previous->next = current->next;
    }

    if (current == tail) {
        tail = previous;
    }

    delete current;
    --reservationTotal;
    return true;
}

// Displays all active reservations in the order they were added.
void ReservationManager::displayReservations() const {
    if (head == nullptr) {
        std::cout << "  (no active reservations)" << std::endl;
        return;
    }

    std::cout << "  --- Active reservations ---" << std::endl;
    int index = 1;
    for (ReservationNode* current = head; current != nullptr; current = current->next) {
        std::cout << "  " << index << ". ";
        current->data.display();
        ++index;
        std::cout << "-----------------------------------" << std::endl;
    }
}

// Returns the number of active reservations.
int ReservationManager::reservationCount() const {
    return reservationTotal;
}

// Deletes every node in the reservation list.
void ReservationManager::clearReservations() {
    while (head != nullptr) {
        ReservationNode* nodeToRemove = head;
        head = head->next;
        delete nodeToRemove;
    }
    tail = nullptr;
    reservationTotal = 0;
}
