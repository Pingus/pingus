//  $Id: miner.hxx,v 1.14 2003/04/19 10:23:18 torangan Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_MINER_HXX
#define HEADER_PINGUS_ACTIONS_MINER_HXX

#include "../pingu_action.hxx"
#include "../sprite.hxx"

namespace Actions {

class Miner : public PinguAction
{
private:
  CL_Surface miner_radius;
  CL_Surface miner_radius_gfx;
  Sprite sprite;
  int slow_count;

  /** Width of the miner_radius surface */
  unsigned int miner_radius_width;

  /** Width of the miner_radius_gfx surface */
  unsigned int miner_radius_gfx_width;

public:
  Miner (Pingu* p);
  virtual ~Miner () {}

  ActionName get_type () const { return Actions::Miner; }

  void draw (GraphicContext& gc);
  void update ();

private:
  Miner (const Miner&);
  Miner& operator= (const Miner&);
};

} // namespace Actions

#endif

/* EOF */
