#include "WaitingList.h"
#include <iostream>

using namespace std;

// WaitingRequest default constructor
WaitingRequest::WaitingRequest() {
    studentID = 0;
    studentName = "";
    resourceID = "";
}

// WaitingRequest parameterized constructor
WaitingRequest::WaitingRequest(int id, string name, string resource) {
    studentID = id;
    studentName = name;
    resourceID = resource;
}

// WaitingNode constructor
WaitingNode::WaitingNode(WaitingRequest req) {
    request = req;
    next = nullptr;
}

// WaitingList constructor
WaitingList::WaitingList() {
    front = nullptr;
    rear = nullptr;
    count = 0;
}

// WaitingList destructor
WaitingList::~WaitingList() {
    clear();
}

// Adds a student to the back of the waiting queue.
void WaitingList::addStudent(int studentID, string studentName, string resourceID) {
    WaitingRequest request(studentID, studentName, resourceID);

    WaitingNode* newNode = new WaitingNode(request);

    // If the queue is empty, the new node becomes
    // both the front and rear.
    if (isEmpty()) {
        front = newNode;
        rear = newNode;
    }
    else {
        // Add the new node after the current rear.
        rear->next = newNode;
        rear = newNode;
    }

    count++;
}

// Removes the student at the front of the waiting queue.
bool WaitingList::removeStudent(WaitingRequest& request) {
    // Cannot remove a student from an empty queue.
    if (isEmpty()) {
        return false;
    }

    WaitingNode* temp = front;

    // Copy the request before deleting the node.
    request = temp->request;

    // Move front to the next student.
    front = front->next;

    delete temp;
    count--;

    // If the last student was removed,
    // reset rear as well.
    if (front == nullptr) {
        rear = nullptr;
    }

    return true;
}

// Checks whether the waiting queue is empty.
bool WaitingList::isEmpty() const {
    return front == nullptr;
}

// Returns the number of students currently waiting.
int WaitingList::size() const {
    return count;
}

// Displays all students in FIFO order.
void WaitingList::display() const {
    if (isEmpty()) {
        cout << "Waiting list is empty." << endl;
        return;
    }

    WaitingNode* current = front;
    int position = 1;

    cout << "\n===== Waiting List =====" << endl;

    while (current != nullptr) {
        cout << position << ". "
             << "Student ID: " << current->request.studentID
             << ", Name: " << current->request.studentName
             << ", Resource ID: " << current->request.resourceID
             << endl;

        current = current->next;
        position++;
    }

    cout << "Total waiting: " << count << endl;
}

// Removes every node from the waiting queue.
void WaitingList::clear() {
    while (front != nullptr) {
        WaitingNode* temp = front;
        front = front->next;
        delete temp;
    }

    rear = nullptr;
    count = 0;
}
