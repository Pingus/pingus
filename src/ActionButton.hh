//  $Id: ActionButton.hh,v 1.13 2001/03/31 11:21:50 grumbel Exp $
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

#ifndef ACTIONBUTTON_HH
#define ACTIONBUTTON_HH

#include <string>
#include <ClanLib/core.h>

#include "PinguAction.hh"
#include "GuiObj.hh"
#include "ActionHolder.hh"
#include "Server.hh"
#include "AnimCounter.hh"


// ----------------- snip --------------------
///
class Button
{
protected:
  ///
  int x_pos;
  ///
  int y_pos;
  ///
  CL_Surface surface;
  ///
  bool pressed;
public:
  ///
  Button();
  ///
  virtual ~Button();

  ///
  virtual void   draw() = 0;
  ///
  virtual bool   mouse_over() = 0;
};

// ----------------- snip --------------------
///
class EventButton : public Button
{
private: 
public:
  ///
  EventButton(int x, int y, std::string);
  ///
  virtual ~EventButton();
  
  ///
  void   draw();
  ///
  bool   mouse_over();
};

// ----------------- snip --------------------
///
class ArmageddonButton : public Button
{
private:
  ///
  AnimCounter counter;
  ///
  Server* server;
  ///
  friend class ButtonPanel;
public:
  ///
  ArmageddonButton(int x, int y);
  ///
  virtual ~ArmageddonButton();

  ///
  void draw();
  ///
  bool mouse_over();
}///
;

// ----------------- snip --------------------

///
class ForwardButton : public Button
{
private:
  ///
  Server* server;
  ///
  friend class ButtonPanel;
public:
  ///
  ForwardButton(int x, int y);
  ///
  virtual ~ForwardButton();

  ///
  void draw();
  ///
  bool mouse_over();
};

// ----------------- snip --------------------

///
class PauseButton : public Button
{
private:
  ///
  Server* server;
  ///
  friend class ButtonPanel;
public:
  ///
  PauseButton(int x, int y);
  ///
  virtual ~PauseButton();

  ///
  void draw();
  ///
  bool mouse_over();
}///
;

// ----------------- snip --------------------

/** The button class manage a simple button for the button_panel. It
    keeps his position, his surfaces and his font. */
class ActionButton : public Button
{
protected:
  ///
  CL_Font*    font;
  ///
  CL_Font*    font_h;

  /// The x and y position of the button
  std::string name;
  ///
  int available;
  ///
  bool is_multi_direct;

  ///
  ActionHolder* action_holder;
  ///
  AnimCounter action_c;
public:  
  ///
  bool pressed;

  ///
  ActionButton();
  ///
  virtual ~ActionButton();

  ///
  void init(int x, int y, std::string str);

  /// Draws the button and increase the animation counter.
  virtual void   draw() = 0;
 
  /// Not used.
  void   update();

  /// Returns the name of the action the button represents.
  std::string get_action_name();

  /// Returns true if the button is pressed.
  bool   is_pressed();

  ///
  virtual bool   mouse_over() = 0;

  ///
  void set_action_holder(ActionHolder* h);
}///
;

// ----------------- snip --------------------

///
class HorizontalActionButton : public ActionButton 
{
private:
public:
  ///
  HorizontalActionButton(int x, int y, std::string str);
  ///
  virtual ~HorizontalActionButton();
  ///
  void draw();
  ///
  bool mouse_over();
}///
;

// ----------------- snip --------------------
///
class VerticalActionButton : public ActionButton 
{
private:
public:
  ///
  VerticalActionButton(int x, int y, std::string str);
  ///
  virtual ~VerticalActionButton();
  ///
  void draw();
  ///
  bool mouse_over();
};

// ----------------- snip --------------------

#endif /* ACTIONBUTTON */

/* EOF */
