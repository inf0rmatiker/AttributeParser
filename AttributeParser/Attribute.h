#pragma once

#include <vector>
#include <string>

/* Represents a single (name, value) attribute of a tag.
   Definitions provided in Attribute.cpp.
*/
class Attribute {
public:
	Attribute(std::string, std::string);
	Attribute(std::string);
	Attribute();

	~Attribute() = default;
	std::string name, value;
};