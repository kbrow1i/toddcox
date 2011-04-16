#include "coset.h"
#include "gens_and_words.h"

// Constructor
Coset::Coset ()
{
  for (int i = 0; i < NGENS; i++)
    row[i] = -1;		// all actions initially undefined
}
