//  $Id: PingusMain.hh,v 1.2 2000/08/10 15:13:07 grumbel Exp $
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

#ifndef PINGUS_HH
#define PINGUS_HH

#include <string>
#include <ClanLib/core.h>

#include "GenericMain.hh"
#include "Intro.hh"

///
void segfault_handler(int);

///
class PingusMain : public AbstractMain
{
private:
  ///
  bool    pingus_datadir_set;
  ///
  bool    no_config_file;

  ///
  std::string pingus_datafile;
  /// 
  std::string global_datafile;
  ///
  std::string pingus_soundfile;
  ///
  std::string levelfile;
  ///
  std::string resolution;
  
  ///
  Intro intro;

  ///
  thSlot on_button_press_slot;
  ///
  thSlot on_button_release_slot;

public:
  ///
  enum GameMode { Lemmings, Worms };

  ///
  PingusMain();
  ///
  virtual ~PingusMain();

  ///
  virtual int main(int argc, char** argv);
  ///
  virtual char* get_title();
  ///
  virtual void init_modules();
  ///
  virtual void deinit_modules();

  ///
  GameMode select_game_mode(void);
  ///
  void load_resources(std::string filename);
  ///
  bool do_lemmings_mode(void);
  ///
  bool do_worms_mode(void);

  ///
  void check_args(int argc, char* argv[]);
  ///
  void quick_check_args(int argc, char* argv[]);
  ///
  void read_rc_file(void);

  ///
  void get_filenames(void);
  ///
  void init(int argc, char* argv[]);
  ///
  void init_clanlib(void);
  /// void intro(void);
  void init_pingus(void);
};


#endif

/* EOF */
