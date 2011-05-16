/* tc.cc: A driver for the Todd-Coxeter routines.

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

int getmethod ();

int
main (int argc, char * argv[])
{
  // If there's a command line argument, it should be the name of a
  // file containing the group info; otherwise prompt the user for the
  // group info.
  bool interactive = (argc == 1);
  ifstream fin;
  if (!interactive)
    {
      if (argc != 2)
	{
	  cerr << "Too many arguments.  Usage: " << argv[0] << " [filename]\n";
	  exit (EXIT_FAILURE);
	}
      fin.open (argv[1]);
      if (!fin.is_open ())
	{
	  cerr << "unable to open " << argv[1] << endl;
	  exit (EXIT_FAILURE);
	}
    }
const string instruct =
    "\nThis program uses the Todd-Coxeter procedure to compute the\n"
    "index in a finitely presented group G of a subgroup H.  You\n"
    "will be prompted to enter the number of generators of G, the\n"
    "defining relators of G, and the generators of H.  Use\n"
    "a,b,... for the generators of G and A,B,... for their\n"
    "inverses.  By default, the HLT method is used.  You may\n"
    "choose to use the HLT+lookahead method or the Felsch method\n"
    "instead.\n\n";
  int NGENS;
  vector<string> rel, gen_H;
  if (!interactive)
    {
      getgroup (NGENS, rel, gen_H, fin);
      fin.close ();
    }
  else
    {
      cout << instruct;
      getgroup (NGENS, rel, gen_H);
    }
  int meth = getmethod ();
  bool felsch = (meth == -1);
  CosetTable C (NGENS, rel, gen_H, felsch);
  coset_enum_result res = C.enumerate (meth);
  if (res == COSET_ENUM_OUT_OF_MEMORY)
    return 1;			// Out-of-memory message has already
				// been written.
  if (res == COSET_ENUM_THRESHOLD_EXCEEDED)
    {
      cout << "Sorry, please try again with a bigger threshold.\n\n";
      return 1;
    }
  // Success
  cout << "\nThe index of H in G is " << C.getnlive ()
       << ".\nThe coset table had size " << C.getsize ()
       << " before compression.\n\n";
  if (C.getnlive () < 50)
    {
      cout << "Compressed and standardized coset table:\n\n";
      C.compress ();
      C.standardize ();
      C.print ();
    }
  else				// Offer to print table to file
    {
      ofstream fout;
      if (getfout (fout))
	{
	  C.compress ();	// Don't standardize for big table.
	  C.print (fout);
	  fout.close ();
	}
    }
  return 0;
}

// Get enumeration method.  -1 means Felsch, 0 means HLT, a positive
// number is a threshold for HLT+.
int
getmethod ()
{
  cout <<
    "\nPress Enter to use the default HLT enumeration method, or\n"
    "enter + for HLT+lookahead or f for Felsch: ";
  string s;
  getline (cin, s);
  if (s.empty ())
    {
      cout << "\nUsing HLT.\n";
      return 0;
    }
  if (s == "f" || s == "F")
    {
      cout << "\nUsing Felsch.\n";
      return -1;
    }
  if (s == "+")
    {
      cout <<
	"\nUsing HLT+lookahead.  If an HLT step causes the size of the\n"
	"coset table to exceed a specified threshold, the program\n"
	"will use lookahead to try to shrink the table.\n\n"
	"Enter threshold: ";
      int threshold; bool gotthresh = false;
      while (!gotthresh)
	{
	  string errprompt = "Please enter a positive integer.\n> ";
	  threshold = getnum (errprompt);
	  gotthresh = (threshold > 0);
	  if (!gotthresh)
	    cout << errprompt;
	}
      return threshold;
    }
  // The user entered something else.
  cout << "\nInvalid choice; using default HLT method.\n";
  return 0;    
}
