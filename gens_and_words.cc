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
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

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
      while (cin.get () != '\n')
	;
      if (!gotnum)
	cout << errprompt;
    }
  return n;
}

// Read a string representing a valid word from fin into s and return
// true, or read an empty line and return false.  Always read the
// terminating newline.
bool
getstringword (string& s, int NGENS, istream& fin)
{
  if (!getline (fin, s) || s.empty ())
    return false;
  word w;
  if (string_to_word (w, s, NGENS))
    return true;
  // s is invalid; give up if not interactive
  if (fin != cin)
    {
      cerr << "Invalid word: " << s << endl;
      exit (EXIT_FAILURE);
    }
  // We're working interactively; retry.  
  do
    {
      cout << "Invalid word; use alphabet ";
      for (int x = 0; x < NGENS - 1; x++)
	cout << gen[x] << ",";
      cout << gen[NGENS - 1] << ".\n> ";
      getline (cin, s);
      if (s.empty ())
	return false;
    }
  while (!string_to_word (w, s, NGENS));
  return true;
}

// Accumulate valid strings from fin, one per line, in a vector.
void
getvecstringword (vector<string>& v, int NGENS, istream& fin = cin)
{
  string s;
  while (getstringword (s, NGENS, fin))
    {
      v.push_back (s);
      if (fin == cin)
	cout << "> ";
    }
}

// Read an integer between 1 and 26 from fin; read newline.
int
getngens (istream& fin)
{
  int n;
  if (fin == cin)
    cout << "Number of generators: ";    
  if (fin >> n && n >=1 && n <= 26) // Got valid n, move past newline.
    {
      while (fin.get () != '\n')
	;
      return n;
    }
  // n is invalid; give up if not interactive.
  if (fin != cin)
    {
      cerr << "Number of generators must be an integer between 1 and 26.\n";
      exit (EXIT_FAILURE);
    }
  // We're working interactively; retry.
  string errprompt = "Please enter an integer between 1 and 26: ";
  do
    {
      cout << errprompt;
      n = getnum (errprompt);
    }
  while (n < 1 || n > 26);
  return n;
}

// Prompt for number of generators and group and subgroup info.
void
getgroup (int& NGENS, vector<string>& rel, vector<string>& gen_H, istream& fin)
{
  NGENS = 2 * getngens (fin);		// NGENS counts inverses
  if (fin == cin)
    cout <<
      "Enter the relators for G, one per line; press Enter when finished:\n> ";
  getvecstringword (rel, NGENS, fin);
  if (fin == cin)
    cout <<
      "Enter the generators of H, one per line; press Enter when finished:\n> ";
  getvecstringword (gen_H, NGENS, fin);
}

// Get a valid filename for output and return true, or return false if
// user enters an empty line.  Read newline.
bool
getfout (ofstream& fout)
{
  cout << "Enter file name for output, or press Enter to quit:\n> ";
  bool gotfout = false;
  while (!gotfout)
    {
      string s;
      getline (cin, s);
      if (s.empty ())
	return false;
      fout.open (s.c_str ());
      gotfout = fout.is_open ();
      if (!gotfout)
	cout << "Unable to open " << s << "; please try again.\n> ";
    }
  return true;
}

void
rotate (string& s)
{
  s = s.substr (1) + s[0];
}
