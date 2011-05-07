/* tc+.cc: A driver for the Todd-Coxeter (HLT plus lookahead) routines.

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

// This is a toy implementation of the HLT (plus lookahead) version of
// the Todd--Coxeter procedure, based on Holt, Handbook of
// computational group theory.

#include "cosettable.h"
#include "gens_and_words.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

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
    "\nThis program uses the HLT+lookahead version of the Todd-Coxeter\n"
    "procedure to compute the index in a finitely presented group G of\n"
    "a subgroup H.  You will be prompted to enter the number of\n"
    "generators of G, the defining relators of G, and the generators\n"
    "of H.  Use a,b,... for the generators of G and A,B,... for their\n"
    "inverses.  You will also be prompted to enter a threshold.  If an\n"
    "HLT step causes the size of the coset table to exceed the threshold,\n"
    "the program will use lookahead to try to shrink the table.\n\n";
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
  cout << "Enter threshold for HLT with lookahead, or 0 to use ordinary HLT: ";
  int threshold; bool gotthresh = false;
  while (!gotthresh)
    {
      string errprompt = "Please enter a non-negative integer.\n> ";
      threshold = getnum (errprompt);
      gotthresh = (threshold >= 0);
      if (!gotthresh)
	cout << errprompt;
    }
  CosetTable C (NGENS, rel, gen_H, threshold);
  if (C.threshold_is_bad ())
    {
      cout << "\nSorry, threshold too big; please try again.\n";
      return 2;
    }
  bool res = true;
  if (threshold == 0)
    C.hlt ();
  else
    res = C.hlt_plus ();
  if (!res)
    {
      cout << "Sorry, please try again with a bigger threshold.\n\n";
      return 1;
    }
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
