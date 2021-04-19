cc = g++-10
googletest = /home/francesc/Projectes/googletest

all: file_entity_test kanji libobase

libobase: debug/libobasestreamer.o debug/libobasefile.o debug/libobasecriteria.o

kanji: tests/bin/kanji

file_entity_test: tests/bin/file_entity_test

debug/libobasefile.o: libobase/model.hpp libobase/file.hpp  libobase/streamer.hpp libobase/criteria.hpp
	$(cc) -c libobase/file.cpp -I. -o $@

debug/libobasestreamer.o: libobase/model.hpp libobase/streamer.hpp libobase/criteria.hpp libobase/streamer.cpp
	$(cc) -c libobase/streamer.cpp -I. -o $@

debug/libobasecriteria.o: libobase/model.hpp libobase/streamer.hpp libobase/criteria.hpp libobase/criteria.cpp
	$(cc) -c libobase/criteria.cpp -I. -o $@

tests/bin/kanji.o: libobase/model.hpp libobase/streamer.hpp libobase/criteria.hpp tests/kanji.hpp libobase/criteria.cpp
	$(cc) -c tests/kanji.cpp -I. -o $@

tests/bin/kanji: debug/libobasefile.o debug/libobasecriteria.o debug/libobasestreamer.o tests/bin/kanji.o 
	$(cc) -o $@ $?

tests/bin/file_entity_test.o: tests/file_entity_test.cpp libobase/model.hpp  libobase/streamer.hpp libobase/file.hpp libobase/criteria.hpp
	$(cc) -c tests/file_entity_test.cpp -I. -I$(googletest)/googletest/include -o tests/bin/file_entity_test.o

tests/bin/file_entity_test: debug/libobasefile.o debug/libobasecriteria.o debug/libobasestreamer.o tests/bin/file_entity_test.o
	$(cc) -o $@ $? $(googletest)/lib/libgtest.a $(googletest)/lib/libgtest_main.a -pthread

.PHONNY: clean
clean:
	rm debug/* || :
	rm tests/bin/* || :