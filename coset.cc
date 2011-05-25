/* coset.cc: the Coset class.

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

#include "coset.h"
#include "gens_and_words.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// Constructor
Coset::Coset (int NG) : NGENS (NG)
{
  vector<int> r (NG);
  for (int i = 0; i < NGENS; i++)
    r[i] = -1;		// all actions initially undefined
  row = r;
}

// Use standard numbering for coset tables, starting with 1 instead of 0.
void
Coset::print (ostream& fout) const
{
  for (int x = 0; x < NGENS; x++)
    fout << setw (4) << row[x] + 1;
}
