//  $Id: faller.hxx,v 1.17 2002/11/03 20:30:38 grumbel Exp $
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

#include "../sprite.hxx"
#include "../pingu_action.hxx"

namespace Actions {

class Faller : public PinguAction
{
private:
  Sprite faller;
  Sprite tumbler;

  int falling;

public:
  static const float deadly_velocity;

  Faller(Pingu*);
  virtual ~Faller();
  
  void  draw (GraphicContext& gc);
  void  update();

  bool change_allowed (Actions::ActionName new_action);
  
  std::string get_name() const { return "Faller"; }
  ActionName get_type() const { return Actions::Faller; }
  bool is_tumbling () const;
  
private:
  Faller (const Faller&);
  Faller& operator= (const Faller&);
};

} // namespace Actions

#endif

/* EOF */
