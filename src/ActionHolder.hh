//  $Id: ActionHolder.hh,v 1.2 2000/02/09 21:43:39 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef ACTIONHOLDER_HH
#define ACTIONHOLDER_HH

#include <string>
#include <map>

#include "PinguAction.hh"

struct ActionCounter {
  vector<PinguAction*> action;
  int   number;
};

class ActionHolder
{
private:
  vector<string>   action_name;
  map<string, ActionCounter> action;

  static vector<PinguAction*> uactions;
  static map<string, ActionCounter> uaction_buffer;

  static PinguAction* translate_action(const string&);
public:
  ActionHolder();
  ~ActionHolder();

  void add_action(const string& name, int available);
  void push_action(const string& name);
  int  get_available(const string&);
  PinguAction* get_action(const string&);

  static void init_uactions();
  static void clear_uactions();
  static PinguAction* get_uaction(const string&);
};

#endif

/* EOF */
