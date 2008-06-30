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
#include "demo_player.hpp"
#include "components/pingus_counter.hpp"
#include "gui/gui_manager.hpp"
#include "display/scene_context.hpp"
#include "pingus_demo.hpp"
#include "demo_session.hpp"

DemoSession::DemoSession(const Pathname& pathname)
{
  // Load Demo file
  demo = std::auto_ptr<PingusDemo>(new PingusDemo(pathname));

  // Create server
  server   = std::auto_ptr<Server>(new Server(PingusLevel(Pathname(demo->get_levelname(), Pathname::DATA_PATH))));

  // Create GUI
  pcounter = new PingusCounter(server.get());
  gui_manager->add(pcounter, true);
}

DemoSession::~DemoSession()
{
}

/** Draw this screen */
void
DemoSession::draw_background(DrawingContext& gc)
{
#if 0
  World* world = server->get_world();
  
  SceneContext* sc = new SceneContext();


  if (CL_Keyboard::get_keycode(CL_KEY_LEFT))
    sc->translate(10.0, 0.0);

  if(CL_Keyboard::get_keycode(CL_KEY_RIGHT))
    sc->translate(-10.0, 0.0);

  if(CL_Keyboard::get_keycode(CL_KEY_UP))
    sc->translate(0.0, 10.0);

  if(CL_Keyboard::get_keycode(CL_KEY_DOWN))
    sc->translate(0.0, -10.0);
#endif 

#if 0 // FIXME
  float x_of = -sc->get_x_offset();
  float y_of = -sc->get_y_offset();

  x_of = Math::mid(sc->get_width()/2.0f,
                   x_of,
                   world->get_width() - 1 - sc->get_width()/2.0f);

  y_of = Math::mid(sc->get_height()/2.0f,
                   y_of,
                   world->get_height() - 1 - sc->get_height()/2.0f);

  sc->set_offset(-x_of, -y_of);


  world->draw(*sc);
  while (CL_Keyboard::get_keycode(CL_KEY_D))
    {
      world->draw(*sc);
      CL_System::keep_alive();
    }
#endif
}

/** Pass a delta to the screen */
void
DemoSession::update(float delta)
{
  UNUSED_ARG(delta);

  // FIXME: Duplicate all timing code here?!
  server->update();
  update_demo();

  int skip_count = 0;
  while (1) //CL_Keyboard::get_keycode(CL_KEY_SPACE) && skip_count < 10)
    {
      ++skip_count;
      server->update();
      update_demo();
    }
}

void
DemoSession::update_demo()
{
#if 0
  while(!events.empty() && events.back().time_stamp == server->get_time())
    {
      ServerEvent& event = events.back();

      std::cout << "Sending: ";
      event.write(std::cout);

      event.send(server);
      events.pop_back();
    }

  // Check for unexpected things (might happen if the demo file is broken)
  if (!events.empty() && events.back().time_stamp < server->get_time())
    {
      std::cout << "DemoPlayer Bug: We missed a timestamp: " << events.back().time_stamp << std::endl;
    }
#endif 
}

/* EOF */
