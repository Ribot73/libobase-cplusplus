#include <libobase/streamer.hpp>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

void SerialRecord::write(string &value) {
	write(current, value);
	current += value.length();
};

void SerialRecord::write(unsigned short int value) {
	write(current, value);
	current += 2;
};

void SerialRecord::write(unsigned int value) {
	write(current, value);
	current += 4;
};

unsigned int SerialRecord::readInt() {
	unsigned int value;
	char* intBytes = reinterpret_cast<char*>(&value) + 3;

	*(intBytes--) = *(current++);
	*(intBytes--) = *(current++);
	*(intBytes--) = *(current++);
	*intBytes = *(current++);

	return value;
};

unsigned short int SerialRecord::readShort() {
	unsigned int value;
	char* intBytes = reinterpret_cast<char*>(&value) + 1;

	*(intBytes--) = *(current++);
	*intBytes = *(current++);

	return value;
};

void SerialRecord::readString(string &content, unsigned short int col_pos) {
	column_max length = boundaries[col_pos - 1] - (col_pos == 1 ? 0 : boundaries[col_pos - 2]);
	content.assign(current, length);
	current += length;
};

void SerialRecord::resize(record_max isize) {
	if(0 == isize) {
		delete [] bytes;
		bytes = 0;
	} else if(isize > size) {
		delete [] bytes;
		bytes = new char[isize];
	}
	current = bytes;
};

void SerialRecord::parse(unsigned short position, unsigned int & start, unsigned int & length) {
	start = (position == 1 ? 0 : boundaries[position -2]);
	length = boundaries[position -1] - start;
}


SerialRecord::SerialRecord(record_max isize, unsigned short int col_amount) : size(isize) {
	boundaries.reserve(col_amount);
	bytes = (isize ? new char[isize] : 0);
	current = bytes;
};

SerialRecord::SerialRecord(unsigned short int col_amount) : SerialRecord(0, col_amount) {};

SerialRecord::~SerialRecord() {
	delete [] bytes;
};



