//  $Id: boarder.hxx,v 1.15 2003/10/18 23:17:27 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_ACTIONS_BOARDER_HXX
#define HEADER_PINGUS_ACTIONS_BOARDER_HXX

#include "../pingu_action.hxx"
#include "../state_sprite.hxx"

namespace Pingus {
namespace Actions {

/** The Boarder action causes a pingu to use a skateboard to move
    forward. */
class Boarder : public PinguAction
{
private:
  //double counter;
  double x_pos;
  double speed;
  StateSprite sprite;
public:
  Boarder (Pingu* p);

  ActionName get_type () const { return Actions::Boarder; }

  void  draw (SceneContext& gc);
  void  update ();

private:
  bool on_ground ();

  Boarder (const Boarder&);
  Boarder& operator= (const Boarder&);
};

} // namespace Actions
} // namespace Pingus

#endif

/* EOF */
