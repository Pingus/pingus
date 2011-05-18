//  $Id$
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

#ifndef HEADER_PINGUS_SCREEN_MANAGER_HXX
#define HEADER_PINGUS_SCREEN_MANAGER_HXX

#include "../pingus.hpp"
#include <vector>

#include "screen_ptr.hpp"

class Size;
class DrawingContext;
class Screen;

class ScreenManager
{
private:
  static ScreenManager* instance_;

  SDL_Surface* screen;
  DrawingContext* display_gc;

  /** Screen stack (first is the screen, second is delete_screen,
      which tells if the screen should be deleted onces it got poped
      or replaced) */
  std::vector<ScreenPtr> screens;

  /** the screen that was used in the last update() */
  ScreenPtr last_screen;

  enum { CA_NONE, CA_POP, CA_POP_ALL, CA_REPLACE, CA_CLEAR } cached_action;
  ScreenPtr replace_screen_arg;

  int m_time_limit_msec;
  int m_time_passed;

protected:
  ScreenManager ();
public:
  ~ScreenManager();

  void resize(const Size& size);

  /** Start the screen manager and let it take control, this will
      not return until the somebody signals a quit() */
  void display ();

  /** Replace the current screen */
  void replace_screen (Screen*, bool delete_screen = false);

  /** Add a screen on top of another screen */
  void push_screen (Screen*, bool delete_screen = false);

  /** Remove the current screen and fall back to the last one */
  void pop_screen ();

  /** Remove all screens */
  void pop_all_screens();

  /** Remove all screens from the stack */
  void clear();

  /** @return a pointer to the current Screen */
  ScreenPtr& get_current_screen();

  /** @return a pointer to the current Screen */
  Screen* get_screen();

  void set_time_limit(int msec);
  int get_time_limit() const;
  bool time_limit_over() const;

  void draw_time_limit(DrawingContext& gc);

private:
  void real_clear();

  /** Replace the current screen */
  void real_replace_screen (const ScreenPtr&);

  /** Remove the current screen and fall back to the last one */
  void real_pop_screen ();

  /** Remove all screens */
  void real_pop_all_screens();

  /** FadeOver test*/
  void fade_over (ScreenPtr& old_screen, ScreenPtr& new_screen);

public:
  static ScreenManager* instance ();
  static void init();
  static void deinit();
private:
  ScreenManager (const ScreenManager&);
  ScreenManager& operator= (const ScreenManager&);
};

#endif

/* EOF */
