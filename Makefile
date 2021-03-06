cc = g++-10
#googletest = /home/francesc/Projectes/googletest

all: clean libobase kanji file_entity_test

libobase: debug/libobasecriteria.o debug/libobasestreamer.o debug/libobasefile.o

file_entity_test: tests/bin/file_entity_test

kanji: tests/bin/kanji

debug/libobasefile.o: libobase/model.hpp libobase/file.hpp  libobase/streamer.hpp libobase/criteria.hpp
	$(cc) -c libobase/file.cpp -I. -o $@ -g

debug/libobasestreamer.o: libobase/model.hpp libobase/streamer.hpp libobase/criteria.hpp libobase/streamer.cpp
	$(cc) -c libobase/streamer.cpp -I. -o $@ -g

debug/libobasecriteria.o: libobase/model.hpp libobase/streamer.hpp libobase/criteria.hpp libobase/criteria.cpp
	$(cc) -c libobase/criteria.cpp -I. -o $@ -g

tests/bin/file_entity_test.o: tests/file_entity_test.cpp libobase/model.hpp  libobase/streamer.hpp libobase/file.hpp libobase/criteria.hpp
	$(cc) -c tests/file_entity_test.cpp -I. -o tests/bin/file_entity_test.o -g

tests/bin/kanji.o: libobase/model.hpp libobase/streamer.hpp libobase/criteria.hpp tests/kanji.hpp libobase/criteria.cpp
	$(cc) -c tests/kanji.cpp -I. -o $@ -g

tests/bin/kanji: debug/libobasefile.o debug/libobasecriteria.o debug/libobasestreamer.o tests/bin/kanji.o 
	$(cc) -o $@ $? -g

tests/bin/file_entity_test: debug/libobasefile.o debug/libobasecriteria.o debug/libobasestreamer.o tests/bin/file_entity_test.o
	$(cc) -o $@ $? -lgtest -lgtest_main -g -pthread

.PHONNY: clean
clean:
	rm -r debug/* || :
	rm -r tests/bin/* || :
	