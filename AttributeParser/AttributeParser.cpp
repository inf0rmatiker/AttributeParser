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
#include <ctype.h>
#include <exception>

// Non-STL includes
#include "Attribute.h"
#include "Tag.h"

using namespace std;

/* Strips the quotation marks off the attribute value.
	Also removes ending '>', if included. */
string formatValue(string value) {
	if (!value.find(">")) {
		return value.substr(1, value.size() - 2);
	}
	return value.substr(1, value.size() - 1);
}

/* Gets the stream iterator to the first
	non-whitespace character. */
void skipWhiteSpace(istringstream &iss) {
	char c;
	while (iss.get(c)) {
		if (!isspace(c)) {
			iss.putback(c);
			return;
		}
	}
}

string readAttributeName(istringstream &iss) {
	string name = "";
	char c;
	while (iss.get(c)) {
		if (c != '=') {
			name += c;
		}
		else {
			iss.get(c);
			if (c == '"') {
				break;
			}
			else {
				throw logic_error("Malformed attribute: " + name);
			}
		}
	}
	return name;
}

string readAttributeValue(istringstream &iss) {
	string value = "";
	char c;
	while (iss.get(c)) {
		if (c == '"') {
			break;
		}
		else {
			value += c;
		}
	}
	return value;
}

/* Reads a single attribute (e.g. name="value") from the istringstream
	and returns it as an Attribute object. */
Attribute readAttribute(istringstream &iss) {
	skipWhiteSpace(iss);
	try {
		string name = readAttributeName(iss);
		string value = readAttributeValue(iss);
		return Attribute(name, value);
	}
	catch (const logic_error &e) {
		throw e;
	}
}

/* Takes a single opening tag line, and parses the attributes out of it. */
vector<Attribute> parseAttributes(istringstream &iss) {
	vector<Attribute> attributes;
	while (iss) {
		// Test for ending '>'
		skipWhiteSpace(iss);
		char c; iss.get(c);
		if (c == '>') {
			return attributes;
		} 
		else {
			iss.putback(c);
			attributes.push_back(readAttribute(iss));
		}
	}

	throw logic_error("No closing '>' found on line!");
}

/* Recursively populates a std::vector of Tag objects for each level. */
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
				try {
					currentTag.attributes = parseAttributes(iss);
				}
				catch (const logic_error &e) {
					throw e;
				}
			}

			currentTag.name = tagName;

			// Recursive call; throw any exceptions up the call stack
			try {
				currentTag.children = parseTag(fileIn);
			}
			catch (const logic_error &e) {
				throw e;
			}
			
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
	ifstream fileIn("attributes_test_0.txt");
	string line;

	// We use a std::vector here, because even though it has O(n) lookup
	// time, it preserves the order of the tags unlike a map.
	try {
		vector<Tag> tags = parseTag(fileIn);
		printStructure(tags);
	}
	catch (const logic_error &e) {
		cerr << "Problem parsing tags: " << e.what() << '\n';
		return 1;
	}
	
	
	return 0;
}