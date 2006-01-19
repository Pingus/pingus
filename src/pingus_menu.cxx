//  $Id: pingus_menu.cxx,v 1.22 2003/10/19 12:25:47 grumbel Exp $
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

#include <ClanLib/Display/display.h>
#include <ClanLib/core.h>
#include <config.h>
#include "gettext.h"
#include "components/menu_button.hxx"
#include "resource.hxx"
#include "debug.hxx"
#include "globals.hxx"
#include "sound/sound.hxx"
#include "stat_manager.hxx"
#include "start_screen.hxx"
#include "story_screen.hxx"
#include "worldmap/worldmap.hxx"
#include "worldmap/manager.hxx"
#include "gui/screen_manager.hxx"
#include "pingus_menu_manager.hxx"
#include "gui/gui_manager.hxx"
#include "plf_res_mgr.hxx"
#include "path_manager.hxx"
#include "file_dialog.hxx"
#include "editor/editor_screen.hxx"

namespace Pingus {

PingusMenu::PingusMenu (PingusMenuManager* m)
  : PingusSubMenu (m),
	  filedialog(0)
{
  is_init = false;
    
  start_button = new MenuButton(CL_Point(CL_Display::get_width() * 400 / 800,
                                         CL_Display::get_height() * 450 / 600),
                                Resource::load_sprite("core/menu/play_on"),
                                _("Start"),
                                _("..:: Start the game ::.."));
  
  quit_button = new MenuButton(CL_Point(CL_Display::get_width() * 650 / 800,
                                        CL_Display::get_height() * 450 / 600),
                               Resource::load_sprite("core/menu/exit_on"),
                               _("Exit"),
                               _("..:: Bye, bye ::.."));

  contrib_button = new MenuButton(CL_Point(CL_Display::get_width() * 150 / 800,
                                          CL_Display::get_height() * 450 / 600),
                                  Resource::load_sprite("core/menu/options_on"),
                                  _("Contrib\nLevels"),
                                  _("..:: Play User Build levels ::.."));

  story_button  = new MenuButton(CL_Point(CL_Display::get_width() * 400 / 800,
                                          CL_Display::get_height() * 340 / 600),
                                 Resource::load_sprite("core/menu/credits_on"),
                                 _("Story"),
                                 _("..:: Start the story ::.."));
  
  multiplayer_button = new MenuButton(CL_Point(CL_Display::get_width() * 150 / 800,
                                               CL_Display::get_height() * 340 / 600),
                                      Resource::load_sprite("core/menu/multi_on"),
                                      _("Multiplayer"),
                                      _("..:: Multiplayer Match ::.."));

	editor_button = new MenuButton(CL_Point(CL_Display::get_width() * 400 / 800,
                                               CL_Display::get_height() * 450 / 600),
                                      Resource::load_sprite("core/menu/create_on"),
                                      _("Level Editor"),
                                      _("..:: Create your own levels ::.."));

  slots.push_back(start_button->sig_click().connect(this, &PingusMenu::setup_game_menu));
  slots.push_back(quit_button->sig_click().connect(this, &PingusMenu::do_quit));

  slots.push_back(story_button->sig_click().connect(this, &PingusMenu::setup_worldmap_menu));
  slots.push_back(multiplayer_button->sig_click().connect(this, &PingusMenu::setup_main_menu));
  
  slots.push_back(contrib_button->sig_click().connect(this, &PingusMenu::setup_contrib_menu));
	slots.push_back(editor_button->sig_click().connect(this, &PingusMenu::do_edit));
}

void
PingusMenu::setup_main_menu()
{
  gui_manager->remove(contrib_button);
  gui_manager->remove(story_button);
  gui_manager->remove(multiplayer_button);
	gui_manager->remove(editor_button);

  gui_manager->add(quit_button);
  gui_manager->add(start_button);
}

void
PingusMenu::setup_game_menu()
{
  gui_manager->remove(start_button);

  gui_manager->add(contrib_button);
  gui_manager->add(story_button);
	gui_manager->add(editor_button);
	// FIXME: Re-enable this next line once multiplayer functionality
	// is actually available.
  //gui_manager->add(multiplayer_button);
}

void
PingusMenu::setup_contrib_menu()
{
	if (filedialog)
		delete filedialog;
	filedialog = new FileDialog(this, ".pingus", 
		path_manager.complete("levels/"), true);
  manager->push_menu (filedialog);
}

void
PingusMenu::setup_worldmap_menu()
{
	if (filedialog)
		delete filedialog;
	filedialog = new FileDialog(this, ".xml", 
		path_manager.complete("worldmaps/"), true);
  manager->push_menu (filedialog);
}

void
PingusMenu::preload ()
{
  if (!is_init)
    {
      is_init = true;

      background = Resource::load_sprite("core/misc/logo");
      setup_main_menu();
    }
}

PingusMenu::~PingusMenu()
{
	delete start_button;
	delete quit_button;
	delete contrib_button;
	delete story_button;
	delete multiplayer_button;
	delete editor_button;
	if (filedialog)
		delete filedialog;
}

void
PingusMenu::do_quit()
{
  get_manager ()->show_exit_menu ();
}

void
PingusMenu::do_start(const std::string &filename)
{ // Start the story or worldmap mode
  Sound::PingusSound::play_sound ("letsgo");
	WorldMapNS::WorldMapManager::instance()->load(filename);
  
  bool story_seen = false;
	StatManager::instance()->get_bool(WorldMapNS::WorldMapManager::instance()->
		get_worldmap()->get_shortname() + "-startstory-seen", story_seen);
	
	if (!story_seen)
		ScreenManager::instance()->push_screen(new StoryScreen(
			WorldMapNS::WorldMapManager::instance()->get_worldmap()->get_intro_story()), true);
  else
	  ScreenManager::instance()->push_screen(WorldMapNS::WorldMapManager::instance());
}

void PingusMenu::do_contrib(const std::string &levelfile)
{ // Launch the specified level - don't bother checking for it, it has to exist
  Sound::PingusSound::play_sound ("letsgo");
  ScreenManager::instance()->push_screen
     (new StartScreen(PLFResMgr::load_plf_from_filename(levelfile)),
      true);
}

void PingusMenu::do_edit()
{	// Launch the level editor
	Sound::PingusSound::stop_music();
	ScreenManager::instance()->push_screen (new Editor::EditorScreen());
}

void
PingusMenu::on_resize(int w, int h)
{
  pout << "Width: " << w << " Height: " << h << std::endl;
}

void
PingusMenu::on_escape_press ()
{
  get_manager ()->show_exit_menu ();
}

void
PingusMenu::draw_foreground(DrawingContext& gc)
{
  if (gc.get_height() == 480)
    {
      gc.draw(background,
              Vector((gc.get_width()/2) - (background.get_width()/2),
                     20.0f));
    }
  else
    {
      gc.draw(background, 
              Vector((gc.get_width()/2) - (background.get_width()/2),
							static_cast<float>(CL_Display::get_height()/10)));
    }
#ifdef OFFICIAL_PINGUS_BUILD
  gc.print_left(Fonts::pingus_small, 20.0f, 
								static_cast<float>(CL_Display::get_height()-100),
                "Pingus version "VERSION", Copyright (C) 2003 Ingo Ruhnke <grumbel@gmx.de>\n");
#else
  gc.print_left(Fonts::pingus_small, 20.0f, 
								static_cast<float>(CL_Display::get_height()-100),
                "Pingus version "VERSION" (unofficial build), Copyright (C) 2003 Ingo Ruhnke <grumbel@gmx.de>\n");
#endif
  gc.print_left(Fonts::pingus_small, 20.0f, 
								static_cast<float>(CL_Display::get_height()-70),
                "Pingus comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome\n"
                "to redistribute it under certain conditions; see the file COPYING for details.\n");
}

void
PingusMenu::load(const std::string &file, const std::string &filemask)
{
	// Level
	if (filemask == ".pingus")
		do_contrib(file);
	// Worldmap
	else if (filemask == ".xml")
		do_start(file);
	manager->pop_menu();
}
		
void
PingusMenu::cancel()
{
	manager->pop_menu();
}

} // namespace Pingus

/* EOF */
