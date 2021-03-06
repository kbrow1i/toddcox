/* cosettable.cc: the CosetTable class.

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

#include <iostream>
#include <iomanip>
#include <set>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>

#include "cosettable.h"
#include "gens_and_words.h"
#include "stack.h"
#include "equivreln.h"

using namespace std;

// Constructor
CosetTable::CosetTable (int NG, vector<string> rel, vector<string> gen_H,
			bool felsch) : NGENS (NG), p (EquivReln (1))
{
  row r (NGENS, -1);
  tab.push_back (r);
  for (int i = 0; i < gen_H.size (); i++)
    {
      word w;
      // Caller should check that strings are valid.
      string_to_word (w, gen_H[i], NGENS);
      generator_of_H.push_back (w);
    }
  vector<string> R;
  for (int i = 0; i < rel.size (); i++)
    {
      word w;
      string s = rel[i];
      string_to_word (w, s, NGENS);
      relator.push_back (w);
      if (felsch)
	{
	  generator_of_H.push_back (w);
	  // Accumulate s, s^{-1}, and all cyclic conjugates
	  string sinv;
	  transform (s.rbegin (), s.rend (), back_inserter (sinv), switch_case);
	  for (int i = 0; i < s.size (); i++)
	    {
	      R.push_back (s);
	      R.push_back (sinv);
	      rotate (s);
	      rotate (sinv);
	    }
	}
    }
  // Group elements of R according to first letter after removing duplicates.
  if (felsch)
    {
      typedef vector<string>::const_iterator VI;
      VI itu = unique (R.begin (), R.end ());
      for (VI it = R.begin (); it != itu; it++)
	{
	  word w;
	  string_to_word (w, *it, NGENS);
	  relator_grouped[w[0]].push_back (w);
	}
    }
}

// Define coset k acted on by x to be new coset; exit if can't
// allocate memory.
void
CosetTable::define (coset k, gen x, bool save)
{
  const int l = tab.size ();	// index of new coset
  tab[k][x] = l;
  row r (NGENS, -1);
  r[inv (x)] = k;
  try
    {
      tab.push_back (r);
      p.add ();		// p(l) = l
    }
  catch (bad_alloc)
    {
      cerr << "\n\nCoset table has size " << tab.size ()
	   << "; memory exhausted.\n";
      exit (EXIT_FAILURE);
    }
  if (save)
    {
      deduction ded = {k, x};
      deduction_stack.push (ded);
    }
}

// Use standard numbering for coset tables, starting with 1 instead of 0.
ostream&
operator<< (ostream& os, const CosetTable::row& r)
{
  os << setw (4) << r[0] + 1;
  for (CosetTable::gen x = 1; x < r.size (); x++)
    os << " " << setw (3) << r[x] + 1;
  return os;
}

ostream&
operator<< (ostream& os, const CosetTable& C)
{
  os << "    ";
  for (CosetTable::gen x = 0; x < C.NGENS; x++)
    os << setw (4) << gens[x];
  os << endl;
  for (CosetTable::coset k = 0; k < C.tab.size (); k++)
    if (C.isalive (k))
	os << setw (2) << k + 1 << ": " << C.tab[k] << endl;
  return os;
}

#if 0
void
CosetTable::debug_print () const
{
  cout << "       ";
  for (gen x = 0; x < NGENS; x++)
    cout << setw (4) << gens[x];
  cout << endl;
   for (CosetTable::coset k = 0; k < tab.size (); k++)
    {
      cout << setw (2) << k << setw (3) << p[k] << ": ";
      tab[k].print (false);	// Don't standardize coset numbering
      cout << endl;
    }
  return;
}
#endif

void
CosetTable::coincidence (coset k, coset l, bool save)
{
  merge (k, l);
  while (!q.empty ())
    {
      coset e = q.front ();
      q.pop ();
      // Transfer all info about e
      for (gen x = 0; x < NGENS; x++)
	{
	  if (!isdefined (e, x))
	    continue;
	  gen y = inv (x);
	  coset f = tab[e][x];	// x: e --> f, y: f --> e
	  undefine (f, y);	// remove arrow f --> e
	  coset e1 = p.rep (e);
	  coset f1 = p.rep (f);
	  // insert arrows x: e1 --> f1 and y: f1 --> e1
	  if (isdefined (e1, x))
	    merge (f1, tab[e1][x]);
	  else if (isdefined (f1, y))
	    merge (e1, tab[f1][y]);
	  else
	    {
	      tab[e1][x] = f1;
	      tab[f1][y] = e1;
	      if (save)
		{
		  deduction ded = {e1, x};
		  deduction_stack.push (ded);
		}
	    }
	}
    }
}

void
CosetTable::scan_and_fill (coset k, const word& w, bool save)
{
  int i = 0, j = w.size () - 1;	// Starting pos for forward and backward scans
  coset f = k, b = k;		// Starting cosets for scans
  for (;;)
    {
      // Scan forward
      while (i <= j && isdefined (f, w[i]))
	f = tab[f][w[i++]];
      if (i > j)		// Scan completed, possibly with coincidence
	{
	  if (f != b)
	    coincidence (f, b, save);
	  return;
	}
      // Scan backward
      while (j >= i && isdefined (b, inv (w[j])))
	b = tab[b][inv (w[j--])];
      if (j < i)		// Scan completed with coincidence
	{
	  coincidence (f, b, save);
	  return;
	}
      if (j == i)		// Scan completed with deduction
	{
	  tab[f][w[i]] = b;
	  tab[b][inv (w[i])] = f;
	  if (save)
	    {
	      deduction d = {f, w[i]};
	      deduction_stack.push (d);
	    }
	  return;
	}
      // Scan is incomplete; make a definition to allow it to get further.
      define (f, w[i], save);
    }
}

void
CosetTable::scan (coset k, const word& w, bool save)
{
  int i = 0, j = w.size () - 1;	// Starting pos for forward and backward scans
  coset f = k, b = k;		// Starting cosets for scans
  // Scan forward
  while (i <= j && isdefined (f, w[i]))
    f = tab[f][w[i++]];
  if (i > j)		// Scan completed, possibly with coincidence
    {
      if (f != b)
	coincidence (f, b, save);
      return;
    }
  // Scan backward
  while (j >= i && isdefined (b, inv (w[j])))
    b = tab[b][inv (w[j--])];
  if (j < i)		// Scan completed with coincidence
    coincidence (f, b, save);
  else if (j == i)		// Scan completed with deduction
    {
      tab[f][w[i]] = b;
      tab[b][inv (w[i])] = f;
      if (save)
	{
	  deduction d = {f, w[i]};
	  deduction_stack.push (d);
	}
    }
  // else scan is incomplete and yields no information
}

void
CosetTable::enumerate (int method)
{
  if (method == 0)
    hlt ();
  else if (method < 0)
    felsch ();
  else
    {
      try
	{
	  hlt_plus (method);
	}
      catch (Threshold_Exceeded)
	{
	  cerr << "\nSorry, can't recover enough memory.\n"
	       << "Please try again with a bigger threshold.\n";
	  exit (EXIT_FAILURE);
	}
    }
}

// HLT algorithm
void
CosetTable::hlt ()
{
  for (int i = 0; i < generator_of_H.size (); i++)
    scan_and_fill (0, generator_of_H[i]);
  // Must recompute tab.size() after each iteration.  Note that an
  // iterator wouldn't work well here because elements keep getting
  // added to tab.
  for (coset k = 0; k < tab.size (); k++)
    {
      for (int i = 0; i < relator.size () && isalive (k); i++)
	scan_and_fill (k, relator[i]);
      if (isalive (k))
	for (gen x = 0; x < NGENS; x++)
	  if (!isdefined (k, x))
	    define (k, x);
    }
}

// HLT algorithm with lookahead.
void
CosetTable::hlt_plus (int threshold)
{
// Try to reserve space for a table of size threshold to avoid the
// overhead of reallocation.
  try
    {
      tab.reserve (threshold);
    }
  catch (bad_alloc)
    {
      ;	 // No harm if it fails; just means threshold will be useless.
    }
  catch (length_error)
    {
      ;				// ditto
    }
  for (int i = 0; i < generator_of_H.size (); i++)
    scan_and_fill (0, generator_of_H[i]);
  for (coset k = 0; k < tab.size (); k++)
    {
      if (!isalive (k))
	continue;
      if (tab.size () > threshold)
	{
	  const int n = tab.size ();
	  cout << "\nThreshold exceeded; table size is "
	       << n << ".  Looking ahead...\n";
	  lookahead (k);
	  // Move to next live coset, in case k died
	  while (k < n && !isalive (k))
	    k++;
	  if (k == n)
	    return;
	  k = compress (k);
	  cout << "Table size is now " << tab.size () << ".";
	  if (tab.size () > threshold)
	    throw Threshold_Exceeded ();
	  cout << "  Continuing.\n";
	}
      for (int i = 0; i < relator.size () && isalive (k); i++)
	scan_and_fill (k, relator[i]);
      if (isalive (k))
	for (gen x = 0; x < NGENS; x++)
	  if (!isdefined (k, x))
	    define (k, x);
    }
}

// Felsch algorithm
void
CosetTable::felsch ()
{
  for (int i = 0; i < generator_of_H.size (); i++)
    scan_and_fill (0, generator_of_H[i], true);
  process_deductions ();
  for (coset k = 0; k < tab.size (); k++)
    {
      for (gen x = 0; x < NGENS && isalive (k); x++)
	if (!isdefined (k, x))
	  {
	    define (k, x, true);
	    process_deductions ();
	  }
    }
}

void
CosetTable::process_deductions ()
{
  while (!deduction_stack.is_empty ())
    {
      if (deduction_stack.is_full ())
	{
	  lookahead ();
	  deduction_stack.erase ();
	  return;
	}
      deduction d;
      deduction_stack.pop (d);
      coset k = d.c;
      gen x = d.x;
      vector<word> relx = relator_grouped[x];
      const int n = relx.size ();
      for (int i = 0; i < n && isalive (k); i++)
	scan (k, relx[i], true);
      // No need to continue with this deduction if k died.
      if (!isalive (k))
	continue;
      k = tab[k][x];
      x = inv (x);
      relx = relator_grouped[x];
      for (int i = 0; i < n && isalive (k); i++)
	scan (k, relx[i], true);
    }
}

int
CosetTable::getnlive () const
{
  int count = 0;
  const int n = tab.size ();
  for (coset k = 0; k < n; k++)
    if (isalive (k))
      count++;
  return count;
}

void
CosetTable::lookahead (coset start)
{
  const int n = tab.size ();
  for (coset k = start; k < n; k++)
    for (int i = 0; i < relator.size () && isalive (k); i++)
      scan (k, relator[i]);
}

// When compress is called after lookahead in hlt_plus, we have some
// current (live) coset that we are about to process.  After
// compression, we need to resume processing at the same coset, which
// may have been renumbered; compress returns the new number (or -1 if
// a current live coset wasn't specified).
CosetTable::coset
CosetTable::compress (coset current)
{
  coset l = 0;
  const int n = tab.size ();
  coset ret = -1;
  for (coset k = 0; k < n; k++)
    if (isalive (k))
      {
	if (k == current)
	  ret = l;
	if (k > l)		// Replace k by l in table
	  {
	    for (gen x = 0; x < NGENS; x++)
	      {
		coset m = tab[k][x];
		if (m == k)
		  tab[l][x] = l;
		else
		  {
		    tab[l][x] = m;
		    if (m >= 0)
		      tab[m][inv (x)] = l;
		  }
	      }
	  }
	l++;
      }
  p = EquivReln (l);
  if (l < n)
    tab.erase (tab.begin () + l, tab.end ());
  return ret;
}
      
// Swap two distinct live rows of the coset table
void
CosetTable::swap (coset k, coset l)
{
  for (gen x = 0; x < NGENS; x++)
    {
      const coset temp = tab[k][x];
      tab[k][x] = tab[l][x];
      tab[l][x] = temp;
      const int n = tab.size ();
      for (coset m = 0; m < n; m++)
	{
	  if (!isalive (m))
	    continue;
	  if (tab[m][x] == k)
	    tab[m][x] = l;
	  else if (tab[m][x] == l)
	    tab[m][x] = k;
	}
    }
}

// Standardize a complete compressed coset table
void
CosetTable::standardize ()
{
  const int n = tab.size ();
  if (n <= 2)
    return;
  coset goal = 1;	   // next coset number we want to find in the table
  for (tab_iter it = tab.begin (); it != tab.end (); it++)
    for (gen x = 0; x < NGENS; x++)
      {
	coset l = (*it)[x];
	if (l >= goal)
	  {
	    if (l > goal)
	      swap (l, goal);
	    goal++;
	    if (goal == n - 1)
	      return;
	  }
      }
}
