//  $Id: Pingus.hh,v 1.2 2000/02/09 21:43:40 grumbel Exp $
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


#include "Intro.hh"

class PingusMain : public CL_ClanApplication
{
private:
  char*  title;
  string argv_0;
  bool    pingus_datadir_set;

  string  pingus_datafile;

  string  global_datafile;

  string  pingus_soundfile;
  string  levelfile;
  string resolution;
  
  int   level;  // the number of the current level, -1 if the level is custom  
  //  char* levelfile; // the name of the level_file
  int screen_width;
  int screen_height;

  // Functions
  void remove_comments(char*);
  bool line_empty(char*);

  Intro intro;

public:
  enum GameMode { Lemmings, Worms };

  PingusMain();
  virtual ~PingusMain();

  virtual int main(int argc, char** argv);
  virtual char* get_title();
  virtual void init_modules();
  virtual void deinit_modules();

 GameMode select_game_mode(void);
  bool do_lemmings_mode(void);
  bool do_worms_mode(void);
  void check_args(int argc, char* argv[]);
  void read_rc_file(void);
  void get_filenames(void);
  void init(int argc, char* argv[]);
  void init_clanlib(void);
  // void intro(void);
  void init_pingus(void);

  void play_level(string filename, PingusGameMode);
} my_app; // notice this line. It creates the global instance.

#endif

/* EOF */
