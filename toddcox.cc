// This is a toy implementation of the HLT version of the
// Todd--Coxeter procedure, based on Holt, Handbook of computational
// group theory.  It assumes two generators and uses the alphabet
// a,A,b,B.

#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

const int NGENS = 4;
const int NOTAGEN = -1;
enum { a, A, b, B };		// generators
int inv[NGENS] = {A, a, B, b }; // inverses
typedef vector<int> word;

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

class Coset
{
public:
  Coset ();
  int get_act (int x) const { return row[x]; };
  void set_act (int x, int k) { row[x] = k; return; } ;
private:
  int row[NGENS];		// row[x] is index of coset acted on by x
};

// Constructor
Coset::Coset ()
{
  for (int i = 0; i < NGENS; i++)
    row[i] = -1;		// all actions initially undefined
}

class CosetTable
{
public:
  CosetTable ();
  void define (int k, int x);
  bool is_defined (int k, int x) const { return (tab[k].get_act (x) >= 0); };
  void print () const;
  void debug_print () const;
  void scan_and_fill (int k, const word& w);
  bool is_alive (int k) const { return (p[k] == k); };
  int get_size () const { return tab.size (); };
private:
  vector<Coset> tab;
  vector<int> p;		// for generating equivalence classes; p[k] <= k
  queue<int> q;			// dead cosets to be processed
  int rep (int c);
  void merge (int k, int l);
  void coincidence(int k, int l);
};

CosetTable::CosetTable ()
{
  Coset c;
  tab.push_back (c);
  p.push_back (0);		// p[0] = 0
}

// Define coset k acted on by x to be new coset.
void
CosetTable::define (int k, int x)
{
  int l = tab.size ();		// index of new coset
  tab[k].set_act(x, l);
  Coset d;
  d.set_act(inv[x], k);
  tab.push_back (d);
  p.push_back (l);		// p[l] = l
}

void
CosetTable::print () const
{
  for (int k = 0; k < tab.size (); k++)
    if (is_alive (k))
	  {
	    cout << k + 1 << ":";	// Make coset numbering start at 1.
	    for (int x = 0; x < NGENS; x++)
	      cout << " " << tab[k].get_act (x) + 1;
	    cout << endl;
	  }
}

void
CosetTable::debug_print () const
{
  for (int k = 0; k < tab.size (); k++)
    {
      cout << k << ", " << p[k] << ": ";
      for (int x = 0; x < NGENS; x++)
	cout << " " << tab[k].get_act (x);
      cout << endl;
    }
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
CosetTable::coincidence (int k, int l)
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
	  tab[f].set_act(inv[x], -1);	// remove arrow f --> e
	  int e1 = rep (e);
	  int f1 = rep (f);
	  // insert arrows x: e1 --> f1 and y: f1 --> e1, where...
	  int y = inv[x];
	  if (is_defined (e1, x))
	    merge (f1, tab[e1].get_act (x));
	  else if (is_defined (f1, y))
	    merge (e1, tab[f1].get_act (y));
	  else
	    {
	      tab[e1].set_act (x, f1);
	      tab[f1].set_act (y, e1);
	    }
	}
    }
}

void
CosetTable::scan_and_fill (int k, const word& w)
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
	    {
	      cout << "Coset table before coincidence:\n";
	      debug_print ();
	      // print ();
	      cout << "\ncoincidence (" << f << ", " << b << ")\n\n";
	      coincidence (f, b);
	      cout << "Coset table after coincidence:\n";
	      debug_print ();
	      // print ();
	    }
	  return;
	}
      // Scan backward
      while (j >= i && is_defined (b, inv[w[j]]))
	b = tab[b].get_act (inv[w[j--]]);
      if (j < i)		// Scan completed with coincidence
	{
	  cout << "Coset table before coincidence:\n";
	  debug_print ();
	  // print ();
	  cout << "coincidence (" << f << ", " << b << ")\n";
	  coincidence (f, b);
	  cout << "Coset table after coincidence:\n";
	  debug_print ();
	  // print ();
	  return;
	}
      if (j == i)		// Scan completed with deduction
	{
	  cout << "deduction  " << gen_to_char (w[i]) << ":" << f << "-->" << b << endl;
	  tab[f].set_act (w[i], b);
	  tab[b].set_act (inv[w[i]], f);
	  return;
	}
      // Scan is incomplete; make a definition to allow it to get further.
      cout << "definition " << gen_to_char (w[i]) << ":" << f << "-->" << get_size () << endl;
      define (f, w[i]);
    }
}
		   
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
