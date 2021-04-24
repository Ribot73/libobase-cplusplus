#include <libobase/model.hpp>
#include <libobase/file.hpp>

#include <tests/kanji.hpp>
#include <gtest/gtest.h>

typedef class FilteredEntity<kanji::Kanji, kanji::KanjiEntity, Filter<kanji::KunYomi, StringEquals>> FilteredKunYomi;
typedef class FilteredEntity<kanji::Kanji, kanji::KanjiEntity, Filter<kanji::Description, StringEquals>> FilteredDesc;
typedef class FilteredEntity<kanji::Kanji, FilteredKunYomi, Filter<kanji::Description, StringEquals>> FilteredKunYomiDesc;
typedef class FilteredEntity<kanji::Kanji, FilteredDesc, Filter<kanji::KunYomi, StringEquals>> FilteredDescKunYomi;
typedef class JoinedEntity <kanjiSimple::KanjiSimple, kanji::KanjiEntity, simpleWord::SimpleWordEntity, kanjiSimple::KanjiSimpleMapper, Join<kanji::Id, simpleWord::KanjiId, StringEquals>> JoinedKanjiSimple;
typedef class FilteredEntity<kanjiSimple::KanjiSimple, JoinedKanjiSimple, Filter<kanjiSimple::KanjiId, StringEquals>> FilteredKanjiSimple;
typedef class FilteredEntity<kanji::Kanji, kanji::KanjiEntity, Filter<kanji::Id, StringEquals>> FilteredId;
typedef class JoinedEntity <kanjiSimple::KanjiSimple, FilteredId, simpleWord::SimpleWordEntity, kanjiSimple::KanjiSimpleMapper, Join<kanji::Id, simpleWord::KanjiId, StringEquals>> JoinedFilteredKanjiSimple;

TEST(FileEntity, Single_Filtered_KunYomi)
{
	kanji::Kanji kanji;
	kanji::KanjiEntity kanjiEntity(DATA_FOLDER, "kanji");

	kanji::KunYomi kunYomi("?");
	FilteredKunYomi filteredKunYomi(kanjiEntity);
	filteredKunYomi.getFilter().setColumn(kunYomi);
	filteredKunYomi.open();
	ASSERT_NE(filteredKunYomi.fetch(kanji), 0);
	ASSERT_EQ(kanji.column.getValue(), 1);
	ASSERT_EQ(kanji.next().column.getValue(), kanji::Ideogram("突").getValue());
	filteredKunYomi.close();
}

TEST(FileEntity, Single_Filtered_Description)
{
	kanji::Kanji kanji;
	kanji::KanjiEntity kanjiEntity(DATA_FOLDER, "kanji");

	kanji::Description description("Res...");
	FilteredDesc filteredDesc(kanjiEntity);
	filteredDesc.getFilter().setColumn(description);
	filteredDesc.open();
	ASSERT_NE(filteredDesc.fetch(kanji), 0);
	ASSERT_EQ(kanji.column.getValue(), 3);
	ASSERT_EQ(kanji.next().column.getValue(), kanji::Ideogram("通").getValue());
	filteredDesc.close();
}


TEST(FileEntity, KunYomiDesc_Filtered)
{
	kanji::Kanji kanji;
	kanji::KanjiEntity kanjiEntity(DATA_FOLDER, "kanji");

	kanji::KunYomi kunYomi("?");
	kanji::Description description("Res...");

	FilteredKunYomi filteredKunYomi(kanjiEntity);
	filteredKunYomi.getFilter().setColumn(kunYomi);
	FilteredKunYomiDesc filteredKunYomiDesc(filteredKunYomi);
	filteredKunYomiDesc.getFilter().setColumn(description);
	filteredKunYomiDesc.open();
	ASSERT_NE(filteredKunYomiDesc.fetch(kanji), 0);
	ASSERT_EQ(kanji.next().column.getValue(), kanji::Ideogram("撃").getValue());
	filteredKunYomiDesc.close();
}

TEST(FileEntity, DescKunYomi_Filtered)
{
	kanji::Kanji kanji;
	kanji::KanjiEntity kanjiEntity(DATA_FOLDER, "kanji");

	kanji::Description description("Res...");
	kanji::KunYomi kunYomi("?");

	FilteredDesc filteredDesc(kanjiEntity);
	filteredDesc.getFilter().setColumn(description);
	FilteredDescKunYomi filteredDescKunYomi(filteredDesc);
	filteredDescKunYomi.getFilter().setColumn(kunYomi);
	filteredDescKunYomi.open();
	ASSERT_NE(filteredDescKunYomi.fetch(kanji), 0);
	ASSERT_EQ(kanji.next().column.getValue(), kanji::Ideogram("撃").getValue());
	filteredDescKunYomi.close();
}

TEST(FileEntity, Joined_Kanji_SimpleWord)
{
	kanjiSimple::KanjiSimple kanjiSimple;
	kanji::KanjiEntity kanjiEntity(DATA_FOLDER, "kanji");
	simpleWord::SimpleWordEntity simpleWordEntity(DATA_FOLDER, "simpleWord");
	JoinedKanjiSimple joinedKanjiSimple(kanjiEntity, simpleWordEntity);

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

TEST(FileEntity, Filtered_Joined_Kanji_SimpleWord)
{
	kanjiSimple::KanjiSimple kanjiSimple;
	kanji::KanjiEntity kanjiEntity(DATA_FOLDER, "kanji");
	simpleWord::SimpleWordEntity simpleWordEntity(DATA_FOLDER, "simpleWord");
	JoinedKanjiSimple joinedKanjiSimple(kanjiEntity, simpleWordEntity);

	kanjiSimple::KanjiId kanjiId(3);
	FilteredKanjiSimple filteredKanjiSimple(joinedKanjiSimple);
	filteredKanjiSimple.getFilter().setColumn(kanjiId);

	filteredKanjiSimple.open();

	ASSERT_NE(filteredKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 1);

	ASSERT_NE(filteredKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 4);
	ASSERT_EQ(kanjiSimple.next().next().column.getValue(), "とおす");
	ASSERT_EQ(kanjiSimple.next().next().next().column.getValue(), "Kun");
	ASSERT_EQ(kanjiSimple.next().next().next().next().column.getValue(), "Fer passar");
	ASSERT_EQ(kanjiSimple.next().next().next().next().next().column.getValue(), "Transitiu");


	ASSERT_NE(filteredKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 5);

	ASSERT_NE(filteredKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 7);

	ASSERT_EQ(filteredKanjiSimple.fetch(kanjiSimple), 0);

	filteredKanjiSimple.close();
}

TEST(FileEntity, Joined_Filtered_Kanji_SimpleWord)
{
	kanjiSimple::KanjiSimple kanjiSimple;
	kanji::KanjiEntity kanjiEntity(DATA_FOLDER, "kanji");
	kanji::Id id(3);
	FilteredId filteredId(kanjiEntity);
	filteredId.getFilter().setColumn(id);

	simpleWord::SimpleWordEntity simpleWordEntity(DATA_FOLDER, "simpleWord");
	JoinedFilteredKanjiSimple joinedFilteredKanjiSimple(filteredId, simpleWordEntity);

	joinedFilteredKanjiSimple.open();

	ASSERT_NE(joinedFilteredKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 1);

	ASSERT_NE(joinedFilteredKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 4);
	ASSERT_EQ(kanjiSimple.next().next().column.getValue(), "とおす");
	ASSERT_EQ(kanjiSimple.next().next().next().column.getValue(), "Kun");
	ASSERT_EQ(kanjiSimple.next().next().next().next().column.getValue(), "Fer passar");
	ASSERT_EQ(kanjiSimple.next().next().next().next().next().column.getValue(), "Transitiu");


	ASSERT_NE(joinedFilteredKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 5);

	ASSERT_NE(joinedFilteredKanjiSimple.fetch(kanjiSimple), 0);
	ASSERT_EQ(kanjiSimple.column.getValue(), 3);
	ASSERT_EQ(kanjiSimple.next().column.getValue(), 7);

	ASSERT_EQ(joinedFilteredKanjiSimple.fetch(kanjiSimple), 0);

	joinedFilteredKanjiSimple.close();
}
