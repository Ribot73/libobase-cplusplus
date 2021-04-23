#include <libobase/model.hpp>
#include <libobase/file.hpp>

#include <tests/kanji.hpp>
#include <gtest/gtest.h>

using namespace kanji;

typedef class FilteredEntity<Kanji, KanjiEntity, Filter<KunYomi, StringEquals>> FilteredKunYomi;
typedef class FilteredEntity<Kanji, KanjiEntity, Filter<Description, StringEquals>> FilteredDesc;
typedef class FilteredEntity<Kanji, FilteredKunYomi, Filter<Description, StringEquals>> FilteredKunYomiDesc;
typedef class FilteredEntity<Kanji, FilteredDesc, Filter<KunYomi, StringEquals>> FilteredDescKunYomi;

TEST(FileEntity, Single_Filtered_KunYomi)
{
	Kanji kanji;
	KanjiEntity kanjiEntity("/home/francesc/Projectes/libobase/tests/data", "kanji");

	kanji::KunYomi kunYomi("?");
	FilteredKunYomi filteredKunYomi(kanjiEntity);
	filteredKunYomi.getFilter().setColumn(kunYomi);
	filteredKunYomi.open();
	ASSERT_NE(filteredKunYomi.fetch(kanji), 0);
	ASSERT_EQ(kanji.column.getValue(), 1);
	ASSERT_EQ(kanji.next().column.getValue(), Ideogram("突").getValue());
	filteredKunYomi.close();
}

TEST(FileEntity, Single_Filtered_Description)
{
	kanji::Kanji kanji;
	kanji::KanjiEntity kanjiEntity("/home/francesc/Projectes/libobase/tests/data", "kanji");

	kanji::Description description("Res...");
	FilteredDesc filteredDesc(kanjiEntity);
	filteredDesc.getFilter().setColumn(description);
	filteredDesc.open();
	ASSERT_NE(filteredDesc.fetch(kanji), 0);
	ASSERT_EQ(kanji.column.getValue(), 3);
	ASSERT_EQ(kanji.next().column.getValue(), Ideogram("通").getValue());
	filteredDesc.close();
}


TEST(FileEntity, KunYomiDesc_Filtered)
{
	kanji::Kanji kanji;
	kanji::KanjiEntity kanjiEntity("/home/francesc/Projectes/libobase/tests/data", "kanji");

	kanji::KunYomi kunYomi("?");
	kanji::Description description("Res...");

	FilteredKunYomi filteredKunYomi(kanjiEntity);
	filteredKunYomi.getFilter().setColumn(kunYomi);
	FilteredKunYomiDesc filteredKunYomiDesc(filteredKunYomi);
	filteredKunYomiDesc.getFilter().setColumn(description);
	filteredKunYomiDesc.open();
	ASSERT_NE(filteredKunYomiDesc.fetch(kanji), 0);
	ASSERT_EQ(kanji.next().column.getValue(), Ideogram("撃").getValue());
	filteredKunYomiDesc.close();
}

TEST(FileEntity, DescKunYomi_Filtered)
{
	kanji::Kanji kanji;
	kanji::KanjiEntity kanjiEntity("/home/francesc/Projectes/libobase/tests/data", "kanji");

	kanji::Description description("Res...");
	kanji::KunYomi kunYomi("?");

	FilteredDesc filteredDesc(kanjiEntity);
	filteredDesc.getFilter().setColumn(description);
	FilteredDescKunYomi filteredDescKunYomi(filteredDesc);
	filteredDescKunYomi.getFilter().setColumn(kunYomi);
	filteredDescKunYomi.open();
	ASSERT_NE(filteredDescKunYomi.fetch(kanji), 0);
	ASSERT_EQ(kanji.next().column.getValue(), Ideogram("撃").getValue());
	filteredDescKunYomi.close();
}

TEST(FileEntity, Joined_Kanji_SimpleWord)
{
	kanjiSimple::KanjiSimple kanjiSimple;
	KanjiEntity kanjiEntity("/home/francesc/Projectes/libobase/tests/data", "kanji");
	simpleWord::SimpleWordEntity simpleWordEntity("/home/francesc/Projectes/libobase/tests/data", "simpleWord");
	kanjiSimple::JoinedKanjiSimple joinedKanjiSimple(kanjiEntity, simpleWordEntity);

	joinedKanjiSimple.open();

	ASSERT_NE(joinedKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 1);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 2);

	ASSERT_NE(joinedKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 1);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 6);

	ASSERT_NE(joinedKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 1);

	ASSERT_NE(joinedKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 4);
	ASSERT_EQ(kanjiSimple.next().next().column.getValue(), "とおす");
	ASSERT_EQ(kanjiSimple.next().next().next().column.getValue(), "Kun");
	ASSERT_EQ(kanjiSimple.next().next().next().next().column.getValue(), "Fer passar");
	ASSERT_EQ(kanjiSimple.next().next().next().next().next().column.getValue(), "Transitiu");


	ASSERT_NE(joinedKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 5);

	ASSERT_NE(joinedKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 7);

	ASSERT_NE(joinedKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 4);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 3);

	ASSERT_EQ(joinedKanjiSimple.fetch(kanjiSimple), 0);

	joinedKanjiSimple.close();
}


