#include "../include/Reservation.h"
#include "../include/Resource.h"
#include "../include/ReservationManager.h"
#include "../include/WaitingList.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

static const string RESOURCES_FILE = "data/resources.txt";
static const string RESERVATIONS_FILE = "data/reservations.txt";

static void clearInputLine() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Strips a trailing carriage return (from files saved with Windows-style
// CRLF line endings) so field comparisons like == "Available" work
// correctly regardless of which platform the data file was saved on.
static void stripTrailingCR(string& text) {
	if (!text.empty() && text.back() == '\r') {
		text.pop_back();
	}
}

// =====================================================
// File I/O helpers (Resources)
// =====================================================

// Loads resources from a pipe-delimited file into a vector.
static vector<Resource> loadResources(const string& filepath) {
	vector<Resource> resources;

	ifstream inputFile(filepath);
	if (!inputFile.is_open()) {
		cout << "Error: could not open " << filepath << "\n";
		return resources;
	}

	string line;
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
		stripTrailingCR(status);

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
	return resources;
}

// Writes the current resource list back out to disk, preserving
// any availability changes made during this session.
static bool saveResources(const string& filepath, vector<Resource>& resources) {
	ofstream outputFile(filepath);
	if (!outputFile.is_open()) {
		cout << "Error: could not save to " << filepath << "\n";
		return false;
	}

	for (Resource& r : resources) {
		outputFile << r.getResourceID() << "|"
			<< r.getResourceName() << "|"
			<< r.getResourceType() << "|"
			<< r.getAvailabilityStatus() << "\n";
	}

	outputFile.close();
	return true;
}

// =====================================================
// File I/O helpers (Reservations)
// =====================================================

// Loads active reservations from a pipe-delimited file into the
// manager's reservation linked list.
static void loadReservations(const string& filepath, ReservationManager& manager) {

	ifstream inputFile(filepath);
	if (!inputFile.is_open()) {
		// Not fatal - the file may not exist yet on a fresh checkout.
		return;
	}

	string line;
	while (getline(inputFile, line)) {
		if (line.empty()) {
			continue;
		}

		stringstream ss(line);
		string resID, resourceID, studentIDStr, studentName, date;

		getline(ss, resID, '|');
		getline(ss, resourceID, '|');
		getline(ss, studentIDStr, '|');
		getline(ss, studentName, '|');
		getline(ss, date, '|');
		stripTrailingCR(date);

		if (resID.empty() || resourceID.empty() || studentIDStr.empty()) {
			continue;
		}

		Reservation reservation;
		reservation.setReservationID(resID);
		reservation.setResourceID(resourceID);
		reservation.setStudentID(atoi(studentIDStr.c_str()));
		reservation.setStudentName(studentName);
		reservation.setReservationDate(date);

		manager.addReservation(reservation);
	}

	inputFile.close();
}

// Writes the current active reservation list back out to disk.
static bool saveReservations(const string& filepath, ReservationManager& manager) {
	ofstream outputFile(filepath);
	if (!outputFile.is_open()) {
		cout << "Error: could not save to " << filepath << "\n";
		return false;
	}

	manager.forEachReservation([&](Reservation& r) {
		outputFile << r.getReservationID() << "|"
			<< r.getResourceID() << "|"
			<< r.getStudentID() << "|"
			<< r.getStudentName() << "|"
			<< r.getReservationDate() << "\n";
		});

	outputFile.close();
	return true;
}

// =====================================================
// Searching helpers (linear search)
// =====================================================

// Finds a resource by ID. Returns nullptr if no match is found.
static Resource* findResourceByID(vector<Resource>& resources, const string& id) {
	for (Resource& r : resources) {
		if (r.getResourceID() == id) {
			return &r;
		}
	}
	return nullptr;
}

// Generates the next unique reservation ID (RES001, RES002, ...) by
// scanning the currently loaded reservations for the highest suffix used.
static string generateReservationID(ReservationManager& manager) {
	static int nextNumber = 0;

	if (nextNumber == 0) {
		int highest = 0;
		manager.forEachReservation([&](Reservation& r) {
			string id = r.getReservationID();
			if (id.rfind("RES", 0) == 0) {
				int value = atoi(id.substr(3).c_str());
				if (value > highest) {
					highest = value;
				}
			}
			});
		nextNumber = highest;
	}

	++nextNumber;

	stringstream ss;
	ss << "RES" << setw(3) << setfill('0') << nextNumber;
	return ss.str();
}

// =====================================================
// Display helpers
// =====================================================

static void displayResourcesTable(vector<Resource>& resources) {
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

static void displayReservationsTable(const vector<Reservation>& reservations) {
	if (reservations.empty()) {
		cout << "No reservations found.\n";
		return;
	}

	cout << left
		<< setw(10) << "Res. ID"
		<< setw(10) << "Res ID"
		<< setw(12) << "Student ID"
		<< setw(20) << "Student Name"
		<< "Date" << endl;
	cout << string(65, '-') << endl;

	for (const Reservation& r : reservations) {
		Reservation& rr = const_cast<Reservation&>(r);
		cout << left
			<< setw(10) << rr.getReservationID()
			<< setw(10) << rr.getResourceID()
			<< setw(12) << rr.getStudentID()
			<< setw(20) << rr.getStudentName()
			<< rr.getReservationDate() << endl;
	}

	cout << "Total active reservations: " << reservations.size() << endl;
}

// =====================================================
// Menu Option 2: View Resources
// =====================================================

static void viewResources(vector<Resource>& resources) {
	cout << "\n=== View Resources ===\n";
	displayResourcesTable(resources);
}

// =====================================================
// Menu Option 3: Create Reservation
// =====================================================

static void createReservation(vector<Resource>& resources, ReservationManager& manager) {
	cout << "\n=== Create Reservation ===\n";

	string resourceID;
	cout << "Resource ID: ";
	getline(cin, resourceID);

	Resource* resource = findResourceByID(resources, resourceID);
	if (resource == nullptr) {
		cout << "Error: no resource found with ID \"" << resourceID << "\".\n";
		return;
	}

	if (resource->getAvailabilityStatus() != "Available") {
		cout << "Resource \"" << resourceID << "\" is currently "
			<< resource->getAvailabilityStatus() << " and cannot be reserved.\n";
		return;
	}

	int studentID = 0;
	cout << "Student ID: ";
	if (!(cin >> studentID)) {
		clearInputLine();
		cout << "Invalid student ID.\n";
		return;
	}
	clearInputLine();

	string studentName;
	cout << "Student Name: ";
	getline(cin, studentName);

	string date;
	cout << "Reservation Date (YYYY-MM-DD): ";
	getline(cin, date);

	Reservation reservation;
	reservation.setReservationID(generateReservationID(manager));
	reservation.setResourceID(resourceID);
	reservation.setStudentID(studentID);
	reservation.setStudentName(studentName);
	reservation.setReservationDate(date);

	manager.addReservation(reservation);
	resource->setAvailabilityStatus("Reserved");

	saveResources(RESOURCES_FILE, resources);
	saveReservations(RESERVATIONS_FILE, manager);

	cout << "Reservation Created Successfully. (Reservation ID: "
		<< reservation.getReservationID() << ")\n";
}

// =====================================================
// Menu Option 4: Cancel Reservation
// =====================================================

// After a resource is freed by a cancellation, this checks the waiting
// list (in FIFO order) for the next student waiting on that specific
// resource, skipping over - and preserving the order of - anyone else
// waiting on a different resource.
static bool assignNextWaitingStudent(
	WaitingList& waitingList,
	vector<Resource>& resources,
	ReservationManager& manager,
	const string& resourceID) {

	vector<WaitingRequest> skipped;
	bool assigned = false;
	WaitingRequest current;

	while (waitingList.removeStudent(current)) {
		if (!assigned && current.resourceID == resourceID) {
			Reservation reservation;
			reservation.setReservationID(generateReservationID(manager));
			reservation.setResourceID(current.resourceID);
			reservation.setStudentID(current.studentID);
			reservation.setStudentName(current.studentName);
			reservation.setReservationDate(current.waitingDate);

			manager.addReservation(reservation);

			Resource* resource = findResourceByID(resources, resourceID);
			if (resource != nullptr) {
				resource->setAvailabilityStatus("Reserved");
			}

			cout << "Waiting list: assigned " << current.studentName
				<< " (Student ID " << current.studentID << ") to resource "
				<< resourceID << ".\n";

			assigned = true;
		}
		else {
			skipped.push_back(current);
		}
	}

	// Put everyone who wasn't assigned back into the queue, in the
	// same relative order they were in before.
	for (WaitingRequest& req : skipped) {
		waitingList.addStudent(req.studentID, req.studentName, req.resourceID, req.waitingDate);
	}

	return assigned;
}

static void cancelReservation(
	vector<Resource>& resources,
	WaitingList& waitingList,
	ReservationManager& manager) {

	cout << "\n=== Cancel Reservation ===\n";

	string reservationID;
	cout << "Reservation ID: ";
	getline(cin, reservationID);

	Reservation* found = manager.findReservation(reservationID);
	if (found == nullptr) {
		cout << "Error: no reservation found with ID \"" << reservationID << "\".\n";
		return;
	}

	// Copy it first: cancelReservation() deletes the list node and
	// pushes the reservation onto the cancellation history stack.
	Reservation cancelled = *found;
	manager.cancelReservation(reservationID);

	cout << "Reservation Cancelled.\n";
	cout << "Added to cancellation history.\n";

	Resource* resource = findResourceByID(resources, cancelled.getResourceID());
	if (resource != nullptr) {
		resource->setAvailabilityStatus("Available");

		bool assigned = assignNextWaitingStudent(
			waitingList, resources, manager, cancelled.getResourceID());

		if (!assigned) {
			cout << "Resource \"" << cancelled.getResourceID()
				<< "\" is now available.\n";
		}
	}

	saveResources(RESOURCES_FILE, resources);
	saveReservations(RESERVATIONS_FILE, manager);
}

// =====================================================
// Menu Option 5: View Waitlist
// =====================================================

static void viewWaitlist(const WaitingList& waitingList) {
	cout << "\n=== View Waiting List ===\n";
	waitingList.display();
}

static void joinWaitlist(vector<Resource>& resources, WaitingList& waitingList) {
	cout << "\n=== Join Waiting List ===\n";

	string resourceID;
	cout << "Resource ID: ";
	getline(cin, resourceID);

	Resource* resource = findResourceByID(resources, resourceID);
	if (resource == nullptr) {
		cout << "Error: no resource found with ID \"" << resourceID << "\".\n";
		return;
	}

	int studentID = 0;
	cout << "Student ID: ";
	if (!(cin >> studentID)) {
		clearInputLine();
		cout << "Invalid student ID.\n";
		return;
	}
	clearInputLine();

	string studentName;
	cout << "Student Name: ";
	getline(cin, studentName);

	string waitingDate;
	cout << "Requested Date (YYYY-MM-DD): ";
	getline(cin, waitingDate);

	waitingList.addStudent(studentID, studentName, resourceID, waitingDate);
	cout << "Added to the waiting list for \"" << resourceID << "\".\n";
}

// =====================================================
// Menu Option 6: Search Reservations
// =====================================================

static void searchReservations(ReservationManager& manager) {
	cout << "\n=== Search Reservations ===\n";
	cout << "1. Search by Reservation ID\n";
	cout << "2. Search by Student ID\n";
	cout << "3. Search by Student Name\n";
	cout << "Pick an option: ";

	int option = 0;
	if (!(cin >> option)) {
		clearInputLine();
		cout << "Invalid input.\n";
		return;
	}
	clearInputLine();

	vector<Reservation> matches;

	if (option == 1) {
		string id;
		cout << "Reservation ID: ";
		getline(cin, id);

		// Linear search by reservation ID.
		manager.forEachReservation([&](Reservation& r) {
			if (r.getReservationID() == id) {
				matches.push_back(r);
			}
			});
	}
	else if (option == 2) {
		int studentID = 0;
		cout << "Student ID: ";
		if (!(cin >> studentID)) {
			clearInputLine();
			cout << "Invalid student ID.\n";
			return;
		}
		clearInputLine();

		// Linear search by student ID.
		manager.forEachReservation([&](Reservation& r) {
			if (r.getStudentID() == studentID) {
				matches.push_back(r);
			}
			});
	}
	else if (option == 3) {
		string name;
		cout << "Student Name: ";
		getline(cin, name);

		// Linear search by student name.
		manager.forEachReservation([&](Reservation& r) {
			if (r.getStudentName() == name) {
				matches.push_back(r);
			}
			});
	}
	else {
		cout << "That option is not valid.\n";
		return;
	}

	if (matches.empty()) {
		cout << "No matching reservations found.\n";
	}
	else {
		displayReservationsTable(matches);
	}
}

// =====================================================
// Menu Option 7: Sort Resources (Quick Sort)
// =====================================================

enum class SortKey { ID, NAME };

static bool resourceLessThan(Resource& a, Resource& b, SortKey key) {
	if (key == SortKey::ID) {
		return a.getResourceID() < b.getResourceID();
	}
	return a.getResourceName() < b.getResourceName();
}

// Picks the last element as the pivot, moves everything smaller than it
// to the left and everything larger to the right, then places the pivot
// in its final sorted position and returns that position.
static int partitionResources(vector<Resource>& resources, int low, int high, SortKey key) {
	Resource pivot = resources[high];
	int i = low - 1;

	for (int j = low; j < high; ++j) {
		if (resourceLessThan(resources[j], pivot, key)) {
			++i;
			swap(resources[i], resources[j]);
		}
	}

	swap(resources[i + 1], resources[high]);
	return i + 1;
}

// Recursively partitions the resource list around a pivot (Quick Sort, O(n log n) average).
static void quickSortResources(vector<Resource>& resources, int low, int high, SortKey key) {
	if (low < high) {
		int pivotIndex = partitionResources(resources, low, high, key);
		quickSortResources(resources, low, pivotIndex - 1, key);
		quickSortResources(resources, pivotIndex + 1, high, key);
	}
}

static void sortResourcesMenu(vector<Resource>& resources) {
	cout << "\n=== Sort Resources ===\n";

	if (resources.empty()) {
		cout << "No resources to sort.\n";
		return;
	}

	cout << "Sort by:\n";
	cout << "1. Resource ID\n";
	cout << "2. Resource Name\n";
	cout << "Pick an option: ";

	int keyChoice = 0;
	if (!(cin >> keyChoice)) {
		clearInputLine();
		cout << "Invalid input.\n";
		return;
	}
	clearInputLine();

	SortKey key = (keyChoice == 2) ? SortKey::NAME : SortKey::ID;

	quickSortResources(resources, 0, static_cast<int>(resources.size()) - 1, key);

	cout << "\nResources sorted by " << (key == SortKey::ID ? "ID" : "Name") << ":\n";
	displayResourcesTable(resources);
}

// =====================================================
// Menu Option 8: Generate Reports
// =====================================================

static void generateReports(
	vector<Resource>& resources,
	ReservationManager& manager,
	const WaitingList& waitingList) {

	cout << "\n=== Generate Reports ===\n";

	// --- Current availability ---
	int availableCount = 0;
	int unavailableCount = 0;
	for (Resource& r : resources) {
		if (r.getAvailabilityStatus() == "Available") {
			++availableCount;
		}
		else {
			++unavailableCount;
		}
	}

	cout << "\n-- Current Availability --\n";
	cout << "Available resources:   " << availableCount << endl;
	cout << "Unavailable resources: " << unavailableCount << endl;
	cout << "Total resources:       " << resources.size() << endl;

	// --- Number of active reservations ---
	cout << "\n-- Active Reservations --\n";
	cout << "Total active reservations: " << manager.reservationCount() << endl;

	// --- Most frequently reserved resource ---
	// Frequency count uses a map keyed by resource ID.
	map<string, int> frequency;
	manager.forEachReservation([&](Reservation& r) {
		frequency[r.getResourceID()]++;
		});

	cout << "\n-- Most Frequently Reserved Resources --\n";
	if (frequency.empty()) {
		cout << "No active reservations yet.\n";
	}
	else {
		int highestCount = 0;
		for (const auto& entry : frequency) {
			if (entry.second > highestCount) {
				highestCount = entry.second;
			}
		}
		for (const auto& entry : frequency) {
			string marker = (entry.second == highestCount) ? "  <-- most reserved" : "";
			cout << "  " << left << setw(10) << entry.first
				<< "reservations: " << entry.second << marker << endl;
		}
	}

	// --- Waiting list report ---
	cout << "\n-- Waiting List --\n";
	cout << "Students waiting: " << waitingList.size() << endl;
	if (waitingList.size() > 0) {
		waitingList.display();
	}
}

// =====================================================

int main() {
	// Loads last saved datawhenever they change.
	vector<Resource> resources = loadResources(RESOURCES_FILE);
	WaitingList waitingList;
	ReservationManager manager;
	loadReservations(RESERVATIONS_FILE, manager);

	int choice = 0;
	bool exitRequested = false;

	while (!exitRequested) {
		cout << "\n===== Campus Resource Reservation System =====\n";
		cout << "1) Cancellation History / Undo (stack)\n";
		cout << "2) View Resources\n";
		cout << "3) Create Reservation\n";
		cout << "4) Cancel Reservation\n";
		cout << "5) View Waitlist\n";
		cout << "6) Search Reservations\n";
		cout << "7) Sort Resources\n";
		cout << "8) Generate Reports\n";
		cout << "9) Exit\n";
		cout << endl;
		cout << "Choice: ";

		if (!(cin >> choice)) {
			clearInputLine();
			cout << "Invalid input. Please enter a number.\n";
			continue;
		}
		clearInputLine();

		switch (choice) {

		case 1: {
			int menu = 0;

			do {
				cout << "\n--- Cancellation History (Stack) ---\n";
				cout << "1. Show cancellation history\n";
				cout << "2. Undo last cancellation\n";
				cout << "3. Go back\n";
				cout << "Pick an option: ";

				if (!(cin >> menu)) {
					clearInputLine();
					cout << "Invalid input. Please enter a number.\n";
					continue;
				}

				clearInputLine();

				if (menu == 1) {

					cout << "\nCancellation history ("
						<< manager.cancellationHistoryCount()
						<< " total):\n";

					if (manager.hasCancellationHistory()) {
						manager.displayCancellationHistory();
					}
					else {
						cout << "No cancellations saved yet.\n";
					}
				}

				else if (menu == 2) {

					Reservation last;

					if (!manager.lastCancellation(last)) {
						cout << "Cannot undo. History is empty.\n";
					}
					else {
						// The resource must still be free, otherwise restoring the
						// reservation would double-book it (for example, a waiting-list
						// student may have been assigned to it after the cancellation).
						Resource* resource = findResourceByID(resources, last.getResourceID());

						if (resource == nullptr) {
							cout << "Cannot undo. Resource \"" << last.getResourceID()
								<< "\" no longer exists.\n";
						}
						else if (resource->getAvailabilityStatus() != "Available") {
							cout << "Cannot undo. Resource \"" << last.getResourceID()
								<< "\" is now " << resource->getAvailabilityStatus()
								<< " (reserved by someone else).\n";
						}
						else {
							Reservation restored;
							manager.undoLastCancellation(restored);   // stack -> active list
							resource->setAvailabilityStatus("Reserved");

							saveResources(RESOURCES_FILE, resources);
							saveReservations(RESERVATIONS_FILE, manager);

							cout << "Undo worked. This reservation was restored:\n";
							restored.display();
							cout << "Resource \"" << restored.getResourceID()
								<< "\" is marked Reserved again.\n";
						}
					}
				}

				else if (menu == 3) {
					cout << "Going back to main menu...\n";
				}

				else {
					cout << "That option is not valid. Try again.\n";
				}

			} while (menu != 3);

			break;
		}
		case 2:
			viewResources(resources);
			break;

		case 3:
			createReservation(resources, manager);
			break;

		case 4:
			cancelReservation(resources, waitingList, manager);
			break;

		case 5: {
			int menu = 0;
			cout << "\n--- Waiting List ---\n";
			cout << "1. View waiting list\n";
			cout << "2. Join waiting list\n";
			cout << "Pick an option: ";

			if (!(cin >> menu)) {
				clearInputLine();
				cout << "Invalid input.\n";
				break;
			}
			clearInputLine();

			if (menu == 2) {
				joinWaitlist(resources, waitingList);
			}
			else {
				viewWaitlist(waitingList);
			}
			break;
		}

		case 6:
			searchReservations(manager);
			break;

		case 7:
			sortResourcesMenu(resources);
			break;

		case 8:
			generateReports(resources, manager, waitingList);
			break;

		case 9:
			cout << "Goodbye.\n";
			exitRequested = true;
			break;

		default:
			cout << "Unknown option.\n";
			break;
		}
	}

	saveResources(RESOURCES_FILE, resources);
	saveReservations(RESERVATIONS_FILE, manager);

	return 0;
}