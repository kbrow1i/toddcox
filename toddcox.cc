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

void usage (bool help = false);

// Default (HLT): toddcox [FILE]
// HLT+lookahead: toddcox --threshold THRESHOLD [FILE]
// Felsch: toddcox --felsch [FILE]
// Here FILE contains the group info; otherwise the user is prompted
// for the group info.
int
main (int argc, char * argv[])
{
  // Check for option
  bool felsch = false;
  int threshold = 0;
  if (--argc > 0 && (*++argv)[0] == '-')
    {
      string option = *argv;
      if (option == "--help")
	usage (true);
      else if (option == "--felsch")
	{
	  felsch = true;
	  --argc;
	  ++argv;
	}
      else if (option == "--threshold")
	{
	  if (--argc > 0 && (threshold = atoi (*++argv)) > 0)
	    {
	      --argc;
	      ++argv;
	    }
	  else
	    usage ();
	}
      else
	{
	  cerr << "\nInvalid option: " << option << endl;
	  usage ();
	}
    }
  // Check for filename
  bool interactive = true;
  ifstream fin;
  if (argc > 1)
    {
      cerr << "Too many arguments.\n";
      usage ();
    }
  if (argc == 1)
    {
      interactive = false;
      fin.open (*argv);
      if (!fin)
	{
	  cerr << "Unable to open " << *argv << endl;
	  return 1;
	}
    }
  const string instruct =
    "\nThis program uses the Todd-Coxeter procedure to compute the\n"
    "index in a finitely presented group G of a subgroup H.  You\n"
    "will be prompted to enter the number of generators of G, the\n"
    "defining relators of G, and the generators of H.  Use\n"
    "a,b,... for the generators of G and A,B,... for their\n"
    "inverses.\n\n";
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
  CosetTable C (NGENS, rel, gen_H, felsch);
  int method = felsch ? -1 : threshold;
  coset_enum_result res = C.enumerate (method);
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
      cout << C;
    }
  else				// Offer to print table to file
    {
      ofstream fout;
      if (getfout (fout))
	{
	  C.compress ();	// Don't standardize for big table.
	  fout << C;
	  fout.close ();
	}
    }
  return 0;
}

void
usage (bool help)
{
  const string us =
    "Usage:\n"
    "  toddcox [FILE]\n"
    "  toddcox --threshold THRESHOLD [FILE]\n"
    "  toddcox --felsch [FILE]\n"
    "Here FILE is an optional file name and THRESHOLD is a positive integer.\n\n";
  const string us1 =
    "For more information, give the command\n"
    "  toddcox --help\n";
  const string us2 =
    "If FILE is specified, it should contain the information about\n"
    "the group and subgroup.  If it is not specified, the user is\n"
    "prompted for that information.\n\n"
    "By default, the HLT coset enumeration method is used.  If the\n"
    "'--threshold THRESHOLD' option is given, HLT+lookahead is used.\n"
    "(This means that if an HLT step causes the coset table to get\n"
    "bigger than THRESHOLD, processing will not continue unless the\n"
    "table size can be reduced.)  If the '--felsch' option is given,\n"
    "the Felsch enumeration method is used.\n";
  cout << us;
  if (help)
    {
      cout << us2;
      exit (EXIT_SUCCESS);
    }
  else
    {
      cout << us1;
      exit (EXIT_FAILURE);
    }
}
