#pragma once

#include <vector>
#include <string>

/* Represents a single (name, value) attribute of a tag.
   Definitions provided in Attribute.cpp.
*/
class Attribute {
public:
	Attribute(std::string attrName, std::string attrVal);
	Attribute(std::string attrName);
	Attribute();

	~Attribute() = default;
	std::string name, value;
};