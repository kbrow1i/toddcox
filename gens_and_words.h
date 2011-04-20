/* gens_and_words.h: declarations for manipulating generators and words.

   Copyright 2011 Kenneth S. Brown.

   This file is part of toddcox.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 3 of the
   License, or (at your option) any later version (GPLv3+).

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://gnu.org/licenses/>.

   Written by Ken Brown <kbrown@cornell.edu>. */

#ifndef GENS_AND_WORDS_H
#define GENS_AND_WORDS_H

#include <vector>
#include <string>

using namespace std;

const int NGENS = 4;
const int NOTAGEN = -1;
enum { a, A, b, B };		/* generators */
int inv (int x);		/* inverses */
typedef vector<int> word;

int char_to_gen (char c);
char gen_to_char (int x);
bool string_to_word (word& w, const string& s);



#endif	/* GENS_AND_WORDS_H */
