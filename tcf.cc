/* tcf.cc: A driver for the Todd-Coxeter (Felsch) routines.

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

// This is a toy implementation of the HLT version of the
// Todd--Coxeter procedure, based on Holt, Handbook of computational
// group theory.

#include "cosettable.h"
#include "gens_and_words.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

void getgroup (int& N, vector<string>& rel, vector<string>& gen_H);
bool getfout (ofstream& fout);

int
main (void)
{
  cout << "\nThis program uses the Felsch version of the Todd-Coxeter procedure to\n"
       << "compute the index in a finitely presented group G of a subgroup H.\n"
       << "You will be prompted to enter the number of generators of G, the\n"
       << "defining relators of G, and the generators of H.  Use a,b,... for\n"
       << "the generators of G and A,B,... for their inverses.\n\n";
  int NGENS;
  vector<string> rel;
  vector<string> gen_H;
  getgroup (NGENS, rel, gen_H);
  CosetTable C (NGENS, rel, gen_H, 0, true);
  if (!C.felsch ())
    return 1;
  cout << "\nThe index of H in G is " << C.get_nlive ()
       << ".\nThe coset table had size " << C.get_size ()
       << " before compression.\n\n";
  if (C.get_nlive () < 50)
    {
      cout << "Compressed and standardized coset table:\n\n";
      C.print ();
    }
  else				// Offer to print table to file
    {
      ofstream fout;
      if (getfout (fout))
	{
	  C.compress ();
	  C.standardize ();
	  C.print (fout);
	  fout.close ();
	}
    }
  return 0;
}

// Prompt for number of generators (including inverses) and group and
// subgroup info.
void
getgroup (int& N, vector<string>& rel, vector<string>& gen_H)
{
  cout << "Enter the number of generators:\n> ";
  for (;;)
    {
      string s;
      cin >> s;
      const char * str = s.c_str ();
      int n = atoi (str);
      if (n < 1 || n > 26)
	{
	  cout << "Invalid entry.  Please enter an integer between 1 and 26.\n> ";
	  continue;
	}
      N = 2 * n;
      break;
    }
  for (;;)
    {
      string s; word w;
      cout << "Enter a defining relator for G, or . when finished:\n> ";
      cin >> s;
      if (s == ".")
	break;
      if (!string_to_word (w, s, N))
	{
	  cout << "Invalid entry; use alphabet ";
	  for (int x = 0; x < N - 1; x++)
	    cout << gen[x] << ",";
	  cout << gen[N - 1] << ".\n";
	  continue;
	}
      rel.push_back (s);
    }
  for (;;)
    {
      string s; word w;
      cout << "Enter a generator of H, or . when finished:\n> ";
      cin >> s;
      if (s == ".")
	break;
      if (!string_to_word (w, s, N))
	{
	  cout << "Invalid entry; use alphabet ";
	  for (int x = 0; x < N - 1; x++)
	    cout << gen[x] << ",";
	  cout << gen[N - 1] << ".\n";
	  continue;
	}
      gen_H.push_back (s);
    }
}

bool
getfout (ofstream& fout)
{
  for (;;)
    {
      string s;
      cout << "\nEnter file name for output or . to exit:\n> ";
      cin >> s;
      if (s == ".")
	return false;
      fout.open (s.c_str ());
      if (fout.is_open ())
	return true;
      else
	cout << "Unable to open " << s << "; please try again.\n";
    }
}
