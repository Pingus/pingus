//  $Id: demo_recorder.cxx,v 1.7 2002/10/03 12:33:08 grumbel Exp $
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
#include "server_event.hxx"
#include "server.hxx"
#include "plf.hxx"

/* Headers needed for i18n / gettext */
#include <clocale>
#include <config.h>
#include "my_gettext.hxx"

using namespace std;

DemoRecorder::DemoRecorder(Server* server)
{
  std::string levelname = server->get_plf()->get_filename();

  if (!levelname.empty())
    {
      std::string filename = System::get_statdir() + "demos/" + levelname + "-" + get_date() + ".xml";
      std::cout << "DemoRecorder: Writing demo to: " << filename << std::endl;
      out.open(filename.c_str());

      if (!out)
	{
	  assert(!"DemoRecorder: Couldn't write DemoFile");
	}
      
      // Write file header
      out << "<pingus-demo>\n"
	  << "  <level>" << levelname << "</level>\n"
	  << "  <events>" << std::endl;
    }
  else
    {
      assert(!"DemoRecorder Couldn't get levelname");
    }
}

DemoRecorder::~DemoRecorder()
{
  // Write file footer
  out << "  </events>\n"
      << "</pingus-demo>" << std::endl;
  out.close();
}

void
DemoRecorder::record_event (const ServerEvent& event)
{
  event.write_xml(out);  
  event.write_xml(std::cout);
}

string 
DemoRecorder::get_date()
{
  char buffer[32];
  time_t curtime;
  struct tm *loctime;
  curtime = time (NULL);
  loctime = localtime(&curtime);
  strftime(buffer, 32, "%Y-%m-%d_%H:%M:%S", loctime);

  return string(buffer);
}

/* EOF */
