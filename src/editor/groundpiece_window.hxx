//  $Id: groundpiece_window.hxx,v 1.9 2002/09/27 11:26:45 torangan Exp $
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

#include <ClanLib/GUI/label.h>
#include <ClanLib/GUI/button.h>
#include <ClanLib/GUI/radiogroup.h>
#include <ClanLib/GUI/radiobutton.h>
#include "property_frame.hxx"

namespace EditorObjs {
class GroundpieceObj;
}

namespace EditorNS {

class GroundpieceWindow : public PropertyFrame
{
private:
  EditorObjs::GroundpieceObj* data;

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
  GroundpieceWindow (CL_Component* parent,  EditorObjs::GroundpieceObj* data);
  ~GroundpieceWindow ();

  void convert_to_hotspot ();

  void read_data ();
  void write_data ();

  std::string get_title () { return "Groundpiece Properties"; }
      
private:
  GroundpieceWindow (const GroundpieceWindow&);
  GroundpieceWindow& operator= (const GroundpieceWindow&);
};

} // namespace EditorNS

#endif

/* EOF */
