//  $Id: groundpiece_window.cxx,v 1.5 2002/07/01 09:09:31 grumbel Exp $
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

#include "groundpiece_window.hxx"

/************************
 Groundpiece Properties
 ------------------------
 Type: [ground|bridger|transparent|...]

 [convert to hotspot]
 ------------------------
 [ok] [cancel]
*************************/

using namespace Pingus::Editor;

GroundpieceWindow::GroundpieceWindow (CL_Component* parent,  EditorGroundpieceObj* arg_data)
  : PropertyFrame (200, 120, parent),
    data (arg_data),
    convert_to_hotspot_button (CL_Rect (10, 90, 190, 90 + 20), "Convert to HotSpot", this),

    groundtype_label (CL_Rect (10, 10, 90, 10 + 20), "Groundtype", this),
    //groundtype_listbox (CL_Rect (110, 60, 190, 60 + 60), this),

    ground_radiobutton (CL_Point (110, 10), "ground", this),
    transparent_radiobutton (CL_Point (110, 30), "transparent", this),
    solid_radiobutton (CL_Point (110, 50), "solid", this),
    bridge_radiobutton (CL_Point (110, 70), "bridge", this)
{
  groundtype_group.add (&ground_radiobutton);
  groundtype_group.add (&transparent_radiobutton);
  groundtype_group.add (&solid_radiobutton);
  groundtype_group.add (&bridge_radiobutton);

  convert_to_hotspot_button_slot
    = convert_to_hotspot_button.sig_clicked ().connect (this, &GroundpieceWindow::convert_to_hotspot);

  read_data ();
}

GroundpieceWindow::~GroundpieceWindow ()
{
  write_data ();
}

void
GroundpieceWindow::convert_to_hotspot ()
{
  // not implemented
}

void
GroundpieceWindow::read_data ()
{
  switch (data->gptype)
    {
    case GroundpieceData::GP_SOLID:
      solid_radiobutton.set_checked (true);
      break;
    case GroundpieceData::GP_BRIDGE:
      bridge_radiobutton.set_checked (true);
      break;
    case GroundpieceData::GP_TRANSPARENT:
      transparent_radiobutton.set_checked (true);
      break;
    case GroundpieceData::GP_GROUND:
      ground_radiobutton.set_checked (true);
      break;
    default:
      std::cout << "Unhandled" << std::endl;
      break;
    }
}

void
GroundpieceWindow::write_data ()
{
  if (ground_radiobutton.is_checked ())
    data->gptype = GroundpieceData::GP_GROUND;
  else if (transparent_radiobutton.is_checked ())
    data->gptype = GroundpieceData::GP_TRANSPARENT;
  else if (solid_radiobutton.is_checked ())
    data->gptype = GroundpieceData::GP_SOLID;
  else if (bridge_radiobutton.is_checked ())
    data->gptype = GroundpieceData::GP_BRIDGE;
  else
    {
      std::cout << "Unhandled" << std::endl;
    }
}

/* EOF */
