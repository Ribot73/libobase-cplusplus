#include <libobase/file.hpp>


void SerialHeader::add(unsigned int value) {
	char* intBytes = reinterpret_cast<char*>(&value) + 3;

	*(current++) = *(intBytes--);
	*(current++) = *(intBytes--);
	*(current++) = *(intBytes--);
	*(current++) = *intBytes;
};

unsigned int SerialHeader::get() {
	unsigned int value;
	char* intBytes = reinterpret_cast<char*>(&value) + 3;

	*(intBytes--) = *(current++);
	*(intBytes--) = *(current++);
	*(intBytes--)= *(current++);
	*intBytes = *(current++);

	return value;
};

void SerialHeader::calculate(vector<unsigned int> & boundaries) {
	current = &(bytes[TOTAL_LENGTH]);
	for(std::vector<unsigned int>::iterator it = boundaries.begin(); it < boundaries.end(); it++) {
		add(*it);
	}
	current = bytes;
	size = boundaries.back() + 1;
	add(size);
};

void SerialHeader::parse(vector<unsigned int> & boundaries) {
	current = &(bytes[TOTAL_LENGTH]);
	boundaries.clear();
	for(int pos = 0; pos < col_amount; pos++) {
		boundaries.push_back(get());
	}
};

void SerialHeader::parse(unsigned short position, unsigned int & start, unsigned int & length) {
	char* previous = current;

	current = &(bytes[TOTAL_LENGTH + (position - 2) * COLUMN_LENGTH]);
	start = (position == 1) ? 0 : get();
	current = &(bytes[TOTAL_LENGTH + (position - 1) * COLUMN_LENGTH]);
	length = get() - start;

	current = previous;
}

void SerialHeader::write(ofstream & odataFile) {
	odataFile.write(bytes, col_length);
};

unsigned int SerialHeader::read(ifstream & idataFile) {
	if(idataFile.read(bytes, col_length)) {
		current = bytes;
		size = get();
		return 1;
	} else {
		return 0;
	};
};
