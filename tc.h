/* tc.h: declarations for the TC class (interface).

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

#ifndef TC_H
#define TC_H

#include <iostream>

#include "gens_and_words.h"
#include "cosettable.h"

class TC
{
public:
  TC (std::istream*, bool, int);
  ~TC () { delete ctp; }
  coset_enum_result enumerate () const { return ctp->enumerate (enum_method); }
  int index () const { return ctp->getnlive (); }
  int table_size () const { return ctp->getsize (); }
  void display_table (std::ostream*, bool standardize = false);
private:
  std::istream* input;
  int enum_method;		/* see cosettable.h */
  CosetTable* ctp;
};


#endif	/* TC_H */
