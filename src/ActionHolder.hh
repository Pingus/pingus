//  $Id: ActionHolder.hh,v 1.7 2000/05/25 17:16:21 grumbel Exp $
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

#include "actions/basher.hh"
#include "actions/blocker.hh"
#include "actions/bomber.hh"
#include "actions/bridger.hh"
#include "actions/climber.hh"
#include "actions/digger.hh"
#include "actions/exiter.hh"
#include "actions/floater.hh"
#include "actions/miner.hh"
#include "actions/Jumper.hh"
#include "actions/teleported.hh"
#include "actions/exiter.hh"
#include "actions/smashed.hh"
#include "actions/LaserKill.hh"
#include "actions/Splashed.hh"

#include "PinguAction.hh"

struct ActionCounter {
  std::vector<PinguAction*> action;
  int   number;
};

class ActionHolder
{
private:
  std::vector<std::string>   action_name;
  std::map<std::string, ActionCounter> action;
  
  static std::vector<Basher>   bashers;
  static std::vector<Blocker>  blockers;
  static std::vector<Bomber>   bombers;
  static std::vector<Bridger> bridgers;
  static std::vector<Climber>  climbers;

  static std::vector<PinguAction*> uactions;
  static std::map<std::string, ActionCounter> uaction_buffer;

  static PinguAction* translate_action(const std::string&);
public:
  ActionHolder();
  ~ActionHolder();

  void add_action(const std::string& name, int available);
  void push_action(const std::string& name);
  int  get_available(const std::string&);
  PinguAction* get_action(const std::string&);

  static void init_uactions();
  static void clear_uactions();
  static PinguAction* get_uaction(const std::string&);
};

#endif

/* EOF */
