//  $Id: groundpiece_window.cxx,v 1.2 2002/06/29 11:54:22 grumbel Exp $
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

GroundpieceWindow::GroundpieceWindow (CL_Component* parent,  EditorGroundpieceObj* data)
  : CL_Window (CL_Rect (0, 0, 200, 300), "Groundpiece Properties", parent),
    convert_to_hotspot_button (CL_Rect (10, 30, 190, 30 + 20), "Convert to HotSpot", this),

    groundtype_label (CL_Rect (10, 60, 90, 60 + 20), "Groundtype", this),
    groundtype_listbox (CL_Rect (110, 60, 190, 60 + 60), this),

    ok_button (CL_Rect (10, 150, 90, 150 + 20), "Ok", this),
    cancel_button (CL_Rect (110, 150, 190, 150 + 20), "Cancel", this)
{
  groundtype_listbox.insert_item ("ground");
  groundtype_listbox.insert_item ("transparent");
  groundtype_listbox.insert_item ("bridge");
  groundtype_listbox.insert_item ("solid");
}

/* EOF */
