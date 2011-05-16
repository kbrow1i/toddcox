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

#include "cosettable.h"
#include "gens_and_words.h"
#include "stack.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <exception>

using namespace std;

// Constructor
CosetTable::CosetTable (int NG, vector<string> rel, vector<string> gen_H,
			bool felsch) : NGENS (NG)
{
  Coset c (NGENS);
  tab.push_back (c);
  for (int i = 0; i < gen_H.size (); i++)
    {
      word w;
      // Caller should check that strings are valid.
      string_to_word (w, gen_H[i], NGENS);
      generator_of_H.push_back (w);
    }
  set<string> S;
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
	  for (int i = s.size (); i > 0; i--)
	    sinv += gen[inv (w[i - 1])];
	  for (int i = 0; i < s.size (); i++)
	    {
	      S.insert (s);
	      S.insert (sinv);
	      rotate (s);
	      rotate (sinv);
	    }
	}
    }
  // Group elements of S according to first letter
  for (set<string>::iterator it = S.begin (); it != S.end (); it++)
    {
      word w;
      string_to_word (w, *it, NGENS);
      relator_grouped[w[0]].push_back (w);
    }
}

// Define coset k acted on by x to be new coset; return false if
// can't allocate memory.
bool
CosetTable::define (int k, int x, bool save)
{
  int l = tab.size ();		// index and name of new coset
  tab[k].setact(x, l);
  Coset c (NGENS, l, l);
  c.setact(inv (x), k);
  try
    {
      tab.push_back (c);
    }
  catch (exception& e)
    {
      cout << "\n\nCoset table has size " << tab.size ()
	   << "; can't allocate more memory.\n";
      return false;
    }
  if (save)
    {
      deduction ded = {k, x};
      deduction_stack.push (ded);
    }
  return true;
}

void
CosetTable::print (ostream& fout) const
{
  fout << "    ";
  for (int x = 0; x < NGENS; x++)
    fout << setw (4) << gen[x];
  fout << endl;
  for (ctab_iter it = tab.begin (); it != tab.end (); it++)
    if (it->isalive ())
      it->print (fout);
}

#if 0
void
CosetTable::debug_print () const
{
  cout << "       ";
  for (int x = 0; x < NGENS; x++)
    cout << setw (4) << gen[x];
  cout << endl;
   for (int k = 0; k < tab.size (); k++)
    {
      cout << setw (2) << k << setw (3) << p[k] << ": ";
      tab[k].print (false);	// Don't standardize coset numbering
      cout << endl;
    }
  return;
}
#endif

// Return name (= index) of minimal element of equivalence class of
// coset with index k; simplify names of dead cosets encountered on
// the way.
int
CosetTable::rep (int k)
{
  int l, m, n;
  l = k; m = tab[l].getname ();
  while (m < l)
    {
      l = m;
      m = tab[l].getname ();
    } // Now l is the minimal element.
  m = k; n = tab[m].getname ();
  while (n < m)
    {
      tab[m].setname (l);
      m = n;
      n = tab[m].getname ();
    }
  return l;
}

// Merge two equivalence classes; put larger one in queue for
// processing (copy definitions).  k and l are the indices of
// (possibly dead) cosets in the classes to be merged.
void
CosetTable::merge(int k, int l)
{
  k = rep (k);
  l = rep (l);
  if (k == l)
    return;
  int min, max;
  if (k < l)
    {
      min = k;
      max = l;
    }
  else
    {
      min = l;
      max = k;
    }
  tab[max].setname (min);
  q.push (max);
}

void
CosetTable::coincidence (int k, int l, bool save)
{
  merge (k, l);
  while (!q.empty ())
    {
      int e = q.front ();
      q.pop ();
      // Transfer all info about e
      for (int x = 0; x < NGENS; x++)
	{
	  if (!isdefined (e, x))
	    continue;
	  int f = tab[e].getact(x);	// x: e --> f, x^-1: f --> e
	  tab[f].setact(inv (x), -1);	// remove arrow f --> e
	  int e1 = rep (e);
	  int f1 = rep (f);
	  // insert arrows x: e1 --> f1 and y: f1 --> e1, where...
	  int y = inv (x);
	  if (isdefined (e1, x))
	    merge (f1, tab[e1].getact (x));
	  else if (isdefined (f1, y))
	    merge (e1, tab[f1].getact (y));
	  else
	    {
	      tab[e1].setact (x, f1);
	      tab[f1].setact (y, e1);
	      if (save)
		{
		  deduction ded = {e1, x};
		  deduction_stack.push (ded);
		}
	    }
	}
    }
}

// Return false if can't allocate memory for definitions.
bool
CosetTable::scan_and_fill (int k, const word& w, bool save)
{
  int i = 0, j = w.size () - 1;	// Starting pos for forward and backward scans
  int f = k, b = k;		// Starting coset indices for scans
  for (;;)
    {
      // Scan forward
      while (i <= j && isdefined (f, w[i]))
	f = tab[f].getact(w[i++]);
      if (i > j)		// Scan completed, possibly with coincidence
	{
	  if (f != b)
	    coincidence (f, b, save);
	  return true;
	}
      // Scan backward
      while (j >= i && isdefined (b, inv (w[j])))
	b = tab[b].getact (inv (w[j--]));
      if (j < i)		// Scan completed with coincidence
	{
	  coincidence (f, b, save);
	  return true;
	}
      if (j == i)		// Scan completed with deduction
	{
	  tab[f].setact (w[i], b);
	  tab[b].setact (inv (w[i]), f);
	  if (save)
	    {
	      deduction d = {f, w[i]};
	      deduction_stack.push (d);
	    }
	  return true;
	}
      // Scan is incomplete; make a definition to allow it to get further.
      bool alloc = define (f, w[i], save);
      if (!alloc)
	return false;
    }
}

void
CosetTable::scan (int k, const word& w, bool save)
{
  int i = 0, j = w.size () - 1;	// Starting pos for forward and backward scans
  int f = k, b = k;		// Starting coset indices for scans
  // Scan forward
  while (i <= j && isdefined (f, w[i]))
    f = tab[f].getact(w[i++]);
  if (i > j)		// Scan completed, possibly with coincidence
    {
      if (f != b)
	coincidence (f, b, save);
      return;
    }
  // Scan backward
  while (j >= i && isdefined (b, inv (w[j])))
    b = tab[b].getact (inv (w[j--]));
  if (j < i)		// Scan completed with coincidence
    coincidence (f, b, save);
  else if (j == i)		// Scan completed with deduction
    {
      tab[f].setact (w[i], b);
      tab[b].setact (inv (w[i]), f);
      if (save)
	{
	  deduction d = {f, w[i]};
	  deduction_stack.push (d);
	}
    }
  // else scan is incomplete and yields no information
}

coset_enum_result
CosetTable::enumerate (int method)
{
  if (method > 0)
    return (hlt_plus (method));
  if (method == 0)
    return (hlt ());
  return felsch ();
}

// HLT algorithm
coset_enum_result
CosetTable::hlt ()
{
  for (int i = 0; i < generator_of_H.size (); i++)
    scan_and_fill (0, generator_of_H[i]);
  // Must recompute tab.size() after each iteration.  Note that an
  // iterator wouldn't work well here because elements keep getting
  // added to tab.
  for (int k = 0; k < tab.size (); k++)
    {
      for (int i = 0; i < relator.size () && tab[k].isalive (); i++)
	{
	  bool alloc = scan_and_fill (k, relator[i]);
	  if (!alloc)
	    return COSET_ENUM_OUT_OF_MEMORY;
	}
      if (tab[k].isalive ())
	for (int x = 0; x < NGENS; x++)
	  if (!tab[k].isdefined (x))
	    {
	      bool alloc = define (k, x);
	      if (!alloc)
		return COSET_ENUM_OUT_OF_MEMORY;
	    }
    }
  return COSET_ENUM_SUCCESS;
}

// HLT algorithm with lookahead.
coset_enum_result
CosetTable::hlt_plus (int threshold)
{
// Try to reserve space for a table of size threshold to avoid the
// overhead of reallocation.
  try
    {
      tab.reserve (threshold);
    }
  catch (exception& e)
    {
      ;	 // No harm if it fails; just means threshold will be useless.
    }
  for (int i = 0; i < generator_of_H.size (); i++)
    scan_and_fill (0, generator_of_H[i]);
  for (int k = 0; k < tab.size (); k++)
    {
      if (!tab[k].isalive ())
	continue;
      if (tab.size () > threshold)
	{
	  int n = tab.size ();
	  cout << "\nThreshold exceeded; table size is "
	       << n << ".  Looking ahead...\n";
	  lookahead (k);
	  // Move to next live coset, in case k died
	  while (k < n && !tab[k].isalive ())
	    k++;
	  if (k == n)
	    return COSET_ENUM_SUCCESS;
	  k = compress (k);
	  cout << "Table size is now " << tab.size () << ".";
	  if (tab.size () > threshold)
	    {
	      cout << endl;
	      return COSET_ENUM_THRESHOLD_EXCEEDED;
	    }
	  cout << "  Continuing.\n";
	}
      for (int i = 0; i < relator.size () && tab[k].isalive (); i++)
	scan_and_fill (k, relator[i]);
      if (tab[k].isalive ())
	for (int x = 0; x < NGENS; x++)
	  if (!isdefined (k, x))
	    {
	      bool alloc = define (k, x);
	      if (!alloc)
		return COSET_ENUM_OUT_OF_MEMORY;
	    }
    }
  return COSET_ENUM_SUCCESS;
}

// Felsch algorithm
coset_enum_result
CosetTable::felsch ()
{
  for (int i = 0; i < generator_of_H.size (); i++)
    scan_and_fill (0, generator_of_H[i], true);
  process_deductions ();
  for (int k = 0; k < tab.size (); k++)
    {
      for (int x = 0; x < NGENS && tab[k].isalive (); x++)
	if (!isdefined (k, x))
	  {
	    bool alloc = define (k, x, true);
	    if (!alloc)
	      return COSET_ENUM_OUT_OF_MEMORY;
	    process_deductions ();
	  }
    }
  return COSET_ENUM_SUCCESS;
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
      int k = d.coset;
      int x = d.gen;
      vector<word> relx = relator_grouped[x];
      int n = relx.size ();
      for (int i = 0; i < n && tab[k].isalive (); i++)
	scan (k, relx[i], true);
      k = tab[k].getact (x);
      x = inv (x);
      relx = relator_grouped[x];
      for (int i = 0; i < n && tab[k].isalive (); i++)
	scan (k, relx[i], true);
    }
}

int
CosetTable::getnlive () const
{
  int count = 0;
  for (ctab_iter it = tab.begin (); it != tab.end (); it++)
    if (it->isalive ())
      count++;
  return count;
}

void
CosetTable::lookahead (int start)
{
  for (tab_iter it = tab.begin () + start; it != tab.end () ; it++)
    {
      int k = it->getindex ();
      for (int i = 0; i < relator.size () && it->isalive (); i++)
	scan (k, relator[i]);
    }
}

// When compress is called after lookahead in hlt_plus, we have some
// current (live) coset that we are about to process.  After
// compression, we need to resume processing at the same coset, which
// may have been renumbered; compress returns the new number (or -1 if
// a current live coset wasn't specified).
int
CosetTable::compress (int current)
{
  int ret = -1;
  tab_iter it1 = tab.begin ();  // Pointer to new position for *it
  for (ctab_iter it = tab.begin (); it != tab.end (); it++)
    {
      if (!it->isalive ())
	continue;
      // Copy info from it to it1.
      int old = it->getindex ();
      int l = it1->getindex ();
      if (old == current)
	ret = l;
      if (old > l)		// Replace old by l in table.
	{
	  for (int x = 0; x < NGENS; x++)
	    {
	      int m = it->getact (x);
	      if (m == old)
		it1->setact (x, l);
	      else
		{
		  it1->setact (x, m);
		  if (m >= 0)
		    tab[m].setact(inv (x), l);
		}
	    }
	  it1->setname (l);
	}
      it1++;
    }
  tab.erase (it1, tab.end ());
  return ret;
}
      
// Swap two distinct live rows of the coset table
void
CosetTable::swap (int k, int l)
{
  for (int x = 0; x < NGENS; x++)
    {
      int temp = tab[k].getact (x);
      tab[k].setact (x, tab[l].getact (x));
      tab[l].setact (x, temp);
      for (tab_iter it = tab.begin (); it != tab.end (); it++)
	{
	  if (!it->isalive ())
	    continue;
	  if (it->getact (x) == k)
	    it->setact (x, l);
	  else if (it->getact (x) == l)
	    it->setact (x, k);
	}
    }
}

// Standardize a complete compressed coset table
void
CosetTable::standardize ()
{
  int n = tab.size ();
  if (n <= 2)
    return;
  int goal = 1;			// next number we want to find in the table
  for (tab_iter it = tab.begin (); it != tab.end (); it++)
    for (int x = 0; x < NGENS; x++)
      {
	int l = it->getact (x);
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
