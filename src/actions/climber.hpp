//  $Id: climber.hxx,v 1.16 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_CLIMBER_HXX
#define HEADER_PINGUS_ACTIONS_CLIMBER_HXX

#include "../state_sprite.hpp"
#include "../pingu_action.hpp"

namespace Actions {

class Climber : public PinguAction
{
private:
  StateSprite sprite;
  int sprite_width;
  int sprite_height;

public:
  Climber (Pingu*);

  ActionName get_type () const { return Actions::Climber; }
  ActionType get_activation_mode () const { return WALL_TRIGGERED; }

  void draw (SceneContext& gc);

  void update ();

  char get_persistent_char () { return 'c'; }
  bool change_allowed(ActionName new_action);

private:
  Climber (const Climber&);
  Climber& operator= (const Climber&);
};

} // namespace Actions

#endif

/* EOF */
