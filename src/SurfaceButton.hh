//  $Id: SurfaceButton.hh,v 1.11 2000/10/13 12:19:46 grumbel Exp $
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

#ifndef SURFACEBUTTON_HH
#define SURFACEBUTTON_HH

#include <string>

#include <ClanLib/core.h>

#include "PingusMenu.hh"
#include "editor/Editor.hh"
#include "worldmap/PingusWorldMapManager.hh"
#include "ThemeSelector.hh"
#include "PingusMenu.hh"
// #include "FileSelector.hh"

class PingusMenu;

///
class SurfaceButton
{
protected:
  ///
  CL_Surface* surface;
  ///
  CL_Surface* surface_p;
  ///
  CL_Font*    font;
  ///
  CL_Font*    font_large;
  ///
  int x_pos;
  ///
  int y_pos;
  ///
  std::string desc;
  std::string line1;
  std::string line2;
public:
  ///
  SurfaceButton();
  ///
  virtual ~SurfaceButton();
  
  ///
  void draw();
  ///
  bool mouse_over();
  ///
  virtual void on_click() = 0;
}///
;

///
class PlayButton : public SurfaceButton
{
public:
  ///
  PlayButton();
  ///
  virtual ~PlayButton();
  ///
  void on_click();
}///
;

///
class OptionsButton : public SurfaceButton
{
public:
  ///
  OptionsButton();
  ///
  virtual ~OptionsButton();
  ///
  void on_click();
}///
;

///
class QuitButton : public SurfaceButton
{
private:
  ///
  PingusMenu* menu;
public:
  ///
  QuitButton();
  ///
  virtual ~QuitButton();
  ///
  void set_pingus_menu(PingusMenu*);
  ///
  void on_click();
}///
;

///
class LoadButton : public SurfaceButton
{
private:
  ///  FileSelector file;
public:
  LoadButton();
  ///
  virtual ~LoadButton();
  ///
  void on_click();
}///
;

///
class EditorButton : public SurfaceButton
{
private:
  ///
  Editor editor;
public:
  ///
  EditorButton();
  ///
  virtual ~EditorButton();
  ///
  void on_click();
}///
;

///
class ThemeButton : public SurfaceButton
{
private:
  ///
  //ThemeSelector theme_selector;
  PingusWorldMapManager worldmap_manager;
public:
  ///
  ThemeButton();
  ///
  virtual ~ThemeButton();
  ///
  void on_click();
}///
;


#endif

/* EOF */
