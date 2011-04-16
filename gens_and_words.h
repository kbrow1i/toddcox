#ifndef GENS_AND_WORDS_H
#define GENS_AND_WORDS_H

#include <vector>
#include <string>

using namespace std;

const int NGENS = 4;
const int NOTAGEN = -1;
enum { a, A, b, B };		/* generators */
int inv (int x);		/* inverses */
typedef vector<int> word;

int char_to_gen (char c);
char gen_to_char (int x);
bool string_to_word (word& w, const string& s);



#endif	/* GENS_AND_WORDS_H */
