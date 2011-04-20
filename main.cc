// This is a toy implementation of the HLT version of the
// Todd--Coxeter procedure, based on Holt, Handbook of computational
// group theory.  It assumes two generators and uses the alphabet
// a,A,b,B.

#include "cosettable.h"
#include "gens_and_words.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void getgroup (vector<word>& rel, vector<word>& gen_H);

int
main (void)
{
  cout << "\nThis program uses the HLT version of the Todd-Coxeter procedure\n"
       << "to compute the index in a 2-generator group G of a subgroup H.\n"
       << "You will be prompted to enter the defining relators of G and\n"
       << "the generators of H.  Use a,b for the generators and A,B for\n"
       << "their inverses.\n\n";
  vector<word> rel;
  vector<word> gen_H;
  getgroup (rel, gen_H);
  int threshold;
  cout << "Enter threshold for HLT with lookahead, or 0 to use ordinary HLT: ";
  cin >> threshold;
  CosetTable C (rel, gen_H);
  bool res = true;
  if (threshold == 0)
    C.hlt ();
  else
    res = C.hlt_plus (threshold);
  if (!res)
    {
      cout << "Sorry, please try again with a bigger threshold.\n\n";
      return 1;
    }
  cout << "\nThe index of H in G is " << C.get_nlive ()
       << ".\nThe coset table has size " << C.get_size () << " before compression.\n\n"
       << "Compressed and standardized coset table:\n\n";
  C.compress ();
  C.standardize ();
  C.print ();
  return 0;
}

void
getgroup (vector<word>& rel, vector<word>& gen_H)
{
  for (;;)
    {
      string s; word w;
      cout << "Enter a defining relator for G, or q when finished:\n> ";
      cin >> s;
      if (s == "Q" || s == "q")
	break;
      if (!string_to_word (w, s))
	{
	  cout << "Invalid entry: " << s << "\nUse alphabet a, A, b, B.\n> ";
	  continue;
	}
      rel.push_back (w);
    }
  for (;;)
    {
      string s; word w;
      cout << "Enter a generator of H, or q when finished:\n> ";
      cin >> s;
      if (s == "Q" || s == "q")
	break;
      if (!string_to_word (w, s))
	{
	  cout << "Invalid entry: " << s << "\nUse alphabet a, A, b, B.\n> ";
	  continue;
	}
      gen_H.push_back (w);
    }
}
