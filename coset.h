/* coset.h: declarations for the Coset class.

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

#ifndef COSET_H
#define COSET_H

#include "gens_and_words.h"
#include <vector>
#include <iostream>

class Coset
{
public:
  Coset (int NG);
  int get_act (int x) const { return row[x]; };
  void set_act (int x, int k) { row[x] = k; } ;
  void print (std::ostream& fout = std::cout) const;
private:
  std::vector<int> row;		/* row[x] is index of coset acted on by x */
  int NGENS;			/* including inverses */
};

#endif	/* COSET_H */
