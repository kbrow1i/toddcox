/* gens_and_words.h: declarations for manipulating generators and words.

   Copyright 2011 Kenneth S. Brown.

   This file is part of Toddcox.

   Toddcox is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version (GPLv3+).

   Toddcox is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Toddcox; if not, see <http://gnu.org/licenses/>.

   Written by Ken Brown <kbrown@cornell.edu>. */

#ifndef GENS_AND_WORDS_H
#define GENS_AND_WORDS_H

#include <vector>
#include <string>
#include <iostream>

const int NOTAGEN = -1;

/* generators (including inverses); they will be represented
   internally by their index in the string gen */
const std::string gens = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";

int inv (int x);
char switch_case (char);

typedef std::vector<int> word;

int char_to_gen (char c, int NGENS);
bool string_to_word (word& w, const std::string& s, int NGENS);

void getgroup (int& NGENS, std::vector<std::string>& rel,
	       std::vector<std::string>& gen_H, std::istream* inp);

void rotate (std::string& s);



#endif	/* GENS_AND_WORDS_H */
