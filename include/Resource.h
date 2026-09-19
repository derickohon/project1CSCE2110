#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
using namespace std;

class Resource {
private:
	string resourceID;
	string resourceName;
	string resourceType;
	string availabilityStatus;

public:
	Resource();

	string getResourceID();
	string getResourceName();
	string getResourceType();
	string getAvailabilityStatus();

	void setResourceID(string id);
	void setResourceName(string name);
	void setResourceType(string type);
	void setAvailabilityStatus(string status);
};

#endif // RESOURCE_H