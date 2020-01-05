#include <string>
#include "Attribute.h"

using namespace std;

// Constructors
Attribute::Attribute(string attrName, string attrVal) : name(attrName), value(attrVal) {}
Attribute::Attribute(string attrName) { Attribute(attrName, ""); }
Attribute::Attribute() { Attribute(""); }