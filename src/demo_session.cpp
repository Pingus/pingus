//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "math.hpp"
#include "server.hpp"
#include "pathname.hpp"
#include "world.hpp"
#include "components/pingus_counter.hpp"
#include "gui/gui_manager.hpp"
#include "display/scene_context.hpp"
#include "pingus_demo.hpp"
#include "gui/surface_button.hpp"
#include "components/playfield.hpp"
#include "components/smallmap.hpp"
#include "screen/screen_manager.hpp"
#include "display/display.hpp"
#include "resource.hpp"
#include "demo_session.hpp"

static bool false_func() { return false; }

class BButton : public GUI::SurfaceButton
{
private:
  Sprite highlight;
  boost::function<void(void)> callback;
  boost::function<bool(void)> highlight_func;
  
public:
  BButton(int x, int y, const std::string& name, 
          boost::function<void (void)> callback_,
          boost::function<bool(void)> highlight_func_ = &false_func)
    : SurfaceButton(x, y, name, name + "-pressed", name + "-hover"),
      highlight("core/demo/highlight"),
      callback(callback_),
      highlight_func(highlight_func_)
  {}

  virtual void draw (DrawingContext& gc) 
  {

    if (highlight_func())
      {
        gc.draw(button_pressed_surface, Vector2i(x_pos, y_pos));
        gc.draw(highlight, Vector2i(x_pos, y_pos));
      }
    else
      {
        SurfaceButton::draw(gc);
      }
  }

  void on_click() {
    callback();
  }
};

DemoSession::DemoSession(const Pathname& pathname_)
  : pathname(pathname_),
    pause(false),
    fast_forward(false)
{
  // Load Demo file
  demo = std::auto_ptr<PingusDemo>(new PingusDemo(pathname));

  events = demo->get_events();
  // Reverse the vector so that we can use pop_back()  
  std::reverse(events.begin(), events.end());

  // Create server
  server   = std::auto_ptr<Server>(new Server(PingusLevel(Pathname("levels/" + demo->get_levelname()  + ".pingus", 
                                                                   Pathname::DATA_PATH)), 
                                              false));

  // Create GUI
  pcounter = new PingusCounter(server.get());
  gui_manager->add(pcounter);

  int world_width  = server->get_world()->get_width();
  int world_height = server->get_world()->get_height();

  playfield = new Playfield(server.get(), 0,
                            Rect(Vector2i(Math::max((Display::get_width()  - world_width)/2,  0),
                                          Math::max((Display::get_height() - world_height)/2, 0)), 
                                 Size(Math::min(Display::get_width(),  world_width),
                                      Math::min(Display::get_height(), world_height))));

  gui_manager->add(playfield);

  small_map    = new SmallMap(server.get(), playfield, Rect(Vector2i(5, size.height - 105), Size(175, 100)));
  gui_manager->add(small_map);

  gui_manager->add(new BButton(32+50, 32, "core/demo/fastforward",
                               boost::bind(&DemoSession::on_fast_forward_press, this),
                               boost::bind(&DemoSession::is_fast_forward, this)));
  gui_manager->add(new BButton(32,  32, "core/demo/pause",
                               boost::bind(&DemoSession::on_pause_press, this),
                               boost::bind(&DemoSession::is_pause, this)));
  gui_manager->add(new BButton(Display::get_width() - 32 - 48, 32, "core/demo/reload",
                               boost::bind(&DemoSession::restart, this)));
}

DemoSession::~DemoSession()
{
}

/** Pass a delta to the screen */
void
DemoSession::update(float delta)
{
  GUIScreen::update(delta);

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
DemoSession::on_scroller_move(float x, float y)
{
  // FIXME: Rounding considered evil?
  playfield->scroll(static_cast<int>(-x),
                    static_cast<int>(-y));
}

void
DemoSession::restart()
{
  ScreenManager::instance()->replace_screen(new DemoSession(pathname));
}

/* EOF */
