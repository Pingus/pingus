//  $Id: soundtest.cxx,v 1.5 2003/03/04 19:22:19 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "path_manager.hxx"
#include "globals.hxx"
#include "sound/sound.hxx"
#include "soundtest.hxx"

Soundtest::Soundtest()
{
  
}

int
Soundtest::main(int argc, char** argv)
{
  std::cout << "Pingus Sound Test Application\n" << std::endl;

  std::cout << "1-9 to play some music\n"
            << " 0  to stop the music\n"
            << "right or left mouse click will cause a sound effect to be played\n"
            <<" panning will be according to the current mouse position\n\n" << std::endl;

  sound_enabled      = true;
  music_enabled      = true;
  pingus_debug_flags = ~0;

  CL_SetupCore::init ();
  CL_SetupDisplay::init();

  CL_Display::set_videomode(400, 400, false, false);

  path_manager.add_path ("../../data/");
  path_manager.find_path("music/pingus-1.it");

  PingusSound::init();

  PingusSound::play_music("pingus-1.it");

  bool left_down = false;
  while (true)
    {
      if (CL_Keyboard::get_keycode(CL_KEY_1))
        PingusSound::play_music("pingus-1.it");
      else if (CL_Keyboard::get_keycode(CL_KEY_2))
        PingusSound::play_music("pingus-2.it");
      else if (CL_Keyboard::get_keycode(CL_KEY_3))
        PingusSound::play_music("pingus-3.it");
      else if (CL_Keyboard::get_keycode(CL_KEY_4))
        PingusSound::play_music("pingus-5.it");
      else if (CL_Keyboard::get_keycode(CL_KEY_5))
        PingusSound::play_music("pingus-5.it");
      else if (CL_Keyboard::get_keycode(CL_KEY_6))
        PingusSound::play_music("pingus-6.it");      
      else if (CL_Keyboard::get_keycode(CL_KEY_0))
        PingusSound::stop_music();
      
      if (CL_Mouse::left_pressed() && !left_down)
        {
          left_down = true;
          float panning = (CL_Mouse::get_x()-200)/200.0f;
          std::cout << "Panning: " << panning << std::endl;
          PingusSound::play_sound("ohno", CL_Mouse::get_y()/400.0f,
                                  panning);
        }
      else if (!CL_Mouse::left_pressed())
        {
          left_down = false;
        }
        

      if (CL_Mouse::right_pressed())
        {
          float panning = (CL_Mouse::get_x()-200)/200.0f;
          std::cout << "Panning: " << panning << std::endl;
          PingusSound::play_sound("chink", CL_Mouse::get_y()/400.0f,
                                  panning);
        }

      CL_Display::clear_display();
      CL_Display::flip_display();
      CL_System::keep_alive();
      CL_System::sleep(20);
    }
}

/* EOF */
