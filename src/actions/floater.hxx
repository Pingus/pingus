//  $Id: floater.hxx,v 1.16 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_FLOATER_HXX
#define HEADER_PINGUS_ACTIONS_FLOATER_HXX

#include "../pingu_action.hxx"

namespace Pingus {
namespace Actions {

class Floater : public PinguAction
{
private:
  int falling_depth;
  int step;
  CL_Sprite sprite;
public:
  Floater(Pingu* p);

  ActionName get_type() const { return Actions::Floater; }
  ActionType get_activation_mode() const { return FALL_TRIGGERED; }

  void init(void);

  void draw (DrawingContext& gc);
  void update();

  char get_persistent_char () { return 'f'; }
  bool change_allowed (ActionName new_action);

private:
  Floater (const Floater&);
  Floater& operator= (const Floater&);
};

} // namespace Actions
} // namespace Pingus

#endif

/* EOF */
