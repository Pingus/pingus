//  $Id: groundpiece_window.cxx,v 1.3 2002/06/30 22:03:13 grumbel Exp $
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
 [convert to hotspot]

 Type: [ground|bridger|transparent|...]
 ------------------------
 [ok] [cancel]
*************************/

using namespace Pingus::Editor;

GroundpieceWindow::GroundpieceWindow (CL_Component* parent,  EditorGroundpieceObj* arg_data)
  : CL_Frame (CL_Rect (0, 0, 200, 180), /* "Groundpiece Properties",*/ parent),
    data (arg_data),
    convert_to_hotspot_button (CL_Rect (10, 30, 190, 30 + 20), "Convert to HotSpot", this),

    groundtype_label (CL_Rect (10, 60, 90, 60 + 20), "Groundtype", this),
    //groundtype_listbox (CL_Rect (110, 60, 190, 60 + 60), this),

    ground_radiobutton (CL_Point (110, 60), "ground", this),
    transparent_radiobutton (CL_Point (110, 80), "transparent", this),
    solid_radiobutton (CL_Point (110, 100), "solid", this),
    bridge_radiobutton (CL_Point (110, 120), "bridge", this),

    ok_button (CL_Rect (10, 150, 90, 150 + 20), "Ok", this),
    cancel_button (CL_Rect (110, 150, 190, 150 + 20), "Cancel", this)
{
  //groundtype_listbox.insert_item ("ground");
  //groundtype_listbox.insert_item ("transparent");
  //groundtype_listbox.insert_item ("bridge");
  //groundtype_listbox.insert_item ("solid");
  groundtype_group.add (&ground_radiobutton);
  groundtype_group.add (&transparent_radiobutton);
  groundtype_group.add (&solid_radiobutton);
  groundtype_group.add (&bridge_radiobutton);

  ok_button_slot     = ok_button.sig_clicked ().connect (this, &GroundpieceWindow::ok_clicked);
  cancel_button_slot = cancel_button.sig_clicked ().connect (this, &GroundpieceWindow::cancel_clicked);

  convert_to_hotspot_button_slot
    = convert_to_hotspot_button.sig_clicked ().connect (this, &GroundpieceWindow::convert_to_hotspot);

  read_data ();
}

void 
GroundpieceWindow::ok_clicked ()
{
  std::cout << "OK Clicked" << std::endl;
  write_data ();
  show (false);
}

void
GroundpieceWindow::cancel_clicked ()
{
  std::cout << "Cancel Clicked" << std::endl;
  show (false);
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
