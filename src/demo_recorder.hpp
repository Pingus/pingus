//  $Id: demo_recorder.hxx,v 1.10 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_DEMO_RECORDER_HXX
#define HEADER_PINGUS_DEMO_RECORDER_HXX

#include "pingus.hxx"
#include <fstream>
#include <string>


class Server;
class ServerEvent;

class DemoRecorder
{
private:
  /** Set to true to record a demo, set to false to ignore
      record_event() messages */
  bool record_demo;

  /** Stream to which the events are written */
  std::ofstream out;

  /** Filename to which the events are written */
  std::string   filename;

  std::string get_date();
public:
  DemoRecorder(Server* server);
  ~DemoRecorder();

  void record_event (const ServerEvent& event);

private:
  DemoRecorder (const DemoRecorder&);
  DemoRecorder& operator= (const DemoRecorder&);
};


#endif

/* EOF */
