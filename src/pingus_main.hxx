//  $Id: pingus_main.hxx,v 1.6 2002/11/02 19:03:40 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_PINGUS_MAIN_HXX
#define HEADER_PINGUS_PINGUS_MAIN_HXX

#include <string>
#include <ClanLib/Signals/slot.h>
#include <ClanLib/application.h>
#include "pingus.hxx"

void segfault_handler(int);

class PingusMain : public CL_ClanApplication
{
private:
  bool    no_config_file;
  /// the name of the exe: argv[0]
  std::string executable_name;
  std::string levelfile;
  std::string resolution;

  CL_Slot on_button_press_slot;
  CL_Slot on_button_release_slot;

public:
  PingusMain();
  virtual ~PingusMain();

  virtual int main(int argc, char** argv);
  virtual char* get_title();

private:
  /** After all subsystems have been inited, the screen will get
      setup, the game (the menu, the editor or a level, depending on
      command line flags) will start and the user will get
      controll. */
  void start_game();

  void check_args(int argc, char** argv);
  void quick_check_args(int argc, char** argv);
  void read_rc_file(void);

  void init_clanlib();
  void deinit_clanlib();

  void init_pingus();
  void init_path_finder();
  void deinit_pingus();
  
  PingusMain (const PingusMain&);
  PingusMain& operator= (const PingusMain&);

} app; // golbal class instance

#endif

/* EOF */



