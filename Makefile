# -*-makefile-gmake-*-
# Makefile.cygwin

#    Copyright 2011 Kenneth S. Brown.

#    This file is part of Toddcox.

#    Toddcox is free software; you can redistribute it and/or modify it
#    under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 3 of the License, or
#    (at your option) any later version (GPLv3+).

#    Toddcox is distributed in the hope that it will be useful, but
#    WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with Toddcox; if not, see <http://gnu.org/licenses/>.

#    Written by Ken Brown <kbrown@cornell.edu>.

EXEEXT = .exe

.PHONY: all

all: tc$(EXEEXT) tc+$(EXEEXT)

tc$(EXEEXT): main.o coset.o cosettable.o gens_and_words.o
	g++ -o tc$(EXEEXT) main.o coset.o cosettable.o gens_and_words.o

tc+$(EXEEXT): main+.o coset.o cosettable.o gens_and_words.o
	g++ -o tc+$(EXEEXT) main+.o coset.o cosettable.o gens_and_words.o

main.o: main.cc cosettable.h gens_and_words.h coset.h
	g++ -c main.cc

main+.o: main+.cc cosettable.h gens_and_words.h coset.h
	g++ -c main+.cc

coset.o: coset.cc coset.h gens_and_words.h
	g++ -c coset.cc

cosettable.o: cosettable.cc cosettable.h gens_and_words.h coset.h
	g++ -c cosettable.cc

gens_and_words.o: gens_and_words.cc gens_and_words.h
	g++ -c gens_and_words.cc
