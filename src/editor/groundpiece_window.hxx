//  $Id: groundpiece_window.hxx,v 1.5 2002/07/01 09:09:31 grumbel Exp $
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

#include "property_frame.hxx"
#include "editor_groundpiece_obj.hxx"

namespace Pingus
{
  namespace Editor
  {
    class GroundpieceWindow : public PropertyFrame
    {
    private:
      EditorGroundpieceObj* data;

      CL_Button convert_to_hotspot_button;

      CL_Label  groundtype_label;
      //CL_ListBox groundtype_listbox;

      CL_RadioGroup groundtype_group;
      CL_RadioButton ground_radiobutton;
      CL_RadioButton transparent_radiobutton;
      CL_RadioButton solid_radiobutton;
      CL_RadioButton bridge_radiobutton;

      CL_Slot convert_to_hotspot_button_slot;

    public:
      GroundpieceWindow (CL_Component* parent,  EditorGroundpieceObj* data);
      ~GroundpieceWindow ();

      void convert_to_hotspot ();

      void read_data ();
      void write_data ();

      std::string get_title () { return "Groundpiece Properties"; }
    };
  }
}

#endif

/* EOF */
