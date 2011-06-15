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

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "tc.h"

using namespace std;

void usage (bool help = false);

ostream* getfout ();

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
  istream* input = &cin;
  if (argc > 1)
    {
      cerr << "Too many arguments.\n";
      usage ();
    }
  if (argc == 1)
    {
      input = new ifstream (*argv);
      if (!*input)
	{
	  cerr << "Unable to open " << *argv << endl;
	  delete input;
	  return 1;
	}
    }
  TC tc (input, felsch, threshold);
  tc.enumerate ();
  int index = tc.index ();
  cout << "\nThe index of H in G is " << index
       << ".\nThe coset table had size " << tc.table_size ()
       << " before compression.\n\n";
  ostream* output = &cout;
  bool standardize = true;
  const int display_max = 50;
  if (index < display_max)
      cout << "Compressed and standardized coset table:\n\n";
  else			// Offer to print table to file
    {
      standardize = false;
      output = getfout ();
    }
  if (output)
    {
      tc.display_table (output, standardize);
      if (output != &cout)
	delete output;
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

// Get a valid filename for output and return pointer to it, or return
// 0 if user enters an empty line.  Read newline.  Caller is
// responsible for deleting.  (Poor design.)
ostream*
getfout ()
{
  ostream* ret = 0;
  do
    {
      cout << "Enter file name for output, or press Enter to quit:\n> ";
      string s;
      getline (cin, s);
      if (s.empty ())
	break;
      ret = new ofstream (s.c_str ());
      if (!*ret)
	{
	  cout << "Unable to open " << s << "; please try again.\n> ";
	  delete ret;
	  ret = 0;
	}
    }
  while (!ret);
  return ret;
}
