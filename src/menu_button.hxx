//  $Id: menu_button.hxx,v 1.7 2003/10/21 21:37:06 grumbel Exp $
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

#ifndef HEADER_PINGUS_SURFACE_BUTTON_HXX
#define HEADER_PINGUS_SURFACE_BUTTON_HXX

#include <string>
#include <ClanLib/Display/surface.h>
#include <ClanLib/Display/font.h>
#include "gui/component.hxx"

namespace Pingus {

class PingusMenu;

/** Framework for menu buttons */
class SurfaceButton : public GUI::Component
{
protected:
  CL_Surface surface_p;
  CL_Font     font;
  CL_Font     font_large;

  int x_pos;
  int y_pos;

  std::string desc;
  std::string line1;
  std::string line2;

  bool mouse_over;
  bool pressed;

public:
  SurfaceButton();
  virtual ~SurfaceButton();

  void draw(GraphicContext& gc);
  void update (float delta);

  bool is_at (int x, int y);

  void on_pointer_enter ();
  void on_pointer_leave ();
  void on_pointer_press ();
  void on_pointer_release ();

  void on_primary_button_click (int x, int y) { on_click (); UNUSED_ARG(x); UNUSED_ARG(y); }

  virtual void on_click () =0;

private:
  SurfaceButton (const SurfaceButton&);
  SurfaceButton& operator= (const SurfaceButton&);
};

class CreditButton : public SurfaceButton
{
private:
  PingusMenu* menu;
public:
  CreditButton(PingusMenu* menu);
  virtual ~CreditButton();
  void on_click();

private:
  CreditButton (const CreditButton&);
  CreditButton& operator= (const CreditButton&);
};

class OptionsButton : public SurfaceButton
{
private:
  PingusMenu* menu;
public:
  OptionsButton(PingusMenu* menu);
  virtual ~OptionsButton();
  void on_click();

private:
  OptionsButton (const OptionsButton&);
  OptionsButton& operator= (const OptionsButton&);
};

class QuitButton : public SurfaceButton
{
private:
  PingusMenu* menu;
public:
  QuitButton(PingusMenu*);
  virtual ~QuitButton();
  void on_click();

private:
  QuitButton (const QuitButton&);
  QuitButton& operator= (const QuitButton&);
};

class LoadButton : public SurfaceButton
{
public:
  LoadButton();
  virtual ~LoadButton();
  void on_click();

private:
  LoadButton (const LoadButton&);
  LoadButton& operator= (const LoadButton&);
};

class EditorButton : public SurfaceButton
{
private:
  PingusMenu* menu;

public:
  EditorButton(  PingusMenu* menu);
  virtual ~EditorButton();

  void on_click();
  void load_level(const std::string& str);

private:
  EditorButton (const EditorButton&);
  EditorButton& operator= (const EditorButton&);
};

class StoryButton : public SurfaceButton
{
private:
  PingusMenu* menu;
public:
  StoryButton(PingusMenu* menu);
  ~StoryButton();
  void on_click();

private:
  StoryButton (const StoryButton&);
  StoryButton& operator= (const StoryButton&);
};

class ThemeButton : public SurfaceButton
{
private:
  PingusMenu* menu;
public:
  ThemeButton(PingusMenu* menu);
  void on_click();

private:
  ThemeButton (const ThemeButton&);
  ThemeButton& operator= (const ThemeButton&);
};

#if 0
class MultiplayerButton : public SurfaceButton
{
private:
  MultiplayerConfig multiplayer_config;
  PingusMenu* menu;
public:
  MultiplayerButton(PingusMenu* menu);
  virtual ~MultiplayerButton();
  void on_click();

private:
  MultiplayerButton (const MultiplayerButton&);
  MultiplayerButton& operator= (const MultiplayerButton&);
};
#endif

} // namespace Pingus

#endif

/* EOF */
