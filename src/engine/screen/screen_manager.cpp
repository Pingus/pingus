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

#include "engine/screen/screen_manager.hpp"

#include <iostream>
#include <utility>

#include "engine/display/display.hpp"
#include "engine/display/drawing_context.hpp"
#include "engine/display/framebuffer.hpp"
#include "engine/input/manager.hpp"
#include "engine/screen/screen.hpp"
#include "pingus/fps_counter.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"

namespace {

template<class C>
void write(std::ostream& out, const C& value)
{
  out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template<class C>
void read(std::istream& out, C& value)
{
  out.read(reinterpret_cast<char*>(&value), sizeof(value));
}

void write_events(std::ostream& out, const std::vector<Input::Event>& events)
{
  write(out, events.size());
  for(std::vector<Input::Event>::const_iterator i = events.begin();
      i != events.end();
      ++i)
  {
    write(out, *i);
  }
}

void read_events(std::istream& out, std::vector<Input::Event>& events)
{
  std::vector<Input::Event>::size_type len;
  read(out, len);
  for(std::vector<Input::Event>::size_type i = 0; i < len; ++i)
  {
    Input::Event event;
    read(out, event);
    events.push_back(event);
  }
}

} // namespace

ScreenManager* ScreenManager::instance_ = nullptr;

ScreenManager::ScreenManager(Input::Manager& arg_input_manager,
                             Input::ControllerPtr arg_input_controller) :
  input_manager(arg_input_manager),
  input_controller(std::move(arg_input_controller)),
  display_gc(new DrawingContext()),
  fps_counter(),
  cursor(),
  screens(),
  mouse_pos(),
  record_input(false),
  playback_input(false)
{
  assert(instance_ == nullptr);
  instance_ = this;

  cursor = Sprite("core/cursors/animcross");
  fps_counter = std::unique_ptr<FPSCounter>(new FPSCounter());
}

ScreenManager::~ScreenManager()
{
  instance_ = nullptr;
}

void
ScreenManager::display()
{
  show_software_cursor(globals::software_cursor);

  Uint32 last_ticks = SDL_GetTicks();
  float previous_frame_time;
  std::vector<Input::Event> events;

  while (!screens.empty())
  {
    events.clear();

    // Get time and update Input::Events
    if (playback_input)
    {
      // Get Time
      read(std::cin, previous_frame_time);

      // Update InputManager so that SDL_QUIT and stuff can be
      // handled, even if the basic events are taken from record
      input_manager.update(previous_frame_time);
      input_controller->clear_events();
      read_events(std::cin, events);
    }
    else
    {
      // Get Time
      Uint32 ticks = SDL_GetTicks();
      previous_frame_time  = float(ticks - last_ticks)/1000.0f;
      last_ticks = ticks;

      // Update InputManager and get Events
      input_manager.update(previous_frame_time);
      input_controller->poll_events(events);
    }

    if (record_input)
    {
      write(std::cerr, previous_frame_time);
      write_events(std::cerr, events);
    }

    if (globals::software_cursor)
      cursor.update(previous_frame_time);

    // previous frame took more than one second
    if (previous_frame_time > 1.0f)
    {
      if (globals::developer_mode)
        log_warn("ScreenManager: previous frame took longer than 1 second ({} sec.), ignoring and doing frameskip", previous_frame_time);
    }
    else
    {
      update(previous_frame_time, events);

      // cap the framerate at the desired value
      // figure out how long this frame took
      float current_frame_time = float(SDL_GetTicks() - last_ticks) / 1000.0f;
      // idly delay if this frame didn't last long enough to
      // achieve <desired_fps> frames per second
      if (current_frame_time < 1.0f / globals::desired_fps) {
        Uint32 sleep_time = static_cast<Uint32>(1000 *((1.0f / globals::desired_fps) - current_frame_time));
        SDL_Delay(sleep_time);
      }
    }
  }
}

void
ScreenManager::update(float delta, const std::vector<Input::Event>& events)
{
  ScreenPtr last_screen = get_current_screen();

  // Will be triggered when pop_all_screens() is called by pressing window close button
  if (!last_screen)
    return;

  for(std::vector<Input::Event>::const_iterator i = events.begin(); i != events.end(); ++i)
  {
    if (i->type == Input::POINTER_EVENT_TYPE && i->pointer.name == Input::STANDARD_POINTER)
      mouse_pos = Vector2i(static_cast<int>(i->pointer.x),
                           static_cast<int>(i->pointer.y));

    last_screen->update_input(*i);

    if (last_screen != get_current_screen())
    {
      fade_over(last_screen, get_current_screen());
      return;
    }
  }

  last_screen->update(delta);
  if (last_screen != get_current_screen())
  {
    fade_over(last_screen, get_current_screen());
    return;
  }

  // Draw screen to DrawingContext
  get_current_screen()->draw(*display_gc);

  // Render the DrawingContext to the screen
  display_gc->render(*Display::get_framebuffer(), Rect(geom::ipoint(0,0), Size(Display::get_width(),
                                                                           Display::get_height())));
  display_gc->clear();

  // Draw the mouse pointer
  if (globals::software_cursor)
    cursor.render(mouse_pos.x, mouse_pos.y, *Display::get_framebuffer());

  // Draw FPS Counter
  if (globals::print_fps)
  {
    fps_counter->draw();
    if (globals::developer_mode)
    {
      Fonts::pingus_small.render(Origin::CENTER, Display::get_width()/2, 60,
                                 "Developer Mode", *Display::get_framebuffer());
    }
  }
  else if (globals::developer_mode)
  {
    Fonts::pingus_small.render(Origin::CENTER, Display::get_width()/2, 35,
                               "Developer Mode", *Display::get_framebuffer());
  }

  Display::flip_display();
}

ScreenPtr
ScreenManager::get_current_screen()
{
  if (screens.empty())
    return ScreenPtr();
  else
    return screens.back();
}

ScreenManager*
ScreenManager::instance()
{
  return instance_;
}

void
ScreenManager::push_screen(const ScreenPtr& screen)
{
  screens.push_back(screen);
  screen->on_startup();
}

void
ScreenManager::pop_screen()
{
  screens.pop_back();

  if (!screens.empty())
  {
    if (screens.back()->get_size() != Display::get_size())
      screens.back()->resize(Display::get_size());
    screens.back()->on_startup();
  }
}

void
ScreenManager::pop_all_screens()
{
  screens.clear();
}

void
ScreenManager::replace_screen(ScreenPtr screen)
{
  screens.back() = std::move(screen);

  if (screens.back()->get_size() != Display::get_size())
  {
    screens.back()->resize(Display::get_size());
  }

  screens.back()->on_startup();
}

void
ScreenManager::fade_over(const ScreenPtr& old_screen, const ScreenPtr& new_screen)
{
  if (!old_screen.get() || !new_screen.get())
    return;

  Uint32 last_ticks = SDL_GetTicks();
  float progress = 0.0f;
  Framebuffer& fb = *Display::get_framebuffer();
  while (progress <= 1.0f)
  {
    float p = progress; //0.5f + progress * 0.1f;
    int border_x = static_cast<int>(static_cast<float>(Display::get_width()/2)  * (1.0f - p)); // NOLINT
    int border_y = static_cast<int>(static_cast<float>(Display::get_height()/2) * (1.0f - p)); // NOLINT

    old_screen->draw(*display_gc);
    display_gc->render(fb, Rect(geom::ipoint(0,0), Size(Display::get_width(),
                                                    Display::get_height())));
    display_gc->clear();

    Size clip_size(Display::get_width()  - 2*border_x,
                   Display::get_height() - 2*border_y);

    if (clip_size != Size(0, 0))
    {
      // skip drawing the second screen when the cliprect
      fb.push_cliprect(Rect(geom::ipoint(0 + border_x, 0 + border_y),
                            clip_size));

      new_screen->draw(*display_gc);
      display_gc->render(*Display::get_framebuffer(), Rect(geom::ipoint(0,0), Size(Display::get_width(),
                                                                               Display::get_height())));
      display_gc->clear();

      fb.pop_cliprect();
    }

    fb.flip();
    display_gc->clear();

    progress = static_cast<float>(SDL_GetTicks() - last_ticks)/1000.0f * 2.0f;
    SDL_Delay(10);
  }

  input_manager.refresh();
}

void
ScreenManager::resize(const Size& size)
{
  display_gc->set_rect(Rect(geom::ipoint(0, 0), size));

  // The other screens will get resized when they become the current screen
  get_current_screen()->resize(size);
}

void
ScreenManager::show_software_cursor(bool visible)
{
  globals::software_cursor = visible;

  if (globals::software_cursor)
  {
    SDL_ShowCursor(SDL_DISABLE);
  }
  else
  {
    SDL_ShowCursor(SDL_ENABLE);
  }
}

/* EOF */
