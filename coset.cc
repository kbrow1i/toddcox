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

using namespace std;

// Use standard numbering for coset tables, starting with 1 instead of 0.
ostream&
operator<< (ostream& os, const Coset& c)
{
  for (int x = 0; x < c.NGENS; x++)
    os << setw (4) << c.row[x] + 1;
  return os;
}
