#include "Reservation.h"

Reservation::Reservation() { // Constructor to initialize reservation values
	reservationID = "";
	resourceID = "";
	studentID = 0;
	studentName = "";
	reservationDate = "";
}

string Reservation::getReservationID() { // Get the reservation ID
	return reservationID;
}

string Reservation::getResourceID() { // Get the resource ID associated with the reservation
	return resourceID;
}

int Reservation::getStudentID() { // Get the student ID associated with the reservation
	return studentID;
}

string Reservation::getStudentName() { // Get the student name associated with the reservation
	return studentName;
}

string Reservation::getReservationDate() { // Get the reservation date
	return reservationDate;
}

void Reservation::setReservationID(string id) { // Set the reservation ID
	reservationID = id;
}

void Reservation::setResourceID(string id) { // Set the resource ID associated with the reservation
	resourceID = id;
}

void Reservation::setStudentID(int id) { // Set the student ID associated with the reservation
	studentID = id;
}

void Reservation::setStudentName(string name) { // Set the student name associated with the reservation
	studentName = name;
}

void Reservation::setReservationDate(string date) { // Set the reservation date
	reservationDate = date;
}