#include "../include/WaitingList.h"
#include <iostream>

using namespace std;


WaitingRequest::WaitingRequest() {
    studentID = 0;
    studentName = "";
    resourceID = "";
}


WaitingRequest::WaitingRequest(int id, string name, string resource) {
    studentID = id;
    studentName = name;
    resourceID = resource;
}


WaitingNode::WaitingNode(WaitingRequest req) {
    request = req;
    next = nullptr;
}


WaitingList::WaitingList() {
    front = nullptr;
    rear = nullptr;
    count = 0;
}


WaitingList::~WaitingList() {
    clear();
}


void WaitingList::addStudent(int studentID, string studentName, string resourceID) {
    WaitingRequest request(studentID, studentName, resourceID);

    WaitingNode* newNode = new WaitingNode(request);

    
    if (isEmpty()) {
        front = newNode;
        rear = newNode;
    }
    else {
    
        rear->next = newNode;
        rear = newNode;
    }

    count++;
}


bool WaitingList::removeStudent(WaitingRequest& request) {
    // Cannot remove a student from an empty queue.
    if (isEmpty()) {
        return false;
    }

    WaitingNode* temp = front;
 
    request = temp->request;

    front = front->next;

    delete temp;
    count--;

     if (front == nullptr) {
        rear = nullptr;
    }

    return true;
}

bool WaitingList::isEmpty() const {
    return front == nullptr;
}

int WaitingList::size() const {
    return count;
}

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

void WaitingList::clear() {
    while (front != nullptr) {
        WaitingNode* temp = front;
        front = front->next;
        delete temp;
    }

    rear = nullptr;
    count = 0;
}
