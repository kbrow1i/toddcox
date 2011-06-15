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
#include <iostream>

/* The CosetTable class provides a toy implementation of the HLT,
   HLT+lookahead, and Felsch algorithms for coset enumeration.  I have
   followed fairly closely the pseudocode in Holt's book, Handbook of
   Computational Group Theory, except that cosets are (internally)
   numbered starting from 0. */

class CosetTable
{
  typedef std::vector<Coset>::iterator tab_iter;
  typedef std::vector<Coset>::const_iterator ctab_iter;
  friend std::ostream& operator<< (std::ostream&, const CosetTable&);
public:
  CosetTable (int NG, std::vector<std::string> rel,
	      std::vector<std::string> gen_H, bool felsch);
  /* method can be a positive integer (threshold for HLT+), 0 (for
     ordinary HLT) or negative (for Felsch). */
  void enumerate (int method);
  int compress (int current = -1);
  void standardize ();
  int getnlive () const;
  int getsize () const { return tab.size (); }
  class Threshold_Exceeded {};	/* exception */
private:
  int NGENS;
  std::vector<Coset> tab;
  std::vector<int> p;	/* for generating equivalence classes; p[k] <= k */
  std::queue<int> q;			/* dead cosets to be processed */
  std::vector<word> relator;
  std::vector<word> generator_of_H;
  std::map< int, std::vector<word> > relator_grouped; /* for Felsch */
  void hlt ();
  void hlt_plus (int threshold);
  void felsch ();
  Stack deduction_stack;	/* for Felsch */
  void lookahead (int start = 0);
  void process_deductions ();	/* for Felsch */
  void scan_and_fill (int k, const word& w, bool save = false);
  void scan (int k, const word& w, bool save = false);
  bool isalive (int k) const { return (p[k] == k); }
  void define (int k, int x, bool save = false);
  bool isdefined (int k, int x) const { return (tab[k][x] >= 0); }
  int rep (int c);
  void merge (int k, int l);
  void coincidence(int k, int l, bool save = false);
  void swap (int k, int l);
};

std::ostream& operator<< (std::ostream&, const CosetTable&);

#endif	/* COSETTABLE_H */
