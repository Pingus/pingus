//  $Id: laser_kill.hxx,v 1.14 2003/04/19 10:23:18 torangan Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_LASER_KILL_HXX
#define HEADER_PINGUS_ACTIONS_LASER_KILL_HXX

#include "../sprite.hxx"
#include "../pingu_action.hxx"

namespace Actions {

/** This action is triggered by the LaserExit trap and causes the
    pingu to 'burn-away' */
class LaserKill : public PinguAction
{
private:
  Sprite sprite;

public:
  LaserKill (Pingu*);

  ActionName get_type () const { return Actions::Laserkill; }
  void init (void);

  void draw (GraphicContext& gc);
  void update ();

  bool catchable () { return false; }

private:
  LaserKill (const LaserKill&);
  LaserKill& operator= (const LaserKill&);
};

} // namespace Actions

#endif

/* EOF */
