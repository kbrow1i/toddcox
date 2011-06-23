/* equivreln.cc: the EquivReln class.

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

#include <algorithm>

#include "equivreln.h"

using namespace std;

// Return minimal element of equivalence class, simplify p along the way
int
EquivReln::rep (int k)
{
  int l, m, n;
  l = k; m = p[l];
  while (m < l)
    {
      l = m;
      m = p[l];
    } // Now l is the minimal element.
  m = k; n = p[m];
  while (n < m)
    {
      p[m] = l;
      m = n;
      n = p[m];
    }
  return l;
}

// Merge two equivalence classes.  Return the obsolete representative,
// or -1 if the classes were already equal.
int
EquivReln::merge(int k, int l)
{
  k = rep (k);
  l = rep (l);
  if (k < l)
    {
      p[l] = k;
      return l;
    }
  if (l < k)
    {
      p[k] = l;
      return k;
    }
  return -1;
}
