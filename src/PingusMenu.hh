//  $Id: PingusMenu.hh,v 1.9 2000/10/18 20:16:36 grumbel Exp $
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

#ifndef PINGUSMENU_HH
#define PINGUSMENU_HH

#include <list>
#include <ClanLib/core.h>

#include "SurfaceButton.hh"
#include "OptionMenu.hh"
#include "AlphaButton.hh"
#include "LayerManager.hh"

///
class SurfaceButton;
class PlayButton;
class OptionsButton;
class QuitButton;
class EditorButton;
class ThemeButton;

class PingusMenu
{
public:
  ///
  enum PingusMenuItem { pmiSTART, pmiLOAD, pmiQUIT, pmiVOID }///
  ;
  
private:
  ///
  thSlot on_button_press_slot;
  ///
  thSlot on_button_release_slot;
  ///
  thSlot on_mouse_move_slot;
  //
  thSlot on_resize_slot;
  ///
  class Event /*: 
    public CL_Event_ButtonPress, 
    public CL_Event_ButtonRelease, 	
    public CL_Event_MouseMove*/
  {
  public:
    ///
    bool enabled;
    ///
    PingusMenu* menu;
    ///
    virtual void on_button_press(CL_InputDevice *device, const CL_Key &key);
    ///
    virtual void on_button_release(CL_InputDevice *device, const CL_Key &key);
    ///
    virtual void on_mouse_move(CL_InputDevice *,int mouse_x, int mouse_y);
  };

  ///
  friend class Event;
  ///
  Event* event;

  ///
  std::list<SurfaceButton*> buttons;
  ///
  SurfaceButton* temp_button;
  ///
  SurfaceButton* current_button;
  
  ///
  CL_Surface* bg;
  ///
  CL_Surface* background;
  ///
  LayerManager layer_manager;
  ///
  CL_Surface* cursor_sur;

  ///
  int start_x, start_y;
  ///
  int load_x, load_y;
  ///
  int quit_x, quit_y;
  ///
  int temp_mouse_x, temp_mouse_y;
  ///
  PingusMenuItem current_item;
  
  ///
  PlayButton*     play_button;
  ///
  OptionsButton*  options_button;
  ///
  QuitButton*     quit_button;
  ///
  EditorButton*   editor_button;
  ///
  ThemeButton*    theme_button;
  

  void on_resize(int w, int h);
public:
  ///
  bool do_quit;

  ///
  PingusMenu();
  ///
  ~PingusMenu();

  ///
  void draw(void);
  ///
  void select(void);
}///
;

#endif

/* EOF */
