//  $Id: exiter.hxx,v 1.13 2003/04/18 17:08:56 grumbel Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_EXITER_HXX
#define HEADER_PINGUS_ACTIONS_EXITER_HXX

#include "../sprite.hxx"
#include "../pingu_action.hxx"

namespace Actions {

class Exiter : public PinguAction
{
private:
  Sprite sprite;
  bool sound_played;
  
public:
  Exiter(Pingu*);
  void init(void);
  ActionName get_type() const { return Actions::Exiter; }

  void draw (GraphicContext& gc);
  void update();
  
private:
  Exiter (const Exiter&);
  Exiter& operator= (const Exiter&);
};

} // namespace Actions

#endif

/* EOF */
