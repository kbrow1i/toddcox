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

using namespace std;

const int NOTAGEN = -1;

/* generators (including inverses); they will be represented
   internally by their index in the string gen */
const string gen = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";

int inv (int x);
typedef vector<int> word;

int char_to_gen (char c, int NGENS);
bool string_to_word (word& w, const string& s, int NGENS);
int getnum (const string errprompt);
void getgroup (int& NGENS, vector<string>& rel, vector<string>& gen_H);
bool getfout (ofstream& fout);

void rotate (string& s);



#endif	/* GENS_AND_WORDS_H */
