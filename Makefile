.PHONY: all

all: toddcox.exe

toddcox.exe: main.o coset.o cosettable.o gens_and_words.o
	g++ -o toddcox main.o coset.o cosettable.o gens_and_words.o

main.o: main.cc cosettable.h gens_and_words.h coset.h
	g++ -c main.cc

coset.o: coset.cc coset.h gens_and_words.h
	g++ -c coset.cc

cosettable.o: cosettable.cc cosettable.h gens_and_words.h coset.h
	g++ -c cosettable.cc

gens_and_words.o: gens_and_words.cc gens_and_words.h
	g++ -c gens_and_words.cc
