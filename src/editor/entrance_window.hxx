//  $Id: entrance_window.hxx,v 1.5 2002/08/23 15:49:54 torangan Exp $
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

#ifndef HEADER_PINGUS_EDITOR_ENTRANCE_WINDOW_HXX
#define HEADER_PINGUS_EDITOR_ENTRANCE_WINDOW_HXX

#include <ClanLib/GUI/label.h>
#include <ClanLib/GUI/inputbox.h>
#include <ClanLib/GUI/radiogroup.h>
#include <ClanLib/GUI/radiobutton.h>
#include "property_frame.hxx"

class EntranceObj;

namespace Pingus
{
  namespace Editor
  {
    class EntranceWindow : public PropertyFrame
    {
    private:
      EntranceObj* entrance;

      CL_Label       direction_label;
      CL_RadioGroup  direction_group;
      CL_RadioButton direction_left;
      CL_RadioButton direction_misc;
      CL_RadioButton direction_right;

      CL_Label       release_rate_label;
      CL_InputBox    release_rate_input;

    public:
      EntranceWindow (CL_Component* parent, EntranceObj* );
      ~EntranceWindow ();

      std::string get_title () { return "Entrance Properties"; }
      
    private:
      EntranceWindow (const EntranceWindow&);
      EntranceWindow operator= (const EntranceWindow&);
    };
  }
}

#endif

/* EOF */
