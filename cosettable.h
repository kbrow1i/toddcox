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
  void print () const;
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
