#include <vector>
#include <string>
#include "Attribute.h"
#include "Tag.h"

using namespace std;

Tag::Tag() { Tag(""); }
Tag::Tag(string tagName) : name(tagName) {}

string Tag::getAttributesAsString() {
	string attributesStr = "";
	for (unsigned int i = 0; i < attributes.size(); i++) {
		attributesStr += attributes[i].name + " = \"" + attributes[i].value + "\"";
	}

	return attributesStr;
}
