
#ifndef WAITINGLIST_H
#define WAITINGLIST_H

#include <string>

using namespace std;

struct WaitingRequest {
    int studentID;
    string studentName;
    string resourceID;
    string waitingDate;

    WaitingRequest();
    WaitingRequest(int id, string name, string resource);
    WaitingRequest(int id, string name, string resource, string date);
};

struct WaitingNode {
    WaitingRequest request;
    WaitingNode* next;

    WaitingNode(WaitingRequest req);
};

class WaitingList {
private:
    WaitingNode* front;
    WaitingNode* rear;
    int count;

public:
    WaitingList();
    ~WaitingList();

    WaitingList(const WaitingList&) = delete;
    WaitingList& operator=(const WaitingList&) = delete;

    void addStudent(int studentID, string studentName, string resourceID);
    void addStudent(int studentID, string studentName, string resourceID, string waitingDate);

    bool removeStudent(WaitingRequest& request);

    bool isEmpty() const;
    int size() const;

    void display() const;
    void clear();
};

#endif

