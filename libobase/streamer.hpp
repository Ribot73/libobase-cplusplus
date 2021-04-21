#ifndef LIBOBASE_STREAMER_HPP_
#define LIBOBASE_STREAMER_HPP_

#include <cstring>
#include <string>
#include <vector>

typedef unsigned int entity_size;
typedef unsigned int record_max;
typedef unsigned short int column_max;

using namespace std;

class SerialRecord {
	private:
		char* bytes;
		char* current;
		record_max size;

	public:
		vector<unsigned int> boundaries;

	public:
		static void write(char* target, string & ivalue) {
			memcpy(target, ivalue.data(), ivalue.length());
		};

		static void write(char* target, const char* ibytes, unsigned int ilength) {
			memcpy(target, ibytes, ilength);
		};

		static void write(char* target, unsigned short int value) {
			char* intBytes = reinterpret_cast<char*>(&value) + 1;

			*(target++) = *(intBytes--);
			*(target++) = *intBytes;
		};

		static void write(char* target, unsigned int value) {
			char* intBytes = reinterpret_cast<char*>(&value) + 3;

			*(target++) = *(intBytes--);
			*(target++) = *(intBytes--);
			*(target++) = *(intBytes--);
			*(target++) = *(intBytes--);
		};

	public:
		void write(const char* ibytes, unsigned int ilength);
		void write(string &value);
		void write(unsigned int value);
		void write(unsigned short int value);
		void readString(string &content, unsigned short int col_pos);
		unsigned int readInt();
		unsigned short int readShort();
		inline char* getBytes() { return bytes; };
		inline vector<record_max>& getBoundaries() { return boundaries; };
		void resize(record_max isize);
		void parse(unsigned short position, unsigned int & start, unsigned int & length);
		SerialRecord(const SerialRecord & serialRecord);
		SerialRecord(unsigned short int col_amount);
		SerialRecord(record_max isize, unsigned short int col_amount);
		~SerialRecord();
};

#endif /* LIBOBASE_STREAMER_HPP_ */
