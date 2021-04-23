#ifndef LIBOBASE_FILE_HPP_
#define LIBOBASE_FILE_HPP_

#include <string>
#include <iostream>
#include <fstream>

#include <libobase/streamer.hpp>
#include <libobase/model.hpp>
#include <libobase/criteria.hpp>

class SerialHeader{
	public:
		static const unsigned short int TOTAL_LENGTH = 4;
		static const unsigned short int COLUMN_LENGTH = 4;
		unsigned short int col_amount;
		unsigned short int col_length;
	private:
		unsigned short int size = 0;
		char* bytes;
		char* current;

	private:
		void add(unsigned int value);
		unsigned int get();
	public:
		void write(ofstream & odataFile);
		unsigned int read(ifstream & idatafile);
		void calculate(vector<unsigned int> & boundaries);
		void parse(vector<unsigned int> & boundaries);
		void parse(unsigned short position, unsigned int & start, unsigned int & length);
		inline const char* getBytes() { return bytes; };
		inline unsigned short int getSize() { return size; };
		SerialHeader(unsigned short int icol_amount) : col_amount(icol_amount), col_length(TOTAL_LENGTH + (COLUMN_LENGTH*icol_amount)) {
			bytes = new char[col_length];
			current = bytes;
		};
		SerialHeader(const SerialHeader & serialHeader) : SerialHeader(serialHeader.col_amount) {};
		SerialHeader & operator=(const SerialHeader & serialHeader) {
			col_amount = serialHeader.col_amount;
			col_length = serialHeader.col_length;
			bytes = new char[col_length];
			current = bytes;
			return *this;

		};
		~SerialHeader() { delete [] bytes;	};
};


template<class R> class FileEntity {
	protected:
		string folder;
		string dataName;
		string metaName;

	public:
		FileEntity<R>(FileEntity<R> & fileEntity) : folder(fileEntity.folder), dataName(fileEntity.dataName),  metaName(fileEntity.metaName) {};

		FileEntity<R>(const char* ifolder, const char* irecordName, const char* dataSuffix, const char* metaSuffix) : folder(ifolder) , dataName(folder + "/" + irecordName + "." + dataSuffix) , metaName(folder + "/" + irecordName + "." + metaSuffix) {
			ifstream inFile(metaName);
			if(!inFile) {
				inFile.close();
				ofstream outFile(metaName, ios::out | ios::app | ios::binary);
				outFile << 0;
			}
			fstream metaFile(metaName, ios::out | ios::app);
			metaFile.close();
		};

		entity_size count(R record) {
			int total;
			fstream metaFile(metaName, ios::in | ios::out | ios::binary);
			metaFile >> total;
			return total;
		};
	protected:
		void updateMetafile() {
			int total;
			fstream metaFile(metaName, ios::in | ios::out | ios::binary);
			metaFile >> total;
			metaFile.seekg(ios::beg);
			metaFile << ++total;

		}
};


template<class R, class T> class VarFileEntity : public FileEntity<R> {
	public:
	static const unsigned short RECORD_LENGTH = 4 + (2 * R::indent);
	static const unsigned short COLUMN_NUMBER = R::indent;
	public:
		ifstream idataFile;
		SerialHeader serialHeader;
		SerialRecord serialRecord;
	public:
		VarFileEntity<R, T>(VarFileEntity<R, T> & ivarFileEntity) : FileEntity<R>(ivarFileEntity), serialHeader(COLUMN_NUMBER), serialRecord(COLUMN_NUMBER) {};
		VarFileEntity<R, T>(const char* ifolder, const char* irecordName) : FileEntity<R>(ifolder, irecordName, "dat", "met"), serialHeader(COLUMN_NUMBER), serialRecord(COLUMN_NUMBER) {};

		void insert(R record) {
			ofstream odataFile(FileEntity<R>::dataName, ios::out | ios::app | ios::binary);
			record.measure(serialRecord);
			serialHeader.calculate(serialRecord.boundaries);
			serialHeader.write(odataFile);
			serialRecord.resize(serialHeader.getSize());
			record.serialize(serialRecord);
			odataFile.write(serialRecord.getBytes(), serialHeader.getSize() - 1) << endl;
			odataFile.close();
			FileEntity<R>::updateMetafile();
		};

		void open() {
			idataFile.open(FileEntity<R>::dataName, ios::in | ios::binary);
		};

		void close() {
			idataFile.close();
		};

		int fetch(R &record) {
			if(fetch(serialRecord)) {
				record.deserialize(serialRecord);
				return 1;
			} else{
				return 0;
			}
		};

		int fetch(SerialRecord & iserialRecord) {
			if(serialHeader.read(idataFile)) {
				iserialRecord.resize(serialHeader.getSize());
				serialHeader.parse(iserialRecord.boundaries);
				idataFile.read(iserialRecord.getBytes(), serialHeader.getSize());
				return 1;
			} else{
				return 0;
			}
		};

		int fetch(SerialRecord & serialRecord, Criterium & criterium, unsigned short position, const char* bytes, unsigned int length) {
			unsigned int lstart, llength;
			while(serialHeader.read(idataFile)) {
				serialRecord.resize(serialHeader.getSize());
				idataFile.read(serialRecord.getBytes(), serialHeader.getSize());
				serialHeader.parse(position, lstart, llength);
				if(criterium.valid(&(serialRecord.getBytes()[lstart]), llength, bytes, length)) {
					serialHeader.parse(serialRecord.boundaries);
					return 1;
				}
			}
			return 0;
		};

		void retrieve(T &records) {
			open();
			R* precord = new R();
			while(fetch(*precord)) {
				records.push_back(precord);
				precord = new R();
			}
			delete precord;
			close();
		};
};


/*template<class R, template<class, class> class C> class FixFileEntity : public FileEntity<R, C> {
	public:
		FixFileEntity<R, C>(const char* ifolder, const char* irecordName) : FileEntity<R, C>(ifolder, irecordName, "fdat", "fmet") {};

		void findAll(C<R*, allocator<R*>> &records) {
			ifstream dataFile(FileEntity<R, C>::dataName, ios::in | ios::binary);
			SerialIRecord serialI(R::size + 1);
			//Resize container
			while(dataFile.read(serialI.getBytes(), R::size + 1)) {
				R *precord = new R();
				precord->read(serialI);
				records.push_back(precord);
				serialI.reset();
			}
		};

		void insert(R record) {
			ofstream dataFile(FileEntity<R, C>::dataName, ios::out | ios::app | ios::binary);
			SerialORecord serialO(R::size);
			record.write(serialO);
			dataFile << serialO.getBytes();
			dataFile << endl;
			dataFile.close();
			FileEntity<R, C>::updateMetafile();
		};
};*/

#endif /* LIBOBASE_FILE_HPP_ */
