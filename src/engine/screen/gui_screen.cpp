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

#include "engine/display/display.hpp"
#include "engine/gui/gui_manager.hpp"
#include "pingus/globals.hpp"
#include "util/log.hpp"

GUIScreen::GUIScreen() :
  Screen(Display::get_size()),
  gui_manager(new GUI::GUIManager())
{
}

GUIScreen::~GUIScreen ()
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
GUIScreen::update (const Input::Event& event)
{
  // Dispatch the recieved input events
  gui_manager->update(event);

  switch (event.type)
  {
    case Input::POINTER_EVENT_TYPE:
    {
      // ignored cause this is handled in the gui_manager
    }
    break;

    case Input::BUTTON_EVENT_TYPE:
    {
      process_button_event (event.button);
    }
    break;

    case Input::AXIS_EVENT_TYPE:
    {
      if (event.axis.name == Input::ACTION_AXIS)
      {
        on_action_axis_move(event.axis.dir);
      }
    }
    break;

    case Input::SCROLLER_EVENT_TYPE:
    {

    }
    break;

    case Input::KEYBOARD_EVENT_TYPE:
    {

    }
    break;

    case Input::TEXT_INPUT_EVENT_TYPE:
    {

    }
    break;

    default:
      log_error("unhandled event type: %1%", event.type);
      break;
  }
}

void
GUIScreen::process_button_event (const Input::ButtonEvent& event)
{
  if (event.state == Input::BUTTON_PRESSED)
  {
    switch (event.name)
    {
      case Input::PRIMARY_BUTTON:
        // ignoring, handled in the gui_manager
        break;
      case Input::SECONDARY_BUTTON:
        // ignoring, handled in the gui_manager
        break;
      case Input::PAUSE_BUTTON:
        on_pause_press ();
        break;
      case Input::SINGLE_STEP_BUTTON:
        on_single_step_press();
        break;
      case Input::FAST_FORWARD_BUTTON:
        on_fast_forward_press ();
        break;
      case Input::ARMAGEDDON_BUTTON:
        on_armageddon_press ();
        break;
      case Input::ESCAPE_BUTTON:
        on_escape_press ();
        break;
      case Input::ACTION_UP_BUTTON:
        on_action_up_press();
        break;
      case Input::ACTION_DOWN_BUTTON:
        on_action_down_press();
        break;
      default:
        log_debug("unhandled event: %1%", event.name);
        break;
    }
  }
  else if (event.state == Input::BUTTON_RELEASED)
  {
    switch (event.name)
    {
      case Input::PRIMARY_BUTTON:
        // ignoring, handled in the gui_manager
        break;
      case Input::SECONDARY_BUTTON:
        // ignoring, handled in the gui_manager
        break;
      case Input::PAUSE_BUTTON:
        on_pause_release ();
        break;
      case Input::SINGLE_STEP_BUTTON:
        on_single_step_release();
        break;
      case Input::FAST_FORWARD_BUTTON:
        on_fast_forward_release ();
        break;
      case Input::ARMAGEDDON_BUTTON:
        on_armageddon_release ();
        break;
      case Input::ESCAPE_BUTTON:
        on_escape_release ();
        break;
      case Input::ACTION_UP_BUTTON:
        on_action_up_release();
        break;
      case Input::ACTION_DOWN_BUTTON:
        on_action_down_release();
        break;
      default:
        log_debug("unhandled event: %1%", event.name);
        break;
    }
  }
  else
  {
    log_error("got unknown event.state: %1%", event.state);
  }
}

void
GUIScreen::resize(const Size& size_)
{
  Screen::resize(size_);
  gui_manager->set_rect(Rect(Vector2i(0, 0), size));
}

/* EOF */
