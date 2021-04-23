#ifndef LIBOBASE_MODEL_HPP_
#define LIBOBASE_MODEL_HPP_

#include <string>
#include <iostream>
#include <vector>


#include <libobase/streamer.hpp>
#include <libobase/criteria.hpp>

using namespace std;

struct LEFT_RECORD { static const unsigned short position = 0; };
struct RIGHT_RECORD { static const unsigned short position = 1; };

template<unsigned short P> class Column {
	public:
		static const unsigned short position = P;
	protected:
		bool empty = true;
	public:
		bool isNull() { return empty; };
};

//Class VarUTF-8 column
template<unsigned short P> class VarUTF8 : public Column<P> {
	public:
		static const column_max size = 2;
	private:
		string value;
	public:
		inline void serialize(SerialRecord & serialRecord) { serialRecord.write(value);	};
		inline void deserialize(SerialRecord & serialRecord) { serialRecord.readString(value, P); };
		inline column_max length() { return static_cast<column_max>(value.length()); };
		inline void setValue(string & ivalue) { value = ivalue; };
		inline const string& getValue() { return value; };
		inline string serialize() {	return value; };
		VarUTF8() {};
		VarUTF8(string content) : value(content) {};
		VarUTF8(const char* content) : value(content) {};
};

template<unsigned short P, unsigned short S> class BoundUTF8 : public Column<P>  {
	public:
		static const column_max size = S;
	private:
		string value;
	public:
		void serialize(SerialRecord & serialRecord) { serialRecord.write(value); };
		void deserialize(SerialRecord & serialRecord) { serialRecord.readString(value, P); };
		inline unsigned short length() { return S; };
		inline const string& getValue() { return value; };
		inline const void setValue(string & ivalue) {
			value = ivalue.substr(0, S);
			if(S > value.length()) value.append(S - value.length(), 0);
		}
		inline string serialize() {	return value; };
		BoundUTF8() {
			value.reserve(S);
		};
		BoundUTF8(string &content) : BoundUTF8() {
			Column<P>::empty = false;
			setValue(content);
		};
		BoundUTF8(const char* content) : BoundUTF8() {
			string ivalue = content;
			setValue(ivalue);
		};
};

template<unsigned short P> class UnsignedShort : public Column<P>  {
	private:
		unsigned short value;
	public:
		static const column_max size = 2;
	public:
		inline void serialize(SerialRecord & serialRecord) { serialRecord.write(value); };
		inline void deserialize(SerialRecord & serialRecord) { value = serialRecord.readShort(); };
		inline unsigned short length() { return size; };
		inline const unsigned int getValue() { return value; };
		inline string serialize() {
			char bytes[2];
			SerialRecord::writeUShort(bytes, value);
			return string(bytes, 2);
		};
		UnsignedShort() {};
		UnsignedShort(unsigned short number) : value(number) {};
		UnsignedShort& operator=(const unsigned short number) {
			value = number;
			return *this;
		};
};

template<unsigned short P> class UnsignedInt : public Column<P>  {
	private:
		unsigned int value;
	public:
		static const column_max size = 4;
	public:
		inline void serialize(SerialRecord & serialRecord) { serialRecord.write(value); };
		inline void deserialize(SerialRecord & serialRecord) { value = serialRecord.readInt(); };
		inline unsigned short length() { return size; };
		inline const unsigned short getValue() { return value; };
		inline string serialize() {
			char bytes[4];
			SerialRecord::writeUInt(bytes, value);
			return string(bytes, 4);
		};
		UnsignedInt() {};
		UnsignedInt(unsigned int number) : value(number) {};
		UnsignedInt& operator=(const unsigned int number) {
			value = number;
			return *this;
		};
};

template <class... V> class Record {
	public:
		static const record_max size = 0;
		static const unsigned short int indent = 0;
	public:
		inline void write(SerialRecord & serialRecord) {};
		inline void read(SerialRecord & serialRecord) {};
		inline void calculate(SerialRecord & serialRecord) {};
		inline record_max getSize() { return 0; };
};

template <class F, class... V> class Record<F, V...> {
	public:
		F column;
		Record<V...> subRecord;
		static const record_max size = F::size + Record<V...>::size;
		static const unsigned short indent = 1 + Record<V...>::indent;

	public:
		inline Record<V ...>& next() { return subRecord; }

		inline void serialize(SerialRecord & serialRecord) {
			serialRecord.start();
			write(serialRecord);
		};

		inline void deserialize(SerialRecord & serialRecord) {
			serialRecord.start();
			read(serialRecord);
		};

		inline void write(SerialRecord & serialRecord) {
			column.serialize(serialRecord);
			subRecord.write(serialRecord);
		};

		inline void read(SerialRecord & serialRecord) {
			column.deserialize(serialRecord);
			subRecord.read(serialRecord);
		};

		inline void calculate(SerialRecord & serialRecord) {
			serialRecord.boundaries.push_back((F::position == 1 ? column.length() : column.length() + serialRecord.boundaries.back()));
			subRecord.calculate(serialRecord);
		};

		inline void measure(SerialRecord & serialRecord) {
			serialRecord.boundaries.clear();
			calculate(serialRecord);
		};

		inline record_max getSize() { return column.getSize() + subRecord.getSize(); };

		Record<F, V ...>() {};
		Record<F, V ...>(F icolumn, V... irest) : column(icolumn), subRecord(irest...) {};
};

template <class M, class C> class Filter {
	private:
		C criterium;
		string value;
		unsigned int valLength;
	public:
		static const unsigned short position = M::position;
	public:
		inline void setColumn(M & column) {
			value = column.serialize();
		};
		inline const char* getBytes() { return value.data(); };
		inline unsigned int  length() { return value.length(); };
		Filter<M, C>() {};
		Filter<M, C>(M & column) : Filter<M, C>() { setColumn(column); };
		inline C& getCriterium() { return criterium; };
};

template<class R, class E, class F> class FilteredEntity {
	private:
		E entity;
		F filter;
	public:
		FilteredEntity<R, E, F>(E &ientity) : entity(ientity) {};
		inline F & getFilter() { return filter; };
		void open() { entity.open(); };
		void close() { entity.close(); };

		int fetch(R &record) {
			SerialRecord serialRecord(R::indent);
			if(entity.fetch(serialRecord, filter.getCriterium(), F::position, filter.getBytes(), filter.length())) {
				record.deserialize(serialRecord);
				return 1;
			}
			return 0;
		};

		int fetch(SerialRecord & serialRecord, Criterium & icriterium, unsigned short iposition, const char* ibytes, unsigned int ilength) {
			while(entity.fetch(serialRecord, filter.getCriterium(), F::position, filter.getBytes(), filter.length())) {
				unsigned int start, length;
				serialRecord.parse(iposition, start, length);
				if(icriterium.valid(&(serialRecord.getBytes()[start]), length, ibytes, ilength)) {
					return 1;
				}
			}
			return 0;
		};
};

template <class... V> class RecordMapper {
	public:
		inline void map(SerialRecord & target, SerialRecord & leftRecord, SerialRecord & rightRecord) {};
		inline record_max measure(SerialRecord & leftRecord, SerialRecord & rightRecord) { return 0; }
		RecordMapper<V ...>() {};
};

template <class P, class C, class... V> class RecordMapper<P, C, V...> {
	public:
		RecordMapper<V...> subRecordMapper;

	private:
		inline void map(SerialRecord & target, SerialRecord & source) {
			record_max s_position = C::position == 1 ? 0 : source.boundaries[C::position - 2];
			record_max s_length = source.boundaries[C::position - 1] - s_position;
			record_max t_previous = (target.boundaries.empty() ? 0 : target.boundaries.back());
			target.boundaries.push_back(t_previous + s_length);
			target.write(&source.getBytes()[s_position], s_length);
		}

		inline record_max measure(SerialRecord & source) {
			return C::position == 1 ? source.getBoundaries()[C::position - 1] : source.getBoundaries()[C::position - 1] - source.getBoundaries()[C::position - 2];
		};

	public:
		inline void map(SerialRecord & target, SerialRecord & leftRecord, SerialRecord & rightRecord) {
			map(target, (P::position == LEFT_RECORD::position ? leftRecord : rightRecord));
			subRecordMapper.map(target, leftRecord, rightRecord);
		};


		inline record_max measure(SerialRecord & leftRecord, SerialRecord & rightRecord) {
			return measure((P::position == LEFT_RECORD::position ? leftRecord : rightRecord)) + subRecordMapper.measure(leftRecord, rightRecord);
		};

		inline void build(SerialRecord & target, SerialRecord & leftRecord, SerialRecord & rightRecord) {
			record_max length = measure(leftRecord, rightRecord);
			target.resize(length + 1);
			map(target, leftRecord, rightRecord);
		};
};


template <class LC, class RC, class C> class Join {
	private:
		C criterium;
	public:
		static const unsigned short leftPosition = LC::position;
		static const unsigned short rightPosition = RC::position;
	public:
		inline C& getCriterium() { return criterium; };
		bool valid(char* leftBytes, unsigned int leftLength, char* rightBytes, unsigned int rightLength) {
			return criterium.valid();
		}
};

template <class R, class LE, class RE, class M, class J> class JoinedEntity {
	private:
			J join;
			LE leftEntity;
			RE rightEntity;
			M recordMapper;
			SerialRecord leftRecord;
			SerialRecord rightRecord;
			bool isLeft = false;
			unsigned int leftStart;
			unsigned int leftLength;

	public:
			void open() {
				isLeft = false;
				leftEntity.open();
			};

			void close() {
				leftEntity.close();
				rightEntity.close();
			};

			int fetch(R &record) {
				SerialRecord serialRecord(R::indent);
				if(fetch(serialRecord)) {
					record.deserialize(serialRecord);
					return 1;
				}
				return 0;
			};

			int fetch(SerialRecord & serialRecord) {
				while(isLeft || leftEntity.fetch(leftRecord)) {
					if(!isLeft) {
						leftRecord.parse(J::leftPosition, leftStart, leftLength);
						rightEntity.open();
					}
					if(rightEntity.fetch(rightRecord, join.getCriterium(), J::rightPosition, &(leftRecord.getBytes()[leftStart]), leftLength)) {
						isLeft = true;
						recordMapper.build(serialRecord, leftRecord, rightRecord);
						return 1;
					} else {
						isLeft = false;
						rightEntity.close();
					}
				}
				return 0;
			};

			int fetch(SerialRecord & serialRecord, Criterium & icriterium, unsigned short iposition, const char* ibytes, unsigned int ilength) {
				while(leftEntity.fetch(serialRecord)) {
					unsigned int start, length;
					serialRecord.parse(iposition, start, length);
					if(icriterium.valid(&(serialRecord.getBytes()[start]), length, ibytes, ilength)) {
						return 1;
					}
				}
				return 0;
			};

			JoinedEntity<R, LE, RE, M, J>(LE & ileftEntity, RE & irightEntity) : leftEntity(ileftEntity), rightEntity(irightEntity), leftRecord(LE::COLUMN_NUMBER), rightRecord(RE::COLUMN_NUMBER) {};
};

#endif /* LIBOBASE_MODEL_HPP_ */
