/* stack.cc: Stack class (for deduction stack); from Prata

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

#include "stack.h"

bool
Stack::push(const Item & item)   
{
  if (top < MAX)
    {
      items[top++] = item;
      return true;
    }
  return false;
}

bool
Stack::pop(Item & item)
{
  if (top > 0)
    {
      item = items[--top];
      return true;
    }
  return false; 
}
