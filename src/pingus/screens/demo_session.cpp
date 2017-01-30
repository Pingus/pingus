// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/screens/demo_session.hpp"

#include <algorithm>
#include <iostream>
#include <functional>

#include "engine/gui/gui_manager.hpp"
#include "engine/gui/surface_button.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/components/pingus_counter.hpp"
#include "pingus/components/playfield.hpp"
#include "pingus/components/smallmap.hpp"
#include "pingus/components/button_panel.hpp"
#include "pingus/pingus_demo.hpp"
#include "pingus/server.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"

static bool false_func() { return false; }

class BButton : public GUI::SurfaceButton
{
private:
  Sprite highlight;
  std::function<void(void)> callback;
  std::function<bool(void)> highlight_func;

public:
  BButton(int x, int y, const std::string& name,
          std::function<void (void)> callback_,
          std::function<bool(void)> highlight_func_ = &false_func) :
    SurfaceButton(x, y, name, name + "-pressed", name + "-hover"),
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

DemoSession::DemoSession(const Pathname& pathname_) :
  pathname(pathname_),
  server(),
  demo(),
  events(),
  pcounter(),
  playfield(),
  small_map(),
  fastforward_button(),
  pause_button(),
  restart_button(),
  pause(false),
  fast_forward(false)
{
  // Load Demo file
  demo = std::unique_ptr<PingusDemo>(new PingusDemo(pathname));

  events = demo->get_events();
  // Reverse the vector so that we can use pop_back()
  std::reverse(events.begin(), events.end());

  // Create server
  PingusLevel plf(Pathname("levels/" + demo->get_levelname()  + ".pingus", Pathname::DATA_PATH));

  if (plf.get_checksum() != demo->get_checksum())
  {
    log_warn("checksum missmatch between demo (%1%) and level (%2%)",
             demo->get_checksum(), plf.get_checksum());
  }

  server   = std::unique_ptr<Server>(new Server(plf, false));

  // Create GUI
  pcounter = gui_manager->create<PingusCounter>(server.get());
  gui_manager->create<ButtonPanel>(server.get(), Vector2i(0, (size.height - 100)/2));

  int world_width  = server->get_world()->get_width();
  int world_height = server->get_world()->get_height();

  playfield = gui_manager->create<Playfield>(
    server.get(), nullptr,
    Rect(Vector2i(Math::max((size.width  - world_width)/2,  0),
                  Math::max((size.height - world_height)/2, 0)),
         Size(Math::min(size.width,  world_width),
              Math::min(size.height, world_height))));

  gui_manager->create<SmallMap>(server.get(), playfield, Rect(Vector2i(5, size.height - 105), Size(175, 100)));

  fastforward_button = gui_manager->create<BButton>(32+50, 32, "core/demo/fastforward",
                                                    std::bind(&DemoSession::on_fast_forward_press, this),
                                                    std::bind(&DemoSession::is_fast_forward, this));
  pause_button = gui_manager->create<BButton>(32,  32, "core/demo/pause",
                                              std::bind(&DemoSession::on_pause_press, this),
                                              std::bind(&DemoSession::is_pause, this));
  restart_button = gui_manager->create<BButton>(size.width - 32 - 48, 32, "core/demo/reload",
                                                std::bind(&DemoSession::restart, this));
}

DemoSession::~DemoSession()
{
}

void
DemoSession::draw_background(DrawingContext& gc)
{
  Rect rect = playfield->get_rect();

  if (rect != Rect(Vector2i(0,0), Size(gc.get_width(), gc.get_height())))
  { // Draw a black border around the playfield when the playfield is smaller then the screen
    Color border_color(0, 0, 0);
    // top
    gc.draw_fillrect(Rect(0, 0, gc.get_width(), rect.top),
                     border_color);
    // bottom
    gc.draw_fillrect(Rect(0, rect.bottom, gc.get_width(), gc.get_height()),
                     border_color);
    // left
    gc.draw_fillrect(Rect(0, rect.top, rect.left, rect.bottom),
                     border_color);
    // right
    gc.draw_fillrect(Rect(rect.right, rect.top, gc.get_width(), rect.bottom),
                     border_color);
  }
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
    log_info("DemoPlayer Bug: We missed a timestamp: %1%", events.back().time_stamp);
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
    log_info("Fast Forward Pressed: %1% %2%", events.size(), server->get_time());

  fast_forward = !fast_forward;
}

void
DemoSession::on_escape_press()
{
  log_info("Escape Pressed");
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
  ScreenManager::instance()->replace_screen(std::make_shared<DemoSession>(pathname));
}

void
DemoSession::resize(const Size& size_)
{
  GUIScreen::resize(size_);

  int world_width  = server->get_world()->get_width();
  int world_height = server->get_world()->get_height();

  playfield->set_rect(Rect(Vector2i(Math::max((size.width  - world_width)/2,  0),
                                    Math::max((size.height - world_height)/2, 0)),
                           Size(Math::min(size.width,  world_width),
                                Math::min(size.height, world_height))));

  fastforward_button->set_pos(32+50, 32);
  pause_button->set_pos(32,  32);
  restart_button->set_pos(size.width - 32 - 48, 32);

  small_map->set_rect(Rect(Vector2i(5, size.height - 105), Size(175, 100)));
}

/* EOF */
