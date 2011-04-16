#include "gens_and_words.h"

int
inv (int x)
{
  if (x == a)
    return A;
  if (x == A)
    return a;
  if (x == b)
    return B;
  if (x == B)
    return b;
  return NOTAGEN;
}

int
char_to_gen (char c)
{
  if (c == 'a')
    return a;
  if (c == 'A')
    return A;
  if (c == 'b')
    return b;
  if (c == 'B')
    return B;
  return NOTAGEN;
}

char
gen_to_char (int x)
{
  if (x == a)
    return 'a';
  if (x == A)
    return 'A';
  if (x == b)
    return 'b';
  if (x == B)
    return 'B';
}

bool
string_to_word (word& w, const string& s)
{
  for (int i = 0; i < s.size (); i++)
    {
      int x;
      if ((x = char_to_gen (s[i])) == NOTAGEN)
	return false;
      w.push_back (x);
    }
  return true;
}
