#include "Reservation.h"
#include "Resource.h"

#include <iostream>
#include <limits>
#include <string>

using namespace std;

static Reservation makeSampleReservation(
	const string& reservationId,
	const string& resourceId,
	int studentId,
	const string& studentName,
	const string& date) {
	Reservation reservation;
	reservation.setReservationID(reservationId);
	reservation.setResourceID(resourceId);
	reservation.setStudentID(studentId);
	reservation.setStudentName(studentName);
	reservation.setReservationDate(date);
	return reservation;
}

static void clearInputLine() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static void demoCancellationHistory() {
	ReservationManager manager;

	cout << "\n=== Cancellation history stack ===\n";

	Reservation first = makeSampleReservation("RES001", "R114", 1001, "Alex Kim", "2026-03-10");
	Reservation second = makeSampleReservation("RES002", "L001", 1002, "Jordan Lee", "2026-03-11");
	Reservation third = makeSampleReservation("RES003", "C001", 1003, "Sam Patel", "2026-03-12");

	cout << "Recording cancellations...\n";
	manager.recordCancellation(first);
	manager.recordCancellation(second);
	manager.recordCancellation(third);

	cout << "\nHistory count: " << manager.cancellationHistoryCount() << endl;
	manager.displayCancellationHistory();

	cout << "\nUndo (restore most recent cancellation):\n";
	Reservation restored;
	if (manager.undoLastCancellation(restored)) {
		cout << "Restored: ";
		restored.display();
	} else {
		cout << "Nothing to undo.\n";
	}

	cout << "\nRemaining history:\n";
	manager.displayCancellationHistory();
}

int main() {
	cout << "Campus Resource Reservation System — Milestone 1\n";
	cout << "1) Demo cancellation history / undo (stack)\n";
	cout << "2) Exit\n";
	cout << "Choice: ";

	int choice = 0;
	if (!(cin >> choice)) {
		clearInputLine();
		cout << "Invalid input.\n";
		return 1;
	}
	clearInputLine();

	switch (choice) {
	case 1:
		demoCancellationHistory();
		break;
	case 2:
		cout << "Goodbye.\n";
		break;
	default:
		cout << "Unknown option.\n";
		break;
	}

	return 0;
}
