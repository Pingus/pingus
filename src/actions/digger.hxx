//  $Id: digger.hxx,v 1.13 2002/10/20 18:28:49 torangan Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_DIGGER_HXX
#define HEADER_PINGUS_ACTIONS_DIGGER_HXX

#include "../pingu_action.hxx"
#include "../sprite.hxx"

namespace Actions {

class Digger : public PinguAction
{
private:
  CL_Surface digger_radius;
  CL_Surface digger_radius_gfx;
  Sprite sprite;
  int digger_c;

  /** Width of the digger_radius surface */
  unsigned int digger_radius_width;

  /** Height of the digger_radius surface */
  unsigned int digger_radius_height;

  /** Width of the digger_radius_gfx surface */
  unsigned int digger_radius_gfx_width;

  /** Height of the digger_radius_gfx surface */
  unsigned int digger_radius_gfx_height;

public:
  Digger(Pingu*);
    
  std::string get_name () const { return "Digger"; }
  ActionName get_type () const { return Actions::Digger; }
    
  bool have_something_to_dig ();
  void dig ();
    
  void draw (GraphicContext& gc);
  void update ();

private:
  Digger (const Digger&);
  Digger& operator= (const Digger&);
};

} // namespace Actions

#endif

/* EOF */
