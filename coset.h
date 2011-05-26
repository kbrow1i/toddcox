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

/* A Coset is intended to be used as a "row" in a CosetTable.  It is
represented internally as a vector 'row', in which row[x] is the coset
acted on by a generator x, or -1 if the action is not yet defined. */

class Coset
{
  friend std::ostream& operator<< (std::ostream&, const Coset&);
public:
  Coset (int NG) : NGENS (NG), row (NG, -1) {}
  int& operator[](int x) { return row[x]; }
  const int& operator[](int x) const { return row[x]; }
  bool isdefined (int x) const { return row[x] >= 0; }
  void undefine (int x) { row[x] = -1; }
private:
  int NGENS;			/* including inverses */
  std::vector<int> row;
};

std::ostream& operator<< (std::ostream&, const Coset&);


#endif	/* COSET_H */
