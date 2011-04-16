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


int
main (void)
{
  vector <word> gen_H;
  for (;;)
    {
      string s; word w;
      cout << "Enter a generator of H, or q when finished:\n";
      cin >> s;
      if (s == "Q" || s == "q")
	break;
      if (!string_to_word (w, s))
	{
	  cout << "Invalid entry: " << s << "\nUse alphabet a, A, b, B.\n";
	  continue;
	}
      gen_H.push_back (w);
    }
  const int NGENS_H = gen_H.size ();
  vector<word> rel;
  for (;;)
    {
      string s; word w;
      cout << "Enter a relator, or q when finished:\n";
      cin >> s;
      if (s == "Q" || s == "q")
	break;
      if (!string_to_word (w, s))
	{
	  cout << "Invalid entry: " << s << "\nUse alphabet a, A, b, B.\n";
	  continue;
	}
      rel.push_back (w);
    }
  const int NRELS = rel.size ();
  CosetTable C;
  for (int i = 0; i < NGENS_H; i++)
    C.scan_and_fill (0, gen_H[i]);
  for (int k = 0; k < C.get_size (); k++)
    {
      for (int i = 0; i < NRELS && C.is_alive (k); i++)
	C.scan_and_fill (k, rel[i]);
      if (C.is_alive (k))
	for (int x = 0; x < NGENS; x++)
	  if (!C.is_defined (k, x))
	    C.define (k, x);
    }
  C.print ();
  return 0;
}
