#include "coset.h"
#include "gens_and_words.h"
#include <iostream>

// Constructor
Coset::Coset ()
{
  for (int i = 0; i < NGENS; i++)
    row[i] = -1;		// all actions initially undefined
}

void
Coset::print () const
{
  cout << row[0];
  for (int x = 1; x < NGENS; x++)
    cout << " " << row[x];
}
