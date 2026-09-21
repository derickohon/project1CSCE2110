#include "../include/Reservation.h"
#include "../include/Resource.h"
#include "../include/ReservationManager.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
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

// ----------------------------------------------

static void viewResources() {
	cout << "=== View Resources ===\n";
	//open file and read resources from file
	ifstream inputFile("data/resources.txt");
	if (!inputFile.is_open()) {
		cout << "Error: could not open data/resources.txt\n";
		return;
	}

	// Initiate the vector to hold the resources and a string to hold each line read from the file
	vector<Resource> resources;
	string line;

	// loop through each line of the file and parse the resource data
	while (getline(inputFile, line)) {
		if (line.empty()) {
			continue;
		}

		stringstream ss(line);
		string id, name, type, status;

		getline(ss, id, '|');
		getline(ss, name, '|');
		getline(ss, type, '|');
		getline(ss, status, '|');

		// Skip malformed lines that are missing fields
		if (id.empty() || name.empty() || type.empty() || status.empty()) {
			continue;
		}

		Resource resource;
		resource.setResourceID(id);
		resource.setResourceName(name);
		resource.setResourceType(type);
		resource.setAvailabilityStatus(status);

		resources.push_back(resource);
	}
	inputFile.close();

	// Display the resources in a formatted table
	if (resources.empty()) {
		cout << "No resources found.\n";
		return;
	}

	cout << left
		<< setw(10) << "ID"
		<< setw(28) << "Name"
		<< setw(15) << "Type"
		<< "Status" << endl;
	cout << string(61, '-') << endl;

	for (Resource& r : resources) {
		cout << left
			<< setw(10) << r.getResourceID()
			<< setw(28) << r.getResourceName()
			<< setw(15) << r.getResourceType()
			<< r.getAvailabilityStatus() << endl;
	}

	cout << "Total resources: " << resources.size() << endl;
}

// ----------------------------------------------

int main() {
	cout << "===== Campus Resource Reservation System =====\n";
	cout << "1) Demo cancellation history / undo (stack)\n";
	cout << "2) View Resources\n";
	cout << "3) Create Reservation\n";
	cout << "4) Cancel Reservation\n";
	cout << "5) View Waitlist\n";
	cout << "6) Undo Cancellation\n";
	cout << "7) Search Reservations\n";
	cout << "8) Sort Resources\n";
	cout << "9) Generate Reports\n";
	cout << "10) Exit\n";
	cout << endl;
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
		viewResources();
		break;
	case 3:
		cout << "Create Reservation selected.\n";
		break;
	case 4:
		cout << "Cancel Reservation selected.\n";
		break;
	case 5:
		cout << "View Waitlist selected.\n";
		break;
	case 6:
		cout << "Undo Cancellation selected.\n";
		break;
	case 7:
		cout << "Search Reservations selected.\n";
		break;
	case 8:
		cout << "Sort Resources selected.\n";
		break;
	case 9:
		cout << "Generate Reports selected.\n";
		break;
	case 10:
		cout << "Goodbye.\n";
		break;
	default:
		cout << "Unknown option.\n";
		break;
	}

	return 0;
}
