//  $Id: faller.hxx,v 1.5 2002/06/26 19:13:13 grumbel Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_FALLER_HXX
#define HEADER_PINGUS_ACTIONS_FALLER_HXX

#include "../pingu_action.hxx"


class Faller : public PinguAction
{
private:
  Sprite faller;
  Sprite tumbler;

  int falling;

  static const float deadly_velocity;

public:
  Faller();
  virtual ~Faller();
  
  void  init(void);
  
  void  update(float delta);
  void  draw_offset(int x, int y, float s);

  bool change_allowed (const std::string&);
  
  std::string get_name() const { return "Faller"; }
  bool is_tumbling () const;
};

#endif

/* EOF */
