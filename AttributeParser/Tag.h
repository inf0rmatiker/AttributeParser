#pragma once

#include <string>
#include <vector>
#include "Attribute.h"

class Tag {
public:
	Tag() { Tag(""); }
	Tag(std::string tagName) : name(tagName) {}
	~Tag() = default;

	std::string getAttributesAsString() {
		std::string attributesStr = "";
		for (unsigned int i = 0; i < attributes.size(); i++) {
			attributesStr += attributes[i].name + " = \"" + attributes[i].value + "\"";
		}

		return attributesStr;
	}

	std::string name;
	std::vector<Attribute> attributes;
	std::vector<Tag> children;
};