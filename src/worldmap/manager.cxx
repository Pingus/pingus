//  $Id: manager.cxx,v 1.42 2003/12/14 00:30:04 grumbel Exp $
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

#include <iostream>
#include "../gettext.h"
#include "../fonts.hxx"
#include "../gui/screen_manager.hxx"
#include "../gui/surface_button.hxx"
#include "../path_manager.hxx"
#include "../res_descriptor.hxx"
#include "../sound/sound.hxx"
#include "../stat_manager.hxx"
#include "worldmap.hxx"
#include "worldmap_story.hxx"
#include "pingus.hxx"
#include "metamap.hxx"
#include "manager.hxx"
#include "../story_screen.hxx"

namespace WorldMapNS {

WorldMapManager* WorldMapManager::instance_ = 0;

class WorldMapManagerCloseButton
  : public GUI::SurfaceButton
{
public:
  WorldMapManagerCloseButton();
  void on_click();
  void draw (DrawingContext& gc);
  void on_pointer_enter();
};

class WorldMapManagerStoryButton
  : public GUI::SurfaceButton
{
public:
  WorldMapManagerStoryButton();
  void on_click();
  void draw (DrawingContext& gc);
  void on_pointer_enter();
};


class WorldMapManagerCreditsButton
  : public GUI::SurfaceButton
{
public:
  WorldMapManagerCreditsButton();
  void on_click();
  void draw (DrawingContext& gc);
  void on_pointer_enter();
};

class WorldMapManagerEnterButton
  : public GUI::SurfaceButton
{
public:
  WorldMapManagerEnterButton();
  void on_click();
  void draw (DrawingContext& gc);
  void on_pointer_enter();
};


WorldMapManagerCreditsButton::WorldMapManagerCreditsButton()
  : GUI::SurfaceButton(CL_Display::get_width() - 150, 0,
                       ResDescriptor("core/worldmap/credits_button_normal"),
                       ResDescriptor("core/worldmap/credits_button_pressed"),
                       ResDescriptor("core/worldmap/credits_button_hover"))
{
}

void
WorldMapManagerCreditsButton::on_pointer_enter()
{
  SurfaceButton::on_pointer_enter();
  Sound::PingusSound::play_sound ("tick");
}



void
WorldMapManagerCreditsButton::draw (DrawingContext& gc)
{
  SurfaceButton::draw(gc);
  gc.print_left(Fonts::chalk_small, (float)(CL_Display::get_width() - 150 + 15), 5, _("Show Ending?"));
}

void
WorldMapManagerCreditsButton::on_click()
{
	ScreenManager::instance()->replace_screen(
		new StoryScreen(WorldMapManager::instance()->get_worldmap()->get_end_story()), true);
}

WorldMapManagerStoryButton::WorldMapManagerStoryButton()
  : GUI::SurfaceButton(0, 0,
                       ResDescriptor("core/worldmap/story_button_normal"),
                       ResDescriptor("core/worldmap/story_button_pressed"),
                       ResDescriptor("core/worldmap/story_button_hover"))
{
}

void
WorldMapManagerStoryButton::on_pointer_enter()
{
  SurfaceButton::on_pointer_enter();
  Sound::PingusSound::play_sound ("tick");
}



void
WorldMapManagerStoryButton::draw (DrawingContext& gc)
{
  SurfaceButton::draw(gc);
  gc.print_left(Fonts::chalk_small, 10, 5, _("Show Story?"));
}

void
WorldMapManagerStoryButton::on_click()
{
  ScreenManager::instance()->replace_screen(
		new StoryScreen(WorldMapNS::WorldMapManager::instance()->get_worldmap()->get_intro_story()), true);
}

WorldMapManagerCloseButton::WorldMapManagerCloseButton()
  : GUI::SurfaceButton(0, CL_Display::get_height() - 37,
                       ResDescriptor("core/worldmap/leave_button_normal"),
                       ResDescriptor("core/worldmap/leave_button_pressed"),
                       ResDescriptor("core/worldmap/leave_button_hover"))
{
}

void
WorldMapManagerCloseButton::on_pointer_enter()
{
  SurfaceButton::on_pointer_enter();
  Sound::PingusSound::play_sound ("tick");
}

void
WorldMapManagerCloseButton::draw (DrawingContext& gc)
{
  SurfaceButton::draw(gc);
  gc.print_left(Fonts::chalk_small, 10, (float)CL_Display::get_height() - 20, _("Leave?"));
}

void
WorldMapManagerCloseButton::on_click()
{
  ScreenManager::instance ()->pop_screen ();
}

WorldMapManagerEnterButton::WorldMapManagerEnterButton()
  : GUI::SurfaceButton(CL_Display::get_width() - 119, CL_Display::get_height() - 37,
                       ResDescriptor("core/worldmap/enter_button_normal"),
                       ResDescriptor("core/worldmap/enter_button_pressed"),
                       ResDescriptor("core/worldmap/enter_button_hover"))
{
}

void
WorldMapManagerEnterButton::on_pointer_enter()
{
  SurfaceButton::on_pointer_enter();
  if (!WorldMapManager::instance()->get_worldmap()->get_pingus()->is_walking())
    {
      Sound::PingusSound::play_sound ("tick");
    }
}

void
WorldMapManagerEnterButton::draw (DrawingContext& gc)
{
  if (WorldMapManager::instance()->get_worldmap()->get_pingus()->is_walking())
    {
      gc.draw(button_surface, Vector((float)x_pos, (float)y_pos));
    }
  else
    {
      SurfaceButton::draw(gc);
      gc.print_left(Fonts::chalk_small,
                    (float)CL_Display::get_width() - 100,
                    (float)CL_Display::get_height() - 20,
                    _("Enter?"));
    }
}

void
WorldMapManagerEnterButton::on_click()
{
  WorldMapManager::instance()->get_worldmap()->enter_level();
}

WorldMapManager::WorldMapManager ()
  : is_init(false),
		exit_worldmap(false),
		worldmap(0),
    new_worldmap(0),
		metamap(new MetaMap(path_manager.complete("metamap/metamap.xml")))
{
  // FIXME: a bit ugly because of the proteced member, but should work
  // FIXME: well enough. GUIScreen could also use multi-inheritage,
  // FIXME: but that could lead to member function name conflicts
  worldmap_component = new WorldMapComponent();
  gui_manager->add (worldmap_component);
  gui_manager->add(new WorldMapManagerCloseButton());
  gui_manager->add(new WorldMapManagerEnterButton());

  gui_manager->add(new WorldMapManagerStoryButton());
}

void
WorldMapManager::load (const std::string& filename)
{
  if (worldmap)
    delete worldmap;

  worldmap = new WorldMap (filename);
	
  bool credits_unlocked = false;
  StatManager::instance()->get_bool(worldmap->get_shortname() + "-endstory-seen", 
		credits_unlocked);
  if (credits_unlocked)
	{
		gui_manager->add(new WorldMapManagerCreditsButton());
	}
}

void
WorldMapManager::on_startup ()
{
	exit_worldmap = false;
	Sound::PingusSound::stop_music();

	if (!worldmap)
	{
		load(path_manager.complete("worldmaps/" + metamap->get_default_worldmap()));
	}

	if (worldmap)
		worldmap->on_startup ();
}

WorldMapManager::~WorldMapManager ()
{
  delete worldmap;
  delete new_worldmap;
	delete metamap;
}

void
WorldMapManager::on_escape_press ()
{
  //std::cout << "WorldMapManager::on_escape_press ()..." << std::endl;
  exit_worldmap = true;
}

void
WorldMapManager::update (float delta)
{
  UNUSED_ARG(delta);

  // Exit the word
  if (exit_worldmap)
    ScreenManager::instance ()->pop_screen ();

  // Check if new worldmap is set and if so, change it
  if (new_worldmap)
    {
      delete worldmap;
      worldmap     = new_worldmap;
      new_worldmap = 0;
    }
}

void
WorldMapManager::WorldMapComponent::draw (DrawingContext& gc)
{
  WorldMapManager::instance()->worldmap->draw(gc);
}

void
WorldMapManager::WorldMapComponent::update (float delta)
{
  WorldMapManager::instance()->worldmap->update(delta);
  UNUSED_ARG(delta);
}

void
WorldMapManager::WorldMapComponent::on_primary_button_press (int x, int y)
{
  //std::cout << "Buton press" << std::endl;
  /** Fixme: insert Co. translation here */
  WorldMapManager::instance ()->worldmap->on_primary_button_press (x, y);
}


void
WorldMapManager::WorldMapComponent::on_pointer_move (int x, int y)
{
  WorldMapManager::instance ()->worldmap->on_pointer_move (x, y);
}

void
WorldMapManager::WorldMapComponent::on_secondary_button_press (int x, int y)
{
  //std::cout << "Buton press" << std::endl;
  /** Fixme: insert Co. translation here */
  WorldMapManager::instance ()->worldmap->on_secondary_button_press (x, y);
}

void
WorldMapManager::change_map (const std::string& filename, NodeId node)
{
  // Create the new worldmap and make it the current one
  new_worldmap = new WorldMap (path_manager.complete("worldmaps/" + filename));
  new_worldmap->set_pingus (node);
}

WorldMapManager*
WorldMapManager::instance ()
{
  if (instance_)
    return instance_;
  else
    return instance_ = new WorldMapManager ();
}

void WorldMapManager::deinit()
{
	delete instance_;
	instance_ = 0;
}

} // namespace WorldMapNS

/* EOF */
