// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_PINGUS_MAIN_HPP
#define HEADER_PINGUS_PINGUS_PINGUS_MAIN_HPP

#include "pingus/options.hpp"

void segfault_handler(int);

class PingusMain
{
private:
  CommandLineOptions cmd_options;

public:
  PingusMain();
  virtual ~PingusMain();

  virtual int run(int argc, char** argv);

private:
  /** After all subsystems have been inited, the screen will get
      setup, the game (the menu or a level, depending on
      command line flags) will start and the user will get
      controll. */
  void start_game();

  void parse_args(int argc, char** argv);
  void apply_args();
  void read_rc_file(void);

  void print_greeting_message();

  void init_path_finder();

  PingusMain (const PingusMain&);
  PingusMain& operator= (const PingusMain&);
};

#endif

/* EOF */
