//  $Id: splashed.hxx,v 1.13 2002/10/13 20:25:00 torangan Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_SPLASHED_HXX
#define HEADER_PINGUS_ACTIONS_SPLASHED_HXX

#include "../sprite.hxx"
#include "../pingu_action.hxx"

namespace Actions {

class Splashed : public PinguAction
{
private:
  bool particle_thrown;
  bool sound_played;
  Sprite sprite;
public:
  Splashed (Pingu*);
  
  std::string get_name () const { return "Splashed"; }
  ActionName get_type () const { return Actions::Splashed; }
  
  void draw (GraphicContext& gc);
  void update ();

  bool catchable () { return false; }
  bool change_allowed (Actions::ActionName) { return false; }
  
private:
  Splashed (const Splashed&);
  Splashed& operator= (const Splashed&);
};

} // namespace Actions

#endif

/* EOF */
