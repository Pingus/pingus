//  $Id: PingusGame.hh,v 1.3 2000/02/11 16:58:26 grumbel Exp $
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
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA. 

#ifndef PINGUSGAME_HH
#define PINGUSGAME_HH

#include <string>

#include "Client.hh"
#include "Server.hh"
#include "TrueServer.hh"
#include "Result.hh"

class PingusGame
{
private:
  Client* client;
  Server* server;
  Result  result;
public:
  PingusGame();
  ~PingusGame();

  std::string read_lastlevel_file();
  void   write_lastlevel_file(std::string levelfile);
  void   start(std::string plf_filename = "",
	       std::string psm_filename = "");
  Result get_result();
};

#endif

/* EOF */
