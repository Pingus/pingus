//  $Id: level_property_window.cxx,v 1.3 2002/07/01 16:31:40 grumbel Exp $
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

#include "object_manager.hxx"
#include "../string_converter.hxx"
#include "level_property_window.hxx"

/******************
 Level Properties

 Levelname:   ...
 Description: ...
 Author: ........
 
 Pingus: 100
 ToSave: 50
  
 Time: .....?

 Width:  800
 Height: 600

 Difficulty: [   X  ]
 [x] Playable
 Comment: .......

 [  OK  ] [Cancel]
*********************/

using namespace Pingus::Editor;

LevelPropertyWindow::LevelPropertyWindow (CL_Component* parent, ObjectManager* manager)
  : CL_Window (CL_Rect (0, 0, 200, 300), "Level Properties", parent),
    levelname_label (CL_Rect (10, 10, 90, 30), "Levelname", get_client_area ()),
    levelname_input (CL_Rect(110, 10, 190, 30), "bla", get_client_area ()),

    descritpion_label (CL_Rect (10, 30, 90, 50), "Description", get_client_area ()),
    //description_input(CL_Rect(110, 30, 190, 50), "descrip", get_client_area ()),

    author_label (CL_Rect (10, 50, 90, 70), "Author", get_client_area ()),
    author_input (CL_Rect(110, 50, 190, 70), "me", get_client_area ()),

    number_of_pingus_label (CL_Rect (10, 70, 90, 90), "Number of Pingus", get_client_area ()),
    number_of_pingus_input (CL_Rect(110, 70, 190, 90), to_string (10), get_client_area ()),

    pingus_to_save_label (CL_Rect (10, 90, 90, 110), "Pingus To Save", get_client_area ()),
    pingus_to_save_input (CL_Rect(110, 90, 190, 110), to_string (10), get_client_area ()),

    time_label (CL_Rect (10, 110, 90, 130), "Time", get_client_area ()),
    time_input (CL_Rect(110, 110, 190, 130), to_string (-1), get_client_area ()),

    width_label (CL_Rect (10, 130, 90, 150), "Width", get_client_area ()),
    width_input (CL_Rect(110, 130, 190, 150), to_string(manager->get_width ()), get_client_area ()),    

    height_label (CL_Rect (10, 150, 90, 170), "Height", get_client_area ()),
    height_input (CL_Rect(110, 150, 190, 170), to_string(manager->get_height ()), get_client_area ()), 

    difficulty_label (CL_Rect (10, 170, 90, 190), "Difficulty", get_client_area ()),
    difficulty_input (CL_Rect(110, 170, 190, 190), to_string (50), get_client_area ()),

    comment_label (CL_Rect(10, 190, 90, 210), "Comment", get_client_area ()),
    playable_checkbox (CL_Point(10, 210), "Playable", get_client_area ())
{  
  show (false);
}

LevelPropertyWindow::~LevelPropertyWindow ()
{
  
}

/* EOF */
