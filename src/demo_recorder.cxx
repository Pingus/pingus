//  $Id: demo_recorder.cxx,v 1.4 2002/06/21 07:45:35 grumbel Exp $
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

#include <time.h>
#include "system.hxx"
#include "console.hxx"
#include "pingus_error.hxx"
#include "demo_recorder.hxx"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hxx"


using namespace std;

DemoRecorder::DemoRecorder() : is_recording(false)
{
}

DemoRecorder::~DemoRecorder()
{
}

void
DemoRecorder::set_levelname(const string& levelname)
{
  filename = System::get_statdir() + "demos/" + levelname + "-" + get_date() + ".pdm";

  //std::cout << "Demo filename: " + filename << std::endl;
  console << "Recording demo to: " << filename << std::endl;
  
  is_recording = true;

  std::cout << "DemoRecorder: levelname = " << levelname << std::endl;
  out.open(filename.c_str());

  out << levelname << std::endl;

  if (!out)
    throw PingusError(_("DemoRecorder: Couldn't open: ") + filename);
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
  strftime(buffer, 32, "%Y%m%d-%H%M%S", loctime);

  return string(buffer);
}

/* EOF */
