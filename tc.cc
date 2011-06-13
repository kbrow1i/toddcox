/* tc.cc: the TC class (interface).

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
#include <vector>

#include "cosettable.h"
#include "gens_and_words.h"
#include "tc.h"

using namespace std;

TC::TC (istream* inp, bool felsch, int threshold)
  : input (inp), enum_method (felsch ? -1 : threshold)
{
  const string instruct =
    "\nThis program uses the Todd-Coxeter procedure to compute the\n"
    "index in a finitely presented group G of a subgroup H.  You\n"
    "will be prompted to enter the number of generators of G, the\n"
    "defining relators of G, and the generators of H.  Use\n"
    "a,b,... for the generators of G and A,B,... for their\n"
    "inverses.\n\n";
  int NGENS;
  vector<string> rel, gen_H;
  if (input == &cin)
    cout << instruct;
  getgroup (NGENS, rel, gen_H, input);
  if (input != &cin)
    delete input;		// Does this close file?
  ctp = new CosetTable (NGENS, rel, gen_H, felsch);
}
  
void
TC::display_table (ostream* outp, bool standardize)
{
  ctp->compress ();
  if (standardize)
    ctp->standardize ();
  *outp << *ctp;
}
