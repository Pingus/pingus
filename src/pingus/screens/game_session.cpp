// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/screens/game_session.hpp"

#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "engine/screen/screen_manager.hpp"
#include "pingus/server.hpp"

#include "engine/sound/sound.hpp"
#include "pingus/components/button_panel.hpp"
#include "pingus/components/pingus_counter.hpp"
#include "pingus/components/playfield.hpp"
#include "pingus/components/smallmap.hpp"
#include "pingus/components/time_display.hpp"
#include "pingus/globals.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/savegame_manager.hpp"
#include "pingus/screens/result_screen.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"

GameSession::GameSession(const PingusLevel& arg_plf, bool arg_show_result_screen) :
  plf(arg_plf),
  show_result_screen(arg_show_result_screen),
  server(),
  world_delay(),
  button_panel(),
  pcounter(),
  playfield(),
  small_map(),
  armageddon_button(),
  forward_button(),
  pause_button(),
  pause(false),
  fast_forward(false),
  single_step(false)
{
  server = std::unique_ptr<Server>(new Server(plf, true));

  // the world is initially on time
  world_delay = 0;

  log_debug("GameSession");

  int world_width  = server->get_world()->get_width();
  int world_height = server->get_world()->get_height();

  playfield = gui_manager->create<Playfield>(
    get_server(), this,
    Rect(Vector2i(Math::max((Display::get_width()  - world_width)/2,  0),
                  Math::max((Display::get_height() - world_height)/2, 0)),
         Size(Math::min(Display::get_width(),  world_width),
              Math::min(Display::get_height(), world_height))));

  pcounter = gui_manager->create<PingusCounter>(get_server());
  small_map = gui_manager->create<SmallMap>(get_server(), playfield, Rect(Vector2i(5, size.height - 105), Size(175, 100)));

  button_panel = gui_manager->create<ButtonPanel>(get_server(), Vector2i(0, (size.height - 150)/2));
  gui_manager->create<TimeDisplay>(this);

  armageddon_button = gui_manager->create<ArmageddonButton>(get_server(), Display::get_width() - 40, Display::get_height() - 62);
  forward_button = gui_manager->create<ForwardButton>(this, Display::get_width() - 40 * 2, Display::get_height() - 62);
  pause_button = gui_manager->create<PauseButton>(this, Display::get_width() - 40 * 3, Display::get_height() - 62);
}

GameSession::~GameSession()
{
}

void
GameSession::update_server(float delta)
{
  if (server->is_finished())
  {
    PinguHolder* pingu_holder = server->get_world()->get_pingus();
    Result result;

    result.plf    = server->get_plf();

    result.saved  = pingu_holder->get_number_of_exited();
    result.killed = pingu_holder->get_number_of_killed();
    result.total  = server->get_plf().get_number_of_pingus();

    result.needed = server->get_plf().get_number_to_save();

    result.max_time  = server->get_plf().get_time();
    result.used_time = server->get_time();

    { // Write the savegame
      Savegame savegame(result.plf.get_resname(),
                        (result.saved >= result.needed) ? Savegame::FINISHED : Savegame::ACCESSIBLE,
                        result.used_time,
                        result.saved);
      SavegameManager::instance()->store(savegame);
    }

    if (show_result_screen)
      ScreenManager::instance()->replace_screen(std::make_shared<ResultScreen>(result));
    else
      ScreenManager::instance()->pop_screen();

  }
  else
  {
    // how much time we have to account for while doing world updates
    int time_passed = int(delta * 1000) + world_delay;
    // how much time each world update represents
    int update_time = globals::game_speed;

    // update the world (and the objects in it) in constant steps to account
    // for the time the previous frame took

    // invariant: world_updates - the number of times the world
    // has been updated during this frame
    int world_updates = 0;

    while ((world_updates+1)*update_time <= time_passed)
    {
      if (!pause || single_step)
      {
        single_step = false;

        if (fast_forward)
        {
          for (int i = 0; i < globals::fast_forward_time_scale; ++i)
            server->update();
        }
        else
        {
          server->update();
        }
      }

      world_updates++;
    }
    // save how far behind is the world compared to the actual time
    // so that we can account for that while updating in the next frame
    world_delay = time_passed - (world_updates*update_time);
  }
}

void
GameSession::draw_background (DrawingContext& gc)
{
  Rect rect = playfield->get_rect();

  if (rect != Rect(Vector2i(0,0), Size(Display::get_width(), Display::get_height())))
  { // Draw a black border around the playfield when the playfield is smaller then the screen
    Color border_color(0, 0, 0);
    // top
    gc.draw_fillrect(Rect(0, 0, Display::get_width(), rect.top),
                     border_color);
    // bottom
    gc.draw_fillrect(Rect(0, rect.bottom, Display::get_width(), Display::get_height()),
                     border_color);
    // left
    gc.draw_fillrect(Rect(0, rect.top, rect.left, rect.bottom),
                     border_color);
    // right
    gc.draw_fillrect(Rect(rect.right, rect.top, Display::get_width(), rect.bottom),
                     border_color);
  }
}

void
GameSession::update(float delta)
{
  update_server(delta);
  GUIScreen::update(delta);
}

void
GameSession::update(const Input::Event& event)
{
  GUIScreen::update(event);

  //log_info("Events: " << event.get_type ());

  switch (event.type)
  {
    case Input::BUTTON_EVENT_TYPE:
    {
      const Input::ButtonEvent& ev = event.button;

      if (ev.state == Input::BUTTON_PRESSED)
      {
        if (ev.name >= Input::ACTION_1_BUTTON && ev.name <= Input::ACTION_10_BUTTON)
        {
          button_panel->set_button(ev.name - Input::ACTION_1_BUTTON);
        }
        else if (ev.name == Input::ACTION_DOWN_BUTTON)
        {
          button_panel->next_action();
        }
        else if (ev.name == Input::ACTION_UP_BUTTON)
        {
          button_panel->previous_action();
        }
      }
    }
    break;

    case Input::POINTER_EVENT_TYPE:
      // Ignore, is handled in GUIScreen
      break;

    case Input::AXIS_EVENT_TYPE:
      // ???
      process_axis_event (event.axis);
      break;

    case Input::SCROLLER_EVENT_TYPE:
      process_scroll_event(event.scroll);
      break;

    case Input::KEYBOARD_EVENT_TYPE:
      break;

    case Input::TEXT_INPUT_EVENT_TYPE:
      break;

    default:
      // unhandled event
      log_info("unhandled event: %1%", event.type);
      break;
  }
}

void
GameSession::process_scroll_event (const Input::ScrollEvent& ev)
{
  playfield->scroll(static_cast<int>(-ev.x_delta),
                    static_cast<int>(-ev.y_delta));
}

void
GameSession::process_axis_event (const Input::AxisEvent& event)
{
  // log_info("GameSession::process_axis_event ()");
}

void
GameSession:: on_escape_press ()
{
  server->send_finish_event();
}

void
GameSession:: on_pause_press ()
{
  set_pause(!get_pause());
}

void
GameSession::on_single_step_press ()
{
  set_pause(true);
  single_step = true;
}

void
GameSession::on_fast_forward_press ()
{
  if (get_pause())
  {
    set_pause(false);
  }
  else
  {
    set_fast_forward(true);
  }
}

void
GameSession::on_fast_forward_release ()
{
  set_fast_forward(false);
}

void
GameSession::on_armageddon_press ()
{
  server->send_armageddon_event();
}

void
GameSession::on_action_axis_move (float move)
{
  if (move > 0)
    button_panel->next_action ();
  else if (move < 0)
    button_panel->previous_action ();
}

void
GameSession::on_startup ()
{
  if (globals::developer_mode)
    log_info("Starting Music: %1%", server->get_plf().get_music());

  if (server->get_plf().get_music() == "none" ||
      server->get_plf().get_music().empty())
  {
    Sound::PingusSound::stop_music();
  }
  else
  {
    Sound::PingusSound::play_music(server->get_plf().get_music());
  }
}

ActionName::Enum
GameSession::get_action_name() const
{
  return button_panel->get_action_name();
}

void
GameSession::set_fast_forward(bool value)
{
  fast_forward = value;
  if (fast_forward)
  {
    pause = false;
  }
}

bool
GameSession::get_fast_forward() const
{
  return fast_forward;
}

void
GameSession::set_pause(bool value)
{
  pause = value;
  if (pause)
  {
    fast_forward = false;
  }
}

bool
GameSession::get_pause() const
{
  return pause;
}

void
GameSession::resize(const Size& size_)
{
  GUIScreen::resize(size_);

  int world_width  = server->get_world()->get_width();
  int world_height = server->get_world()->get_height();

  playfield->set_rect(Rect(Vector2i(Math::max((size.width  - world_width)/2,  0),
                                    Math::max((size.height - world_height)/2, 0)),
                           Size(Math::min(size.width,  world_width),
                                Math::min(size.height, world_height))));

  armageddon_button->set_rect(Rect(Vector2i(size.width - 40, size.height - 62),
                                   Size(38, 60)));
  forward_button->set_rect(Rect(Vector2i(size.width - 40*2, size.height - 62),
                                Size(38, 60)));
  pause_button->set_rect(Rect(Vector2i(size.width - 40*3, size.height - 62),
                              Size(38, 60)));

  small_map->set_rect(Rect(Vector2i(5, size.height - 105), Size(175, 100)));

  button_panel->set_pos(Vector2i(0, (size.height - 150)/2));
}

/* EOF */
