#pragma once

#include <vector>
#include <string>

class Attribute {
public:
	// Constructors
	Attribute(std::string attrName, std::string attrVal) : name(attrName), value(attrVal) {}
	Attribute(std::string attrName) { Attribute(attrName, ""); }
	Attribute() { Attribute(""); }

	~Attribute() = default;
	std::string name, value;
};