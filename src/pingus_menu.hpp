//  $Id$
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

#ifndef HEADER_PINGUS_PINGUS_MENU_HXX
#define HEADER_PINGUS_PINGUS_MENU_HXX

#include <vector>
#include "fonts.hpp"
#include "pingus_sub_menu.hpp"
#include "file_dialog_listener.hpp"
#include "layer_manager.hpp"

class SurfaceButton;
class GameDelta;
class FileDialog;

namespace GUI {
class GUIManager;
} // namespace GUI

class MenuButton;

class PingusMenu : public PingusSubMenu, public FileDialogListener
{
public:
  bool is_init;
private:
  Sprite background;
  LayerManager layer_manager;

  MenuButton* start_button;
  MenuButton* quit_button;
  MenuButton* credits_button;

#ifdef NEW_MENU
  MenuButton* contrib_button;
  MenuButton* story_button;
  MenuButton* multiplayer_button;
  MenuButton* editor_button;
#endif
  FileDialog* filedialog;
  
  void on_resize (int w, int h);

  // These functions setup the different menus
  void setup_main_menu();
#ifdef NEW_MENU
  void setup_game_menu();
#endif
  void show_credits();
  /** Show the levels folder */
  void setup_contrib_menu();
  /** Show the worldmap folder */
  void setup_worldmap_menu();

  /** Quit the game */
  void do_quit();
  /** Start the story/worldmap mode */
  void do_start(const std::string &filename);
  /** Start the level editor */
  void do_edit();

  /** Use this to load the level or worldmap */
  virtual void load(const std::string &file, const std::string &filemask);

  /** Cancels the file dialog box */
  virtual void cancel();

public:
  PingusMenu (PingusMenuManager* m);
  ~PingusMenu();

  void on_click(MenuButton* button);

  /// Load all images and other stuff for the menu
  void preload ();
  void do_contrib(const std::string&);

  void on_escape_press ();
  void draw_foreground(DrawingContext& gc);
private:
  PingusMenu (const PingusMenu&);
  PingusMenu& operator= (const PingusMenu&);
};


#endif

/* EOF */
