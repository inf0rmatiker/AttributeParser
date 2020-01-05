#pragma once

#include <string>
#include <vector>
#include "Attribute.h"

/* Represents a tag. Contains a list of attributes, and children Tag objects. */
class Tag {
public:
	Tag();
	Tag(std::string);
	~Tag() = default;

	std::string getAttributesAsString();

	std::string name;
	std::vector<Attribute> attributes;
	std::vector<Tag> children;
};