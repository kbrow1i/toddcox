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

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cctype>

#include "gens_and_words.h"

using namespace std;

int
inv (int x)
{
  return x % 2 == 0 ? x + 1 : x - 1;
}

char
switch_case (char c)
{
  if (isupper (c))
    return tolower (c);
  else
    return toupper (c);
}


// NGENS is the number of allowed generators, including inverses.
int
char_to_gen (char c, int NGENS)
{
  size_t found = gens.find_first_of (c);
  if (found != string::npos && found < NGENS)
    return found;
  else
    return NOTAGEN;
}

// char
// gen_to_char (int x)
// {
//   return gens[x];
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

#if 0
// Get an integer from standard input, prompting with errprompt if
// something other than an integer is entered.  Read newline.
int
getnum (const string errprompt)
{
  int n; bool gotnum = false;
  while (!gotnum)
    {
      gotnum = (cin >> n);
      // Read newline after resetting cin (in case there was an error).
      cin.clear ();
      if (!gotnum)
	cout << errprompt;
    }
  return n;
}
#endif

// Read a string representing a valid word from *inp into s and return
// true, or read an empty line and return false.  Always read the
// terminating newline.
bool
getstringword (string& s, int NGENS, istream* inp )
{
  if (!getline (*inp, s) || s.empty ())
    return false;
  word w;
  if (string_to_word (w, s, NGENS))
    return true;
  // s is invalid; give up if not interactive
  if (inp != &cin)
    {
      cerr << "Invalid word: " << s << endl;
      exit (EXIT_FAILURE);
    }
  // We're working interactively; retry.  
  do
    {
      cout << "Invalid word; use alphabet ";
      for (int x = 0; x < NGENS - 1; x++)
	cout << gens[x] << ",";
      cout << gens[NGENS - 1] << ".\n> ";
      getline (cin, s);
      if (s.empty ())
	return false;
    }
  while (!string_to_word (w, s, NGENS));
  return true;
}

// Accumulate valid strings from *inp, one per line, in a vector.
void
getvecstringword (vector<string>& v, int NGENS, istream* inp)
{
  string s;
  while (getstringword (s, NGENS, inp))
    {
      v.push_back (s);
      if (inp == &cin)
	cout << "> ";
    }
}

// Read an integer between 1 and 26 from *inp; read newline.
int
getngens (istream* inp)
{
  int n;
  if (inp == &cin)
    cout << "Number of generators: ";
  bool gotngens = (*inp >> n && n >=1 && n <= 26);
  if (inp != &cin && !gotngens)
    {
      cerr << "Number of generators must be an integer between 1 and 26.\n";
      exit (EXIT_FAILURE);
    }
  // Now n is valid, or else we're working interactively and n is
  // invalid; in that case we retry after clearing cin and any invalid
  // input.
  while (!gotngens)
    {
      cin.clear ();
      char c;
      while (cin.get (c) && c != '\n')
	;
      cout << "Please enter an integer between 1 and 26: ";
      gotngens = (cin >> n && n >=1 && n <= 26);
    }
  // Read newline.
  char c;
  while (inp->get (c) && c != '\n')
    ;
  return n;
}

// Prompt for number of generators and group and subgroup info.
void
getgroup (int& NGENS, vector<string>& rel, vector<string>& gen_H, istream* inp)
{
  NGENS = 2 * getngens (inp);		// NGENS counts inverses
  if (inp == &cin)
    cout <<
      "Enter the relators for G, one per line; press Enter when finished:\n> ";
  getvecstringword (rel, NGENS, inp);
  if (inp == &cin)
    cout <<
      "Enter the generators of H, one per line; press Enter when finished:\n> ";
  getvecstringword (gen_H, NGENS, inp);
}

void
rotate (string& s)
{
  s = s.substr (1) + s[0];
}
