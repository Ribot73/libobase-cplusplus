#include <iostream>
#include <string>
#include <vector>

#include <libobase/model.hpp>
#include <libobase/criteria.hpp>
#include <tests/kanji.hpp>


int main() {
	kanji::KanjiEntity kanjiEntity("/home/francesc/Projectes/libobase/tests/data", "kanji");
//	kanjiEntity.insert(kanji::Kanji(1, "突", "?", "?", "Clavar (etzibar)", "Punxar"));
//	kanjiEntity.insert(kanji::Kanji(2, "牡", "?", "?", "Mascle", ""));
//	kanjiEntity.insert(kanji::Kanji(3, "通", "utsu", "", "Passar (moviment)", "Res..."));
//	kanjiEntity.insert(kanji::Kanji(4, "牛", "GYŪ, ushi", "Gyu", "Bou, vaca, bòvid", "Sense sexe assignat"));
//	kanjiEntity.insert(kanji::Kanji(5, "撃", "?", "?", "Colpejar", "Res..."));

	simpleWord::SimpleWordEntity simpleWordEntity("/home/francesc/Projectes/libobase/tests/data", "simpleWord");
//	simpleWordEntity.insert(simpleWord::SimpleWord(1, "る", "とおる", "Kun", 3, "Travessar, passar", "Intransitiu"));
//	simpleWordEntity.insert(simpleWord::SimpleWord(2, "く", "つく", "Kun", 1, "Clavar", ""));
//	simpleWordEntity.insert(simpleWord::SimpleWord(3, "牛", "うし", "Kun", 4, "Bou", ""));
//	simpleWordEntity.insert(simpleWord::SimpleWord(4, "す", "とおす", "Kun", 3, "Fer passar", "Transitiu"));
//	simpleWordEntity.insert(simpleWord::SimpleWord(5, "り", "とおり", "Kun", 3, "Avinguda, carrer", ""));
//	simpleWordEntity.insert(simpleWord::SimpleWord(6, "く", "つつく", "Kun", 1, "Burxar", "Transitiu"));
//	simpleWordEntity.insert(simpleWord::SimpleWord(7, "う", "かよう", "Kun", 3, "Anar amunt i avall, circular (fluid)", "Intransitiu"));

	vector<kanji::Kanji*> allKanji;
	kanjiEntity.retrieve(allKanji);

	cout << "Indent: " << kanji::Kanji::indent << endl;
	cout << "Tamany: " << allKanji.size() << endl;

	cout << "allKanji.at(0)->next().indent: " << allKanji.at(0)->next().indent << endl;
	cout << "allKanji.at(0)->value.getValue(): " << allKanji.at(0)->value.getValue() << endl;
	cout << "allKanji.at(0)->next().value.getValue(): " << allKanji.at(0)->next().value.getValue() << endl;
	cout << "allKanji.at(0)->next().next().value.getValue(): " << allKanji.at(0)->next().next().value.getValue() << endl;
	cout << "allKanji.at(0)->next().next().next().value.getValue(): " << allKanji.at(0)->next().next().next().value.getValue() << endl;
	cout << "allKanji.at(0)->next().next().next().next().value.getValue(): " << allKanji.at(0)->next().next().next().next().value.getValue() << endl;
	cout << "allKanji.at(0)->next().next().next().next().next().value.getValue(): " << allKanji.at(0)->next().next().next().next().next().value.getValue() << endl;

	cout << "allKanji.at(2)->value.getValue(): " << allKanji.at(2)->value.getValue() << endl;
	cout << "allKanji.at(2)->next().value.getValue(): " << allKanji.at(2)->next().value.getValue() << endl;
	cout << "allKanji.at(2)->next().next().next().value.getValue(): " << allKanji.at(2)->next().next().next().next().value.getValue() << endl;

	vector<simpleWord::SimpleWord *> allSimpleWord;
	simpleWordEntity.retrieve(allSimpleWord);
	cout << "Indent: " << simpleWord::SimpleWord::indent << endl;
	cout << "Tamany: " << allSimpleWord.size() << endl;

	cout << "allSimpleWord.at(3)->value.getValue(): " << allSimpleWord.at(3)->value.getValue() << endl;
	cout << "allSimpleWord.at(3)->next().value.getValue(): " << allSimpleWord.at(3)->next().value.getValue() << endl;
	cout << "allSimpleWord.at(3)->next().next().next().value.getValue(): " << allSimpleWord.at(3)->next().next().next().next().value.getValue() << endl;
}
