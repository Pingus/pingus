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

#ifndef HEADER_PINGUS_FPS_COUNTER_HXX
#define HEADER_PINGUS_FPS_COUNTER_HXX

#include "gui/display.hpp"


///
class FPSCounter : public DisplayHook
{
private:
  /** The font... */
  Font font;

  /** Used to know when the frame has changed */
  bool odd_frame;

  /** A string holding the current value of frames + "fps", its
      updated every second */
  char fps_string[16];

  /** Count the fps until one second is passed */
  int fps_count;

  /** The time were the last fps string update was performent */
  unsigned int start_time;

  /** Check if one second is passed and if that is the case, update
      everything then. */
  virtual void update_fps_counter();

public:
  ///
  FPSCounter();
  ///
  virtual ~FPSCounter();

  /** When we got a Display::flip_display() this function is
      called... */
  virtual void on_event();

  /** Load all the gfx and fonts... */
  void init();

	/** Unload gfx and fonts... */
	void deinit();

private:
  FPSCounter (const FPSCounter&);
  FPSCounter& operator= (const FPSCounter&);
};

/** The fps_counter is a global object, so we don't need to construct
    it over and over again */
extern FPSCounter fps_counter;


#endif

/* EOF */
