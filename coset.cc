#include "coset.h"
#include "gens_and_words.h"
#include <iostream>
#include <iomanip>

// Constructor
Coset::Coset ()
{
  for (int i = 0; i < NGENS; i++)
    row[i] = -1;		// all actions initially undefined
}

void
Coset::print (bool standard) const
{
  for (int x = 0; x < NGENS; x++)
    cout << setw (4) << (standard ? row[x] + 1 : row[x]);
}
