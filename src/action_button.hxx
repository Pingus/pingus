//  $Id: action_button.hxx,v 1.7 2002/08/03 12:00:58 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_ACTION_BUTTON_HXX
#define HEADER_PINGUS_ACTION_BUTTON_HXX

#include <ClanLib/Display/Display/surface.h>
#include <string>
#include "anim_counter.hxx"
#include "pingu_enums.hxx"
#include "gui/component.hxx"

using Pingus::Actions::ActionName;

class Server;
class ActionHolder;
class CL_Font;
class CL_Vector;

// ----------------- snip --------------------
///
class ArmageddonButton : public GUI::Component
{
private:
  Server* server;
  int x_pos;
  int y_pos;
  bool pressed;
  float press_time;
  CL_Surface surface;
  CL_Surface background;
  CL_Surface backgroundhl;

  AnimCounter counter;
  friend class ButtonPanel;
public:
  ArmageddonButton(Server*, int x, int y);
  virtual ~ArmageddonButton();

  void draw();
  void update(float);
  bool is_at(int x, int y);
  void on_primary_button_click (int x, int y);
};

// ----------------- snip --------------------

///
class ForwardButton : public GUI::Component
{
private:
  Server* server;
  int x_pos;
  int y_pos;
  CL_Surface surface;
  CL_Surface background;
  CL_Surface backgroundhl;
  friend class ButtonPanel;
public:
  ForwardButton(Server*, int x, int y);
  virtual ~ForwardButton();

  void draw();
  bool is_at (int x, int y);
  void on_primary_button_click (int x, int y);
};

// ----------------- snip --------------------

///
class PauseButton : public GUI::Component
{
private:
  Server* server;
  int x_pos;
  int y_pos;
  CL_Surface surface;
  CL_Surface background;
  CL_Surface backgroundhl;
  friend class ButtonPanel;
public:
  PauseButton(Server*, int x, int y);
  virtual ~PauseButton();

  void draw();
  bool is_at (int x, int y);
  void on_primary_button_click (int x, int y);
};

// ----------------- snip --------------------

/** The button class manage a simple button for the button_panel. It
    keeps his position, his surfaces and his font. */
class ActionButton : public GUI::Component
{
protected:
  CL_Surface surface;
  int x_pos;
  int y_pos;
  CL_Font*    font;
  CL_Font*    font_h;
  // Added for printing action names next to the button.
  CL_Font*    font_b;

  ActionName name;
  int available;
  bool is_multi_direct;

  ActionHolder* action_holder;
  AnimCounter action_c;
public:  
  bool pressed;

  ActionButton();
  virtual ~ActionButton();

  void init(int x, int y, ActionName name_, int owner_id);

  /// Draws the button and increase the animation counter.
  virtual void   draw() = 0;
 
  /// Not used.
  void   update(float delta);

  /// Returns the name of the action the button represents.
  ActionName get_action_name();

  /// Returns true if the button is pressed.
  bool   is_pressed();

  virtual bool   is_at(int x, int y) = 0;

  void set_action_holder(ActionHolder* h);
};

// ----------------- snip --------------------
///
class VerticalActionButton : public ActionButton 
{
private:
  CL_Surface background;
  CL_Surface backgroundhl;

public:
  VerticalActionButton(int x, int y, ActionName name, int owner_id);
  virtual ~VerticalActionButton();

  void draw();
  bool is_at (int x, int y);
};

// ----------------- snip --------------------

#endif /* ACTIONBUTTON */

/* EOF */




