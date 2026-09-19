#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
using namespace std;

class Reservation {
private:
	string reservationID;
	string resourceID;
	int studentID;
	string studentName;
	string reservationDate;

public:
	Reservation();

	string getReservationID();
	string getResourceID();
	int getStudentID();
	string getStudentName();
	string getReservationDate();

	void setReservationID(string id);
	void setResourceID(string id);
	void setStudentID(int id);
	void setStudentName(string name);
	void setReservationDate(string date);
};

#endif // RESERVATION_H