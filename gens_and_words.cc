/* gens_and_words.cc: manipulating generators and words.

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

#include "gens_and_words.h"

int
inv (int x)
{
  if (x == a)
    return A;
  if (x == A)
    return a;
  if (x == b)
    return B;
  if (x == B)
    return b;
  return NOTAGEN;
}

int
char_to_gen (char c)
{
  if (c == 'a')
    return a;
  if (c == 'A')
    return A;
  if (c == 'b')
    return b;
  if (c == 'B')
    return B;
  return NOTAGEN;
}

char
gen_to_char (int x)
{
  if (x == a)
    return 'a';
  if (x == A)
    return 'A';
  if (x == b)
    return 'b';
  if (x == B)
    return 'B';
}

bool
string_to_word (word& w, const string& s)
{
  for (int i = 0; i < s.size (); i++)
    {
      int x;
      if ((x = char_to_gen (s[i])) == NOTAGEN)
	return false;
      w.push_back (x);
    }
  return true;
}
