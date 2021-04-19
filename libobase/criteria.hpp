#ifndef LIBOBASE_CRITERIA_HPP_
#define LIBOBASE_CRITERIA_HPP_

#include <string>

using namespace std;

class Criterium {
	public:
		virtual bool valid(const char* leftBytes, unsigned int leftLength, const char* rightBytes, unsigned int rightLength) = 0;
		virtual bool valid(const char* leftBytes, unsigned int leftLength, string value) = 0;
};

class StringEquals : public Criterium {
	public:
		StringEquals() {};
		virtual bool valid(const char* leftBytes, unsigned int leftLength, const char* rightBytes, unsigned int rightLength);
		virtual bool valid(const char* leftBytes, unsigned int leftLength, string value);
};

#endif /* LIBOBASE_CRITERIA_HPP_ */
