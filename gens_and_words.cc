/* gens_and_words.cc: manipulating generators and words.

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

#include "gens_and_words.h"

int
inv (int x)
{
  return x % 2 == 0 ? x + 1 : x - 1;
}

// NGENS is the number of allowed generators, including inverses.
int
char_to_gen (char c, int NGENS = 4)
{
  size_t found = gen.find_first_of (c);
  if (found != string::npos && found < NGENS)
    return found;
  else
    return NOTAGEN;
}

// char
// gen_to_char (int x)
// {
//   return gen[x];
// }

bool
string_to_word (word& w, const string& s, int NGENS)
{
  for (int i = 0; i < s.size (); i++)
    {
      int x;
      if ((x = char_to_gen (s[i], NGENS)) == NOTAGEN)
	return false;
      w.push_back (x);
    }
  return true;
}

void
rotate (string& s)
{
  s = s.substr (1) + s[0];
}
