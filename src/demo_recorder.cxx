//  $Id: demo_recorder.cxx,v 1.20 2003/10/18 23:17:27 grumbel Exp $
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
#include <assert.h>
#include <iostream>
#include "system.hxx"
#include "demo_recorder.hxx"
#include "server.hxx"
#include "plf.hxx"
#include "gettext.h"

namespace Pingus {

DemoRecorder::DemoRecorder (Server* server)
  : record_demo (true)
{
  std::string levelname = server->get_plf()->get_resname();
  std::string flat_levelname = levelname;

  // 'Flatten' the levelname so that we don't need directories
  for (std::string::iterator i = flat_levelname.begin(); i != flat_levelname.end(); ++i)
    if (*i == '/')
      *i = '_';

  if (!levelname.empty())
    {
      std::string filename = System::get_statdir() + "demos/" + flat_levelname + "-" + get_date() + ".xml";
      out.open(filename.c_str());

      if (!out)
	{
          record_demo = false;
	  std::cout << "DemoRecorder: Error: Couldn't write DemoFile '" << filename
                    << "', demo recording will be disabled" << std::endl;
	}
      else
        {
          std::cout << "DemoRecorder: Writing demo to: " << filename << std::endl;
          record_demo = true;

          // Write file header
          out << "<pingus-demo>\n"
              << "  <level>" << levelname << "</level>\n"
              << "  <events>" << std::endl;
        }
    }
  else
    {
      record_demo = false;
      // This point should only be reachable if we have a bug
      // somewhere or provide a way to get a PLF without using XMLPLF,
      // since we don't do that, a bug must be somewhere when we reach
      // this.
      assert(!"ERROR: DemoRecorder: Couldn't get levelname, please report this as a bug!");
    }
}

DemoRecorder::~DemoRecorder ()
{
  if (record_demo)
    {
      // Write file footer
      out << "  </events>\n"
          << "</pingus-demo>" << std::endl;
      out.close();
    }
}

void
DemoRecorder::record_event (const ServerEvent& event)
{
  if (record_demo)
    {
      event.write_xml(out);
      //event.write_xml(std::cout);
    }
}

std::string
DemoRecorder::get_date ()
{
  char buffer[32];
  time_t curtime;
  struct tm *loctime;
  curtime = time (NULL);
  loctime = localtime(&curtime);
  strftime(buffer, 32, "%Y%m%d-%H%M%S", loctime);

  return std::string(buffer);
}

} // namespace Pingus

/* EOF */
