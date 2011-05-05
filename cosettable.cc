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

// Constructor
CosetTable::CosetTable (int NG, vector<string> rel, vector<string> gen_H,
			int t, bool felsch) : NGENS (NG), threshold (t)
{
  Coset c (NGENS);
  tab.push_back (c);
  p.push_back (0);		// p[0] = 0
  if (t > 0)
    {
      try
	{
	  tab.reserve (1.1 * t);
	}
      catch (exception& e)
	{
	  threshold = -1;
	}
    }
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

// Define coset k acted on by x to be new coset; return false if can't
// allocate memory.
bool
CosetTable::define (int k, int x, bool save)
{
  int l = tab.size ();		// index of new coset
  tab[k].set_act(x, l);
  Coset c (NGENS);
  c.set_act(inv (x), k);
  try
    {
      tab.push_back (c);
      p.push_back (l);		// p[l] = l
    }
  catch (exception& e)
    {
      cout << "\n\nCoset table has size " << get_size () << "; can't allocate more memory.\n";
      return false;
    }
  if (save)
    {
      deduction d = {k, x};
      deduction_stack.push (d);
    }
  return true;
}

void
CosetTable::print (bool standard) const
{
  cout << "    ";
  for (int x = 0; x < NGENS; x++)
    cout << setw (4) << gen[x];
  cout << endl;
  for (int k = 0; k < tab.size (); k++)
    if (is_alive (k))
      {
	cout << setw (2) << (standard ? k + 1 : k) << ": ";
	tab[k].print (standard);
	cout << endl;
      }
  return;
}

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

// Return minimal element of equivalence class, simplify p along the way
int
CosetTable::rep (int k)
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

// Merge two equivalence classes; put larger one in queue for
// processing (copy definitions)
void
CosetTable::merge(int k, int l)
{
  k = rep (k);
  l = rep (l);
  if (k == l)
    return;
  if (k < l)
    {
      p[l] = k;
      q.push (l);
    }
  else
    {
      p[k] = l;
      q.push (k);
    }
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
	  if (!is_defined (e, x))
	    continue;
	  int f = tab[e].get_act(x);	// x: e --> f, x^-1: f --> e
	  tab[f].set_act(inv (x), -1);	// remove arrow f --> e
	  int e1 = rep (e);
	  int f1 = rep (f);
	  // insert arrows x: e1 --> f1 and y: f1 --> e1, where...
	  int y = inv (x);
	  if (is_defined (e1, x))
	    merge (f1, tab[e1].get_act (x));
	  else if (is_defined (f1, y))
	    merge (e1, tab[f1].get_act (y));
	  else
	    {
	      tab[e1].set_act (x, f1);
	      tab[f1].set_act (y, e1);
	      if (save)
		{
		  deduction d = {e1, x};
		  deduction_stack.push (d);
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
      while (i <= j && is_defined (f, w[i]))
	f = tab[f].get_act(w[i++]);
      if (i > j)		// Scan completed, possibly with coincidence
	{
	  if (f != b)
	    coincidence (f, b, save);
	  return true;
	}
      // Scan backward
      while (j >= i && is_defined (b, inv (w[j])))
	b = tab[b].get_act (inv (w[j--]));
      if (j < i)		// Scan completed with coincidence
	{
	  coincidence (f, b, save);
	  return true;
	}
      if (j == i)		// Scan completed with deduction
	{
	  tab[f].set_act (w[i], b);
	  tab[b].set_act (inv (w[i]), f);
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
  while (i <= j && is_defined (f, w[i]))
    f = tab[f].get_act(w[i++]);
  if (i > j)		// Scan completed, possibly with coincidence
    {
      if (f != b)
	{
	  coincidence (f, b, save);
	}
      return;
    }
  // Scan backward
  while (j >= i && is_defined (b, inv (w[j])))
    b = tab[b].get_act (inv (w[j--]));
  if (j < i)		// Scan completed with coincidence
    {
      coincidence (f, b, save);
      return;
    }
  if (j == i)		// Scan completed with deduction
    {
      tab[f].set_act (w[i], b);
      tab[b].set_act (inv (w[i]), f);
      if (save)
	{
	  deduction d = {f, w[i]};
	  deduction_stack.push (d);
	}
      return;
    }
  // Scan is incomplete and yields no information
  return;
}

// HLT algorithm; return false if can't allocate enough memory.
bool
CosetTable::hlt ()
{
  for (int i = 0; i < generator_of_H.size (); i++)
    scan_and_fill (0, generator_of_H[i]);
  for (int k = 0; k < get_size (); k++)
    {
      for (int i = 0; i < relator.size () && is_alive (k); i++)
	{
	  bool alloc = scan_and_fill (k, relator[i]);
	  if (!alloc)
	    return false;
	}
      if (is_alive (k))
	for (int x = 0; x < NGENS; x++)
	  if (!is_defined (k, x))
	    {
	      bool alloc = define (k, x);
	      if (!alloc)
		return false;
	    }
    }
  return true;
}

// HLT algorithm with lookahead
bool
CosetTable::hlt_plus ()
{
  for (int i = 0; i < generator_of_H.size (); i++)
    scan_and_fill (0, generator_of_H[i]);
  for (int k = 0; k < get_size (); k++)
    {
      if (!is_alive (k))
	continue;
      if (get_size () > threshold)
	{
	  int n = get_size ();
	  cout << "\nThreshold exceeded; table size is " << n << ".  Looking ahead...\n";
	  lookahead (k);
	  // Move to next live coset, in case k died
	  while (k < n && !is_alive (k))
	    k++;
	  if (k == n)
	    return true;
	  k = compress (k);
	  cout << "Table size is now " << tab.size () << ".";
	  if (tab.size () > threshold)
	    {
	      cout << endl;
	      return false;
	    }
	  cout << "  Continuing.\n";
	}
      for (int i = 0; i < relator.size () && is_alive (k); i++)
	scan_and_fill (k, relator[i]);
      if (is_alive (k))
	for (int x = 0; x < NGENS; x++)
	  if (!is_defined (k, x))
	    define (k, x);	// Threshold means we won't run out of memory.
    }
  return true;
}

// Felsch algorithm; return false if can't allocate enough memory.
bool
CosetTable::felsch ()
{
  for (int i = 0; i < generator_of_H.size (); i++)
    scan_and_fill (0, generator_of_H[i], true);
  process_deductions ();
  for (int k = 0; k < get_size (); k++)
    {
      for (int x = 0; x < NGENS && is_alive (k); x++)
	if (!is_defined (k, x))
	  {
	    bool alloc = define (k, x, true);
	    if (!alloc)
	      return false;
	    process_deductions ();
	  }
    }
  return true;
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
      for (int i = 0; i < n && is_alive (k); i++)
	scan (k, relx[i], true);
      k = tab[k].get_act (x);
      x = inv (x);
      relx = relator_grouped[x];
      for (int i = 0; i < n && is_alive (k); i++)
	scan (k, relx[i], true);
    }
}

int
CosetTable::get_nlive () const
{
  int count = 0, n = get_size ();
  for (int k = 0; k < n; k++)
    if (is_alive (k))
      count++;
  return count;
}

void
CosetTable::lookahead (int start)
{
  int n = get_size ();
  for (int k = start; k < n; k++)
    for (int i = 0; i < relator.size () && is_alive (k); i++)
      scan (k, relator[i]);
  return;
}

// When compress is called after lookahead in hlt_plus, we have some
// current (live) coset that we are about to process.  After
// compression, we need to resume processing at the same coset, which
// may have been renumbered; compress returns the new number (or -1 if
// a current live coset wasn't specified).
int
CosetTable::compress (int current)
{
  int l = 0, n = get_size (), ret = -1;
  for (int k = 0; k < n; k++)
    if (is_alive (k))
      {
	if (k == current)
	  ret = l;
	if (k > l)		// Replace k by l in table
	  {
	    for (int x = 0; x < NGENS; x++)
	      {
		int m = tab[k].get_act (x);
		if (m == k)
		  tab[l].set_act (x, l);
		else
		  {
		    tab[l].set_act (x, m);
		    if (m >= 0)
		      tab[m].set_act(inv (x), l);
		  }
	      }
	    p[l] = l;
	  }
	l++;
      }
  if (l < n)
    {
      tab.erase (tab.begin () + l, tab.end ());
      p.erase (p.begin () + l, p.end ());
    }
  return ret;
}
      
// Swap two distinct live rows of the coset table
void
CosetTable::swap (int k, int l)
{
  for (int x = 0; x < NGENS; x++)
    {
      int temp = tab[k].get_act (x);
      tab[k].set_act (x, tab[l].get_act (x));
      tab[l].set_act (x, temp);
      for (int m = 0; m < tab.size (); m++)
	{
	  if (!is_alive (m))
	    continue;
	  if (tab[m].get_act (x) == k)
	    tab[m].set_act (x, l);
	  else if (tab[m].get_act (x) == l)
	    tab[m].set_act (x, k);
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
  for (int k = 0; k < n; k++)
    for (int x = 0; x < NGENS; x++)
      {
	int l = tab[k].get_act (x);
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
