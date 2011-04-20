/* cosettabe.h: declarations for the CosetTable class.

   Copyright 2011 Kenneth S. Brown.

   This file is part of toddcox.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 3 of the
   License, or (at your option) any later version (GPLv3+).

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://gnu.org/licenses/>.

   Written by Ken Brown <kbrown@cornell.edu>. */

#ifndef COSETTABLE_H
#define COSETTABLE_H

#include "coset.h"
#include "gens_and_words.h"
#include <vector>
#include <queue>

class CosetTable
{
public:
  CosetTable (vector<word> rel, vector<word> gen_H);
  void define (int k, int x);
  bool is_defined (int k, int x) const { return (tab[k].get_act (x) >= 0); };
  void print (bool standard = true) const;
  void debug_print () const;
  void scan_and_fill (int k, const word& w);
  void scan (int k, const word& w);
  bool is_alive (int k) const { return (p[k] == k); };
  int get_size () const { return tab.size (); };
  void hlt ();
  bool hlt_plus (int threshold);
  void lookahead ();
  int get_nlive () const;
  void compress ();
  void standardize ();
private:
  vector<Coset> tab;
  vector<int> p;		// for generating equivalence classes; p[k] <= k
  queue<int> q;			// dead cosets to be processed
  int rep (int c);
  void merge (int k, int l);
  void coincidence(int k, int l);
  vector<word> relator;
  vector<word> generator_of_H;
  void swap (int k, int l);
};



#endif	/* COSETTABLE_H */
