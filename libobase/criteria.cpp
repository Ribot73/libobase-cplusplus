#include <libobase/criteria.hpp>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

bool StringEquals::valid(const char* leftBytes, unsigned int leftLength, string value) {
	return (value.length() == leftLength) && !value.compare(0, leftLength, leftBytes, leftLength);
};

bool StringEquals::valid(const char* leftBytes, unsigned int leftLength, const char* rightBytes, unsigned int rightLength) {
	return (leftLength == rightLength) && !memcmp(leftBytes, rightBytes, leftLength);
};
