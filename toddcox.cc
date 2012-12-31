/* toddcox.cc: A driver for the Todd-Coxeter routines.

   Copyright 2011, 2012 Kenneth S. Brown.

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
#include <getopt.h>
#include "tc.h"
#include <config.h>

using namespace std;

void usage ();
void help ();
void parse_args (int, char **, int&, bool&, int&);
void version ();
void gen_progname (const string&);
ostream* getfout ();

static string progname;

int
main (int argc, char *argv[])
{
  bool felsch = false;
  int threshold = 0;
  int fileind = 0;

  gen_progname (argv[0]);
  parse_args (argc, argv, fileind, felsch, threshold);

  istream *input = &cin;
  if (fileind > 0)
    {
      input = new ifstream (argv[fileind]);
      if (!*input)
	{
	  cerr << "Unable to open " << argv[fileind] << endl;
	  delete input;
	  exit (1);
	}
    }

  TC tc (input, felsch, threshold);
  tc.enumerate ();
  int index = tc.index ();
  cout << "\nThe index of H in G is " << index
       << ".\nThe coset table had size " << tc.table_size ()
       << " before compression.\n\n";
  ostream *output = &cout;
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
}

void
parse_args (int argc, char *argv[], int& fileind, bool& felsch,
	    int& threshold)
{
  const struct option long_options[] =
    {
      {"felsch",    no_argument,       NULL, 'f'},
      {"threshold", required_argument, NULL, 't'},
      {"help",	    no_argument,       NULL, 'h'},
      {"usage",	    no_argument,       NULL, 'u'},
      {"version",   no_argument,       NULL, 'v'},
      {NULL,	    no_argument,       NULL,  0 }
    };

  const char *short_options = "ft:hvu";

  int opt;
  while ((opt = getopt_long (argc, argv, short_options, long_options, NULL))
	 != -1)
    {
      switch (opt)
	{
	case 'f':
	  felsch = true;
	  break;
	case 't':
	  if ((threshold = atoi (optarg)) <= 0)
	    {
	      usage ();
	      exit (1);
	    }
	  break;
	case 'v':
	  version ();
	  exit (1);
	  break;
	case 'h':
          help ();
          exit (1);
          break;
	default:
	  usage ();
	  exit (1);
	  break;
	}
    }

  if (felsch && (threshold > 0))
    {
      usage ();
      exit (1);
    }

  if (optind < argc - 1)
    {
      usage ();
      exit (1);
    }
  if (optind == argc - 1)
    fileind = optind;
}

void
usage ()
{
  cerr << "\
Usage: " << progname << " [-t THRESHOLD | -f]  [FILE]\n\n\
Try `" << progname << " --help' for more information.\n";
}

void
help ()
{
  cerr <<"\
Usage: " << progname << " [OPTIONS] [FILE]\n\
Enumerate the cosets of a subgroup of a group.\n\n\
If FILE is specified, it should contain the information about the\n\
group and subgroup.  If it is not specified, the user is prompted\n\
for that information.\n\n\
The HLT coset enumeration method is used unless one of the (mutually\n\
exclusive) options -t or -f is given.\n\n\
Options:\n\n\
  -f, --felsch               Use the Felsch enumeration method.\n\
  -t, --threshold=THRESHOLD  Use the HLT+lookahead method.  This means\n\
                             that if an HLT step causes the coset table\n\
                             to get bigger than THRESHOLD, processing\n\
                             will not continue unless the table size\n\
                             can be reduced.  THRESHOLD must be a\n\
                             positive integer.\n\
  -v, --version              Print version information and exit.\n\
  -u, --usage                Print a brief usage message and exit.\n\
  -h, --help                 Print this help text and exit.\n";
}

void
version ()
{
  cerr << PACKAGE_STRING << endl << "\
Copyright (C) 2012 Kenneth S. Brown\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n\
This is free software; you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n\
Report bugs to <" << PACKAGE_BUGREPORT << ">.\n";
}

void
gen_progname (const string& arg0)
{
  size_t found = arg0.find_last_of ('/');
  progname = (found == string::npos) ? arg0 : arg0.substr (found + 1);
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
