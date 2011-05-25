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

/* A Coset is intended to be used as a "row" in a CosetTable.  It has
   an index, which represents its position in the table, and it has a
   name.  The name equals the index as long as the coset is alive;
   otherwise the name is the index of an earlier equivalent coset. */

class Coset
{
public:
  Coset (int NG, int nm = 0, int ind = 0);
  int getact (int x) const { return row[x]; };
  void setact (int x, int k) { row[x] = k; } ;
  void print (std::ostream& fout = std::cout) const;
  int getname () const { return name; };
  void setname (int k) { name = k; };
  int getindex () const { return index; };
  void setindex (int k) { index = k; };
  /* Test a coset to see if it's alive */
  operator bool () const { return name == index; };
  bool isdefined (int x) const { return row[x] >= 0; };
private:
  std::vector<int> row;		/* row[x] is name of this coset acted on by x */
  int NGENS;			/* including inverses */
  int name;
  int index;
};

#endif	/* COSET_H */
