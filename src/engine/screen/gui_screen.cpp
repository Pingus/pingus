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

#include "engine/screen/gui_screen.hpp"

#include <logmich/log.hpp>

#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "pingus/globals.hpp"
#include "pingus/event_name.hpp"

namespace pingus {

GUIScreen::GUIScreen() :
  Screen(Display::get_size()),
  gui_manager(new pingus::gui::GUIManager())
{
}

GUIScreen::~GUIScreen()
{
}

/** Draw this screen */
void
GUIScreen::draw(DrawingContext& gc)
{
  draw_background(gc);
  gui_manager->draw(gc);
  draw_foreground(gc);
}

void
GUIScreen::update(float delta)
{
  gui_manager->update(delta);
}

void
GUIScreen::update_input(pingus::input::Event const& event)
{
  // Dispatch the recieved input events
  gui_manager->update(event);

  switch (event.type)
  {
    case pingus::input::POINTER_EVENT_TYPE:
    {
      // ignored cause this is handled in the gui_manager
    }
    break;

    case pingus::input::BUTTON_EVENT_TYPE:
    {
      process_button_event (event.button);
    }
    break;

    case pingus::input::AXIS_EVENT_TYPE:
    {
      if (event.axis.name == ACTION_AXIS)
      {
        on_action_axis_move(event.axis.dir);
      }
    }
    break;

    case pingus::input::SCROLLER_EVENT_TYPE:
    {

    }
    break;

    case pingus::input::KEYBOARD_EVENT_TYPE:
    {

    }
    break;

    case pingus::input::TEXT_INPUT_EVENT_TYPE:
    {

    }
    break;

    default:
      log_error("unhandled event type: {}", fmt::streamed(event.type));
      break;
  }
}

void
GUIScreen::process_button_event (pingus::input::ButtonEvent const& event)
{
  if (event.state == pingus::input::BUTTON_PRESSED)
  {
    switch (event.name)
    {
      case PRIMARY_BUTTON:
        // ignoring, handled in the gui_manager
        break;
      case SECONDARY_BUTTON:
        // ignoring, handled in the gui_manager
        break;
      case PAUSE_BUTTON:
        on_pause_press();
        break;
      case SINGLE_STEP_BUTTON:
        on_single_step_press();
        break;
      case FAST_FORWARD_BUTTON:
        on_fast_forward_press();
        break;
      case ARMAGEDDON_BUTTON:
        on_armageddon_press();
        break;
      case ESCAPE_BUTTON:
        on_escape_press();
        break;
      case ACTION_UP_BUTTON:
        on_action_up_press();
        break;
      case ACTION_DOWN_BUTTON:
        on_action_down_press();
        break;
      default:
        log_debug("unhandled event: {}", event.name);
        break;
    }
  }
  else if (event.state == pingus::input::BUTTON_RELEASED)
  {
    switch (event.name)
    {
      case PRIMARY_BUTTON:
        // ignoring, handled in the gui_manager
        break;
      case SECONDARY_BUTTON:
        // ignoring, handled in the gui_manager
        break;
      case PAUSE_BUTTON:
        on_pause_release();
        break;
      case SINGLE_STEP_BUTTON:
        on_single_step_release();
        break;
      case FAST_FORWARD_BUTTON:
        on_fast_forward_release();
        break;
      case ARMAGEDDON_BUTTON:
        on_armageddon_release();
        break;
      case ESCAPE_BUTTON:
        on_escape_release();
        break;
      case ACTION_UP_BUTTON:
        on_action_up_release();
        break;
      case ACTION_DOWN_BUTTON:
        on_action_down_release();
        break;
      default:
        log_debug("unhandled event: {}", event.name);
        break;
    }
  }
  else
  {
    log_error("got unknown event.state: {}", fmt::streamed(event.state));
  }
}

void
GUIScreen::resize(Size const& size_)
{
  Screen::resize(size_);
  gui_manager->set_rect(Rect(geom::ipoint(0, 0), size));
}

} // namespace pingus

/* EOF */
