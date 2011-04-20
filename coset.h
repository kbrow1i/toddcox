#ifndef COSET_H
#define COSET_H

#include "gens_and_words.h"

class Coset
{
public:
  Coset ();
  int get_act (int x) const { return row[x]; };
  void set_act (int x, int k) { row[x] = k; return; } ;
  void print (bool standard = true) const;
private:
  int row[NGENS];		// row[x] is index of coset acted on by x
};

#endif	/* COSET_H */
