//  $Id: DemoPlayer.cc,v 1.8 2002/06/10 13:03:34 torangan Exp $
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
#include <fstream>
#include "globals.hh"
#include "algo.hh"
#include "PingusError.hh"
#include "DemoPlayer.hh"
#include "my_gettext.hh"

using namespace std;

DemoPlayer::DemoPlayer()
{
}

DemoPlayer::~DemoPlayer()
{
}

std::string 
DemoPlayer::get_levelname()
{
  return levelname;
}

void
DemoPlayer::load(const std::string& arg_filename)
{
  const int buffer_size = 256;
  char buffer[buffer_size];
  std::string filename;
  ifstream in;
  
  filename = arg_filename;

  in.open(filename.c_str());

  if (!in)
    throw PingusError(_("DemoPlayer: Couldn't load ") + filename);

  in >> levelname; 
  in.get(); // \n ueberlesen
  std::cout << "DemoPlayer: LevelName=" << levelname << std::endl;

  while(!(in.getline(buffer, buffer_size).eof()))
    {
      cout << "Readline: " << buffer << endl;
      event_queue.push(PingusEvent(buffer));
      std::cout << "Parsed: " << event_queue.back().game_time << ":" << event_queue.back().str << std::endl;
    }
}

const PingusEvent& 
DemoPlayer::peek_event()
{
  return event_queue.front();
}

void
DemoPlayer::dequeue_event()
{
  event_queue.pop();
}

/* EOF */
