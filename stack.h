/* stack.h: declarations for the Stack class (for deduction stack); from Prata

   Copyright 2011 Kenneth S. Brown.

   This file is part of Toddcox.

   Toddcox is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version (GPLv3+).

   Toddcox is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Toddcox; if not, see <http://gnu.org/licenses/>.

   Written by Ken Brown <kbrown@cornell.edu>. */

#ifndef STACK_H_
#define STACK_H_

typedef struct
{
  int coset;
  int gen;
} deduction;

typedef deduction Item;

class Stack
{
private:
  enum {MAX = 1024};
  Item items[MAX];    // holds stack items
  int top;            // index just beyond end of stack
public:
  Stack () { top = 0; };
  bool is_empty () const { return top == 0; };
  bool is_full () const { return top == MAX; };
  // push() returns false if stack already is full, true otherwise
  bool push (const Item & item);
  // pop() returns false if stack already is empty, true otherwise
  bool pop (Item & item);          // pop top into item
  void erase () { top = 0; };
};

#endif	/* STACK_H */
