//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "math.hpp"
#include "server.hpp"
#include "pathname.hpp"
#include "world.hpp"
#include "components/pingus_counter.hpp"
#include "gui/gui_manager.hpp"
#include "display/scene_context.hpp"
#include "pingus_demo.hpp"
#include "components/playfield.hpp"
#include "components/smallmap.hpp"
#include "screen/screen_manager.hpp"
#include "display/display.hpp"
#include "demo_session.hpp"

DemoSession::DemoSession(const Pathname& pathname)
  : pause(false),
    fast_forward(false)
{
  // Load Demo file
  demo = std::auto_ptr<PingusDemo>(new PingusDemo(pathname));

  events = demo->get_events();
  // Reverse the vector so that we can use pop_back()  
  std::reverse(events.begin(), events.end());

  // Create server
  server   = std::auto_ptr<Server>(new Server(PingusLevel(Pathname(demo->get_levelname(), Pathname::DATA_PATH))));

  // Create GUI
  pcounter = new PingusCounter(server.get());
  gui_manager->add(pcounter, true);

  int world_width  = server->get_world()->get_width();
  int world_height = server->get_world()->get_height();

  playfield = new Playfield(server.get(), 0,
                            Rect(Vector2i(Math::max((Display::get_width()  - world_width)/2,  0),
                                          Math::max((Display::get_height() - world_height)/2, 0)), 
                                 Size(Math::min(Display::get_width(),  world_width),
                                      Math::min(Display::get_height(), world_height))));

  gui_manager->add(playfield, true);

  small_map    = new SmallMap(server.get(), playfield);
  gui_manager->add(small_map, true);
}

DemoSession::~DemoSession()
{
}

/** Pass a delta to the screen */
void
DemoSession::update(float delta)
{
  UNUSED_ARG(delta);

  if (server->is_finished())
    {
      ScreenManager::instance()->pop_screen();
    }
  else
    {
      // FIXME: Duplicate all timing code here?!

      if (!pause)
        {
          if (fast_forward)
            {
              for (int i = 0; i < 4; ++i)
                {
                  server->update();
                  update_demo();
                }
            }
          else
            {
              server->update();
              update_demo();
            }
        }
    }
}

void
DemoSession::update_demo()
{
  while(!events.empty() && events.back().time_stamp == server->get_time())
    {
      ServerEvent& event = events.back();

      if (0)
        {
          std::cout << "Sending: ";
          event.write(std::cout);
        }

      event.send(server.get());
      events.pop_back();
    }
  
  // Check for unexpected things (might happen if the demo file is broken)
  if (!events.empty() && events.back().time_stamp < server->get_time())
    {
      std::cout << "DemoPlayer Bug: We missed a timestamp: " << events.back().time_stamp << std::endl;
    }
}

void
DemoSession::on_pause_press()
{
  if (0)
    {
      for(std::vector<ServerEvent>::iterator i = events.begin(); i != events.end(); ++i)
        {
          std::cout << "Event: ";
          i->write(std::cout);      
        }
    }

  pause = !pause;

}

void
DemoSession::on_fast_forward_press()
{
  if (0)
    std::cout << "Fast Forward Pressed: " << events.size() << " " << server->get_time() << std::endl;

  fast_forward = !fast_forward;
}

void
DemoSession::on_escape_press()
{
  std::cout << "Escape Pressed" << std::endl;
  server->send_finish_event();
  ScreenManager::instance()->pop_screen();
}

void
DemoSession::process_scroll_event(const Input::ScrollEvent& ev)
{
  // FIXME: Rounding considered evil?
  playfield->scroll(static_cast<int>(-ev.x_delta),
                    static_cast<int>(-ev.y_delta));
}

/* EOF */
