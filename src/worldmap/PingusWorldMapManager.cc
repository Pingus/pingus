//  $Id: PingusWorldMapManager.cc,v 1.3 2000/09/20 14:31:11 grumbel Exp $
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

#include "../globals.hh"
#include "../algo.hh"
#include "PingusWorldMapManager.hh"

PingusWorldMapManager::PingusWorldMapManager ()
{
  is_init = false;
}

PingusWorldMapManager::~PingusWorldMapManager ()
{
}

void 
PingusWorldMapManager::init ()
{
  if (!is_init)
    {
      //sur = PingusResource::load_surface ("volcano", "worldmaps");
      is_init = true;
    }
}

void
PingusWorldMapManager::display ()
{
  on_button_press_slot   = CL_Input::sig_button_press.connect (thCreateSlot(this, &PingusWorldMapManager::on_button_press));
  on_button_release_slot = CL_Input::sig_button_release.connect (thCreateSlot(this, &PingusWorldMapManager::on_button_release));
  on_mouse_move_slot     = CL_Input::sig_mouse_move.connect (thCreateSlot(this, &PingusWorldMapManager::on_mouse_move));

  init ();

  worldmap = new PingusWorldMap (find_file (pingus_datadir, "worldmaps/volcano.xml"));

  while (true)
    {
      worldmap->draw ();
      CL_System::keep_alive ();
      CL_Display::flip_display ();
    }
}

void
PingusWorldMapManager::on_mouse_move (CL_InputDevice *, int mouse_x, int mouse_y)
{
  //  std::cout << "mouse: " << mouse_x << " " << mouse_y << std::endl;  
}

void 
PingusWorldMapManager::on_button_press (CL_InputDevice *device, const CL_Key &key)
{
  /*  std::cout << "key press: " << key.id << " " 
	    << CL_Mouse::get_x() << " "
	    << CL_Mouse::get_y() << std::endl;  */
  worldmap->on_button_press (device, key);
}

void 
PingusWorldMapManager::on_button_release (CL_InputDevice *device, const CL_Key &key)
{
  //  std::cout << "key release: " << key.id << std::endl;
}

void
PingusWorldMapManager::on_resize(int w, int h)
{
  std::cout << "Width: " << w << " Height: " << h << std::endl;
}

/* EOF */
