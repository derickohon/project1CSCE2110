#include "../include/Resource.h"

using namespace std;

Resource::Resource() {
	resourceID = "";
	resourceName = "";
	resourceType = "";
	availabilityStatus = "";
}

string Resource::getResourceID() {
	return resourceID;
}

string Resource::getResourceName() {
	return resourceName;
}

string Resource::getResourceType() {
	return resourceType;
}

string Resource::getAvailabilityStatus() {
	return availabilityStatus;
}

void Resource::setResourceID(string id) {
	resourceID = id;
}

void Resource::setResourceName(string name) {
	resourceName = name;
}

void Resource::setResourceType(string type) {
	resourceType = type;
}

void Resource::setAvailabilityStatus(string status) {
	availabilityStatus = status;
}