//  $Id: DemoRecorder.cc,v 1.2 2000/06/14 21:45:55 grumbel Exp $
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

#include <ctime>
#include "System.hh"
#include "Console.hh"
#include "PingusError.hh"
#include "DemoRecorder.hh"

DemoRecorder::DemoRecorder()
{
  is_recording = false;
}

DemoRecorder::~DemoRecorder()
{
}

void
DemoRecorder::set_levelname(const string& levelname)
{
  filename = System::get_statdir() + "demos/" + levelname + "-" + get_date() + ".plt";

  //std::cout << "Demo filename: " + filename << std::endl;
  console << "Recording demo to: " << filename << Console::endl;
  
  is_recording = true;

  out.open(filename.c_str());

  out << levelname << std::endl;

  if (!out)
    throw PingusError("DemoRecorder: Couldn't open: " + filename);
}

void
DemoRecorder::queue_event(const string& event)
{
  if (is_recording) {
    out << event << std::endl;
    std::cout << "Recorded: " << event << std::endl;
  }
}

string 
DemoRecorder::get_date()
{
  char buffer[32];
  time_t curtime;
  struct tm *loctime;
  curtime = time (NULL);
  loctime = localtime(&curtime);
  strftime(buffer, 32, "%H_%M_%S-%m_%d_%Y", loctime);

  return string(buffer);
}

/* EOF */
