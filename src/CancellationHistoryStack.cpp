#include "CancellationHistoryStack.h"

#include <iostream>

using namespace std;

CancellationHistoryStack::CancellationHistoryStack() : top(nullptr) {}

CancellationHistoryStack::~CancellationHistoryStack() {
	clear();
}

void CancellationHistoryStack::push(const Reservation& reservation) {
	top = new StackNode(reservation, top);
}

bool CancellationHistoryStack::pop(Reservation& outReservation) {
	if (top == nullptr) {
		return false;
	}

	StackNode* nodeToRemove = top;
	outReservation = nodeToRemove->data;
	top = top->next;
	delete nodeToRemove;
	return true;
}

bool CancellationHistoryStack::isEmpty() const {
	return top == nullptr;
}

int CancellationHistoryStack::size() const {
	int count = 0;
	for (StackNode* current = top; current != nullptr; current = current->next) {
		++count;
	}
	return count;
}

void CancellationHistoryStack::displayHistory() const {
	if (top == nullptr) {
		cout << "  (no cancelled reservations in history)" << endl;
		return;
	}

	cout << "  --- Cancellation history (most recent first) ---" << endl;
	int index = 1;
	for (StackNode* current = top; current != nullptr; current = current->next) {
		cout << "  " << index << ". ";
		current->data.display();
		++index;
		cout << "-----------------------------------" << endl;
	}
}

void CancellationHistoryStack::clear() {
	while (top != nullptr) {
		StackNode* nodeToRemove = top;
		top = top->next;
		delete nodeToRemove;
	}
}
