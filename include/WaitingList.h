#ifndef WAITINGLIST_H
#define WAITINGLIST_H

#include <string>

using namespace std;

// Information Stored about one student
struct WaitingRequest {
    int studentID;
    string studentName;
    string resourceID;

    WaitingRequest();
    WaitingRequest(int id, string name, string resource);
};

// Used node by the waiting-list linked queue
struct WaitingNode {
    WaitingRequest request;
    WaitingNode* next;

    WaitingNode(WaitingRequest req);
};

// Manages students waiting unavailable resources by that Queue.
// FIFO (First In, First Out) was processed.
class WaitingList {
private:
    WaitingNode* front;
    WaitingNode* rear;
    int count;

public:
    // Constructor
    WaitingList();

    // Destructor
    ~WaitingList();

    // Adds a student to the back of the waiting list.
    void addStudent(int studentID, string studentName, string resourceID);

    // Removes the student at the front of the waiting list.
    // Returns false if the waiting list is empty.
    bool removeStudent(WaitingRequest& request);

    // Returns true if there are no students waiting.
    bool isEmpty() const;

    // Returns the number of students currently waiting.
    int size() const;

    // Displays all students in FIFO order.
    void display() const;

    // Removes nodes from waiting list.
    void clear();
};

#endif // WAITINGLIST_H
