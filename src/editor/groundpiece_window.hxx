//  $Id: groundpiece_window.hxx,v 1.2 2002/06/29 11:54:22 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_GROUNDPIECE_WINDOW_HXX
#define HEADER_PINGUS_EDITOR_GROUNDPIECE_WINDOW_HXX

#include <ClanLib/gui.h>
#include "editor_groundpiece_obj.hxx"

namespace Pingus
{
  namespace Editor
  {
    class GroundpieceWindow : public CL_Window
    {
    private:
      GroundpieceData* data;

      CL_Button convert_to_hotspot_button;

      CL_Label  groundtype_label;
      CL_ListBox groundtype_listbox;

      CL_Button ok_button;
      CL_Button cancel_button;
      
      CL_Slot ok_button_slot;
      CL_Slot cancel_button_slot;
    public:
      GroundpieceWindow (CL_Component* parent,  EditorGroundpieceObj* data);
    };
  }
}

#endif

/* EOF */
