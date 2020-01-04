// AttributeParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <regex>

using namespace std;

class Attribute {
public:
	// Constructors
	Attribute(string attrName, string attrVal) : name(attrName), value(attrVal) {}
	Attribute(string attrName) { Attribute(attrName, ""); }
	Attribute() { Attribute(""); }

	~Attribute() = default;
	string name, value;
};

class Tag {
public:
	Tag() { Tag(""); }
	Tag(string tagName) : name(tagName) {}
	~Tag() = default;

	string getAttributesAsString() {
		string attributesStr = "";
		for (unsigned int i = 0; i < attributes.size(); i++) {
			attributesStr += attributes[i].name + " = \"" + attributes[i].value + "\"";
		}

		return attributesStr;
	}

	string name;
	vector<Attribute> attributes;
	vector<Tag> children;
};

string formatValue(string value) {
	if (!value.find(">")) {
		return value.substr(1, value.size() - 2);
	}
	return value.substr(1, value.size() - 1);
}

vector<Attribute> parseAttributes(istringstream &iss) {
	vector<Attribute> attributes;
	while (iss) {
		string name, value;
		char equalSign, currentChar;
		iss >> name >> equalSign;

		if (equalSign == '=') { // Error check
			iss.get(currentChar); iss.get(currentChar); // Read space char and "
			if (currentChar == '"') {
				while (iss.get(currentChar)) {
					if (currentChar == '"') {
						break;
					}
					else {
						value += currentChar;
					}
				}
			}
			else { cout << "Incorrect attribute format!\n"; }

		}
		else { cout << "Incorrect attribute format!\n"; }
		attributes.push_back(Attribute(name, value));
		iss.get(currentChar);
		if (currentChar == '>') {
			return attributes;
		}
	}

	return attributes;
}

vector<Tag> parseTag(ifstream &fileIn) {
	vector<Tag> tagList;
	string line;
	bool withinTag = false;

	while (getline(fileIn, line)) {
		bool foundClosingPattern = line.find("</") != string::npos;
		if (foundClosingPattern) { // Handle closing tag
			return tagList;
		}
		else if (regex_match(line, regex("^\\s*$"))) { // Skip blank lines
			continue;
		}
		else { // Handle opening tag
			istringstream iss(line);
			string tagName; iss >> tagName;
			Tag currentTag;

			bool attributesExist = tagName.find('>') == string::npos;
			if (!attributesExist) { // No attributes with tag
				tagName = tagName.substr(1, tagName.size() - 2);
			}
			else { // Tag has attributes
				tagName = tagName.substr(1);
				currentTag.attributes = parseAttributes(iss);
			}

			currentTag.name = tagName;
			currentTag.children = parseTag(fileIn);
			tagList.push_back(currentTag); // Push new tag
		}
	}

	return tagList;
}

void printStructure(vector<Tag> &tagList) {
	for (Tag &tag : tagList) {
		string attributeStr = tag.getAttributesAsString();
		if (!attributeStr.empty()) { // Prepend a space if attributes exist
			attributeStr = ' ' + attributeStr;
		}

		cout << "<" << tag.name << attributeStr << ">\n";
		printStructure(tag.children);
		cout << "</" << tag.name << ">\n";
	}
}

Tag * getTag(string tagName, vector<Tag> & tags) {
	for (Tag & t : tags) {
		if (t.name == tagName) {
			return &t;
		}
	}
	return NULL;
}

Attribute * getAttribute(Tag *tag, string attrName) {
	for (Attribute & a : tag->attributes) {
		if (a.name == attrName) {
			return &a;
		}
	}
	return NULL;
}

string queryTags(string query, vector<Tag> tags) {
	istringstream iss(query);
	string token = "";
	char c;

	Tag *tag;
	while (iss.get(c)) {
		if (c == '.') {
			tag = getTag(token, tags);
			if (tag == NULL) {
				return "Not Found!";
			}

			tags = tag->children;
			token = ""; // Reset token
		}
		else if (c == '~') {
			// Handle attribute
			tag = getTag(token, tags);

			if (tag == NULL) {
				return "Not Found!";
			}
			string attrName; iss >> attrName;
			Attribute * attr = getAttribute(tag, attrName);
			if (attr == NULL) {
				return "Not Found!";
			}
			return attr->value;
		}
		else {
			token += c;
		}
	}
	return "Not Found!";
}

int main() {
	ifstream fileIn("tags_test_0.txt");
	string line;

	vector<Tag> tags = parseTag(fileIn);
	printStructure(tags);
	/*
	for (int i = 0; i < queries; i++) {
		getline(fileIn, line);
		string queryResult = queryTags(line, tags);
		cout << queryResult << '\n';
	}
	*/
	return 0;
}