/* equivreln.h: declarations for the EquivReln class.

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

#ifndef EQUIVRELN_H
#define EQUIVRELN_H

#include <vector>

/* An EquivReln is a function object f that keeps track of an
   equivalence relation on natural numbers, such that each class is
   represented by its smallest element.  We have f(i) = i if and only
   if i represents its class.  Otherwise, f(i) is a smaller element
   equivalent to i.  Internally, f is maintained as a vector. */

class EquivReln
{
 public:
  explicit EquivReln (int n = 0) { for (int i = 0; i < n; i++) p.push_back (i); }
  int rep (int);
  int operator () (int k) const { return p[k]; }
  int merge (int, int);
  void add () { p.push_back (p.size ()); }
 private:
  std::vector<int> p;
};


#endif	/* EQUIVRELN_H */
