#ifndef TESTS_KANJI_HPP_
#define TESTS_KANJI_HPP_

#include <libobase/model.hpp>
#include <libobase/file.hpp>

static const char* DATA_FOLDER = "/home/francesc/Projects/libobase-cplusplus/tests/data";

namespace kanji {
	typedef class UnsignedInt<1> Id;
	typedef class BoundUTF8<2, 4> Ideogram;
	typedef class VarUTF8<3> KunYomi;
	typedef class VarUTF8<4> OnYomi;
	typedef class VarUTF8<5> Meaning;
	typedef class VarUTF8<6> Description;
	typedef class Record<Id, Ideogram, KunYomi, OnYomi, Meaning, Description> Kanji;

	typedef class VarFileEntity<Kanji, vector<Kanji*>> KanjiEntity;
};

namespace simpleWord {
	typedef class UnsignedInt<1> Id;
	typedef class BoundUTF8<2, 32> Flexed;
	typedef class VarUTF8<3> Hiragana;
	typedef class BoundUTF8<4, 3> Yomi;
	typedef class UnsignedInt<5> KanjiId;
	typedef class VarUTF8<6> Translation;
	typedef class VarUTF8<7> Notes;
	typedef class Record<Id, Flexed, Hiragana, Yomi, KanjiId, Translation, Notes> SimpleWord;

	typedef class VarFileEntity<SimpleWord, vector<SimpleWord*>> SimpleWordEntity;
};

namespace compoundWord {
	typedef class UnsignedInt<1> Id;
	typedef class VarUTF8<2> Hiragana;
	typedef class BoundUTF8<3, 3> Yomi;
	typedef class UnsignedInt<4> KanjiId;
	typedef class VarUTF8<5> Translation;
	typedef class VarUTF8<6> Notes;
	typedef class Record<Id, Yomi, Hiragana, Translation, Notes> CompoundWord;
};

namespace sentence {
	typedef class UnsignedInt<1> Id;
	typedef class BoundUTF8<2, 3> Yomi;
	typedef class UnsignedInt<3> SimpleWordId;
	typedef class UnsignedInt<4> CoumpoundWordId;
	typedef class VarUTF8<5> Hiragana;
	typedef class VarUTF8<6> Translation;
	typedef class VarUTF8<7> Notes;
	typedef class Record<Id, Yomi, SimpleWordId, CoumpoundWordId, Hiragana, Translation, Notes> Sentence;
};

namespace kanjiSimple {
	typedef class UnsignedInt<1> KanjiId;
	typedef class UnsignedInt<2> SimpleId;
	typedef class VarUTF8<3> Hiragana;
	typedef class BoundUTF8<4, 3> Yomi;
	typedef class VarUTF8<5> Translation;
	typedef class VarUTF8<6> Notes;
	typedef class Record<SimpleId, KanjiId, Hiragana, Yomi, Translation, Notes> KanjiSimple;
	typedef class RecordMapper<LEFT_RECORD, kanji::Id, RIGHT_RECORD, simpleWord::Id, RIGHT_RECORD, simpleWord::Hiragana, RIGHT_RECORD, simpleWord::Yomi, RIGHT_RECORD, simpleWord::Translation, RIGHT_RECORD, simpleWord::Notes> KanjiSimpleMapper;
}

#endif /* TESTS_KANJI_HPP_ */
