//  $Id: level_property_window.cxx,v 1.14 2003/10/18 23:17:27 grumbel Exp $
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

#include <iostream>
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

namespace Pingus {
namespace EditorNS {

LevelPropertyWindow::LevelPropertyWindow (CL_Component* parent, ObjectManager* manager_)
  : CL_Window (CL_Rect (0, 0, 400, 300), "Level Properties", parent),
    manager (manager_),
    levelname_label (CL_Rect (10, 10, 90, 30), "Levelname", get_client_area ()),
    levelname_input (CL_Rect(110, 10, 390, 30), manager->get_levelname (), get_client_area ()),

    descritpion_label (CL_Rect (10, 30, 90, 50), "Description", get_client_area ()),
    description_input(CL_Rect(110, 30, 390, 50), manager->get_description (), get_client_area ()),

    author_label (CL_Rect (10, 50, 90, 70), "Author", get_client_area ()),
    author_input (CL_Rect(110, 50, 390, 70), manager->get_author (), get_client_area ()),

    number_of_pingus_label (CL_Rect (10, 70, 110, 90), "Number of Pingus", get_client_area ()),
    number_of_pingus_input (CL_Rect(110, 70, 390, 90), to_string (manager->get_number_of_pingus ()),
			    get_client_area ()),

    pingus_to_save_label (CL_Rect (10, 90, 90, 110), "Pingus To Save", get_client_area ()),
    pingus_to_save_input (CL_Rect(110, 90, 390, 110), to_string (manager->get_number_to_save ()),
			  get_client_area ()),

    time_label (CL_Rect (10, 110, 90, 130), "Time", get_client_area ()),
    time_input (CL_Rect(110, 110, 390, 130), to_string (manager->get_leveltime ()),
		get_client_area ()),

    width_label (CL_Rect (10, 130, 90, 150), "Width", get_client_area ()),
    width_input (CL_Rect(110, 130, 390, 150), to_string(manager->get_width ()), get_client_area ()),

    height_label (CL_Rect (10, 150, 90, 170), "Height", get_client_area ()),
    height_input (CL_Rect(110, 150, 390, 170), to_string(manager->get_height ()), get_client_area ()),

    difficulty_label (CL_Rect (10, 170, 90, 190), "Difficulty", get_client_area ()),
    difficulty_input (CL_Rect(110, 170, 390, 190), to_string (manager->get_difficulty ()),
		      get_client_area ()),

    comment_label (CL_Rect(10, 190, 90, 210), "Comment", get_client_area ()),
    playable_checkbox (CL_Point(10, 210), "Playable", get_client_area ()),

    ok_button(CL_Rect(210, 240, 290, 260), "Ok", get_client_area()),
    cancel_button(CL_Rect(310, 240, 390, 260), "Cancel", get_client_area())
{
  playable_checkbox.set_checked (manager->get_playable ());

  ok_button_slot = ok_button.sig_clicked().connect(this, &LevelPropertyWindow::on_ok_click);
  cancel_button_slot = cancel_button.sig_clicked().connect(this, &LevelPropertyWindow::on_cancel_click);

  set_position(200, 100);
  show (false);
}

LevelPropertyWindow::~LevelPropertyWindow ()
{
  manager->set_playable (playable_checkbox.is_checked ());
}

void
LevelPropertyWindow::on_ok_click()
{
  write_data();
  show(false);
}

void
LevelPropertyWindow::on_cancel_click()
{
  show(false);
}

void
LevelPropertyWindow::show(bool show_window)
{
  if (show_window)
    read_data();

  CL_Window::show(show_window);
}

void
LevelPropertyWindow::write_data()
{
  manager->set_levelname (levelname_input.get_text());
  manager->set_description (description_input.get_text());

  manager->set_author (author_input.get_text());

  int number_of_pingus;
  if (from_string(number_of_pingus_input.get_text(), number_of_pingus))
    manager->set_number_of_pingus (number_of_pingus);

  int pingus_to_save;
  if (from_string(pingus_to_save_input.get_text(), pingus_to_save))
    manager->set_number_to_save (pingus_to_save);

  int time;
  if (from_string (time_input.get_text(), time))
    manager->set_leveltime (time);

  int width;
  if (from_string(width_input.get_text(), width))
    manager->set_width(width);

  int height;
  if (from_string(height_input.get_text(), height))
    manager->set_height(height);

  int difficulty = 40;
  if (from_string(difficulty_input.get_text(), difficulty))
    manager->set_difficulty(difficulty);

  manager->set_playable (playable_checkbox.is_checked());
}

void
LevelPropertyWindow::read_data()
{
  levelname_input.set_text(manager->get_levelname ());
  description_input.set_text(manager->get_description ());
  author_input.set_text(manager->get_author ());
  number_of_pingus_input.set_text(to_string (manager->get_number_of_pingus ()));
  pingus_to_save_input.set_text(to_string (manager->get_number_to_save ()));
  time_input.set_text(to_string (manager->get_leveltime ()));
  width_input.set_text(to_string(manager->get_width ()));
  height_input.set_text(to_string(manager->get_height ()));
  difficulty_input.set_text(to_string (manager->get_difficulty ()));
  playable_checkbox.set_checked(manager->get_playable ());
}

} // namespace EditorNS
} // namespace Pingus

/* EOF */
