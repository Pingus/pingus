//  $Id: miner.hxx,v 1.5 2002/08/23 15:49:53 torangan Exp $
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

class Miner : public PinguAction
{
private:  
  CL_Surface miner_radius;
  CL_Surface miner_radius_gfx;
  Sprite sprite;
  int slow_count;
public:
  Miner ();
  virtual ~Miner () {}

  void init (void);
  std::string get_name () const { return "Miner"; }
  Pingus::Actions::ActionName get_type () const { return Pingus::Actions::Miner; }
  void update (float delta);
  void draw_offset (int x, int y, float s);
  
private:
  Miner (const Miner&);
  Miner operator= (const Miner&); 
};

#endif

/* EOF */
