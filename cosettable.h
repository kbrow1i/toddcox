/* cosettable.h: declarations for the CosetTable class.

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

#ifndef COSETTABLE_H
#define COSETTABLE_H

#include "coset.h"
#include "gens_and_words.h"
#include "stack.h"
#include <vector>
#include <queue>
#include <map>

using namespace std;

class CosetTable
{
public:
  /* 'grouped' means group by first letter for Felsch */
  CosetTable (int NG, vector<word> rel, vector<word> gen_H, bool grouped = false);
  void hlt ();			/* HLT algorithm... */
  bool hlt_plus (int threshold); /* ...with lookahead */
  void felsch ();		 /* Felsch algorithm */
  void compress ();
  void standardize ();
  int get_nlive () const;
  int get_size () const { return tab.size (); };
  void print (bool standard = true) const;
  void debug_print () const;
private:
  int NGENS;
  vector<Coset> tab;
  vector<int> p;	/* for generating equivalence classes; p[k] <= k */
  queue<int> q;			/* dead cosets to be processed */
  Stack deduction_stack;
  vector<word> relator;
  vector<word> generator_of_H;
  map< int, vector<word> > relator_grouped; /* for Felsch */
  void lookahead ();
  void process_deductions ();
  void scan_and_fill (int k, const word& w, bool save = false);
  void scan (int k, const word& w, bool save = false);
  bool is_alive (int k) const { return (p[k] == k); };
  void define (int k, int x, bool save = false);
  bool is_defined (int k, int x) const { return (tab[k].get_act (x) >= 0); };
  int rep (int c);
  void merge (int k, int l);
  void coincidence(int k, int l, bool save = false);
  void swap (int k, int l);
};



#endif	/* COSETTABLE_H */
