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
#include "../fonts.hpp"
#include "../gui/display.hpp"
#include "../gui/screen_manager.hpp"
#include "../gui/surface_button.hpp"
#include "../path_manager.hpp"
#include "../res_descriptor.hpp"
#include "../sound/sound.hpp"
#include "../stat_manager.hpp"
#include "../display/scene_context.hpp"
#include "../math.hpp"
#include "worldmap.hpp"
#include "resource.hpp"
#include "worldmap_story.hpp"
#include "pingus.hpp"
#include "metamap.hpp"
#include "manager.hpp"
#include "../story_screen.hpp"

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
  : GUI::SurfaceButton(Display::get_width() - 150, 0,
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
  gc.print_left(Fonts::chalk_small, (float)(Display::get_width() - 150 + 15), 2, _("Show Ending?"));
}

void
WorldMapManagerCreditsButton::on_click()
{
  ScreenManager::instance()->replace_screen
    (new StoryScreen(WorldMapManager::instance()->get_worldmap()->get_end_story()), true);
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
  gc.print_left(Fonts::chalk_small, 18, 2, _("Show Story?"));
}

void
WorldMapManagerStoryButton::on_click()
{
  ScreenManager::instance()->replace_screen
    (new StoryScreen(WorldMapNS::WorldMapManager::instance()->get_worldmap()->get_intro_story()), true);
}

WorldMapManagerCloseButton::WorldMapManagerCloseButton()
  : GUI::SurfaceButton(0, Display::get_height() - 37,
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
  gc.print_left(Fonts::chalk_small, 24, (float)Display::get_height() - 25, _("Leave?"));
}

void
WorldMapManagerCloseButton::on_click()
{
  ScreenManager::instance ()->pop_screen ();
}

WorldMapManagerEnterButton::WorldMapManagerEnterButton()
  : GUI::SurfaceButton(Display::get_width() - 119, Display::get_height() - 37,
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
      gc.draw(button_surface, Vector3f((float)x_pos, (float)y_pos));
    }
  else
    {
      SurfaceButton::draw(gc);
      gc.print_right(Fonts::chalk_small,
                    (float)Display::get_width() - 40,
                    (float)Display::get_height() - 25,
                    _("Enter"));
    }
}

void
WorldMapManagerEnterButton::on_click()
{
  WorldMapManager::instance()->get_worldmap()->enter_level();
}

WorldMapManager::WorldMapManager ()
  : levelname_bg(Resource::load_sprite("core/worldmap/levelname_bg")),
    is_init(false),
    exit_worldmap(false),
    worldmap(0),
    new_worldmap(0)
    ////metamap(new MetaMap(path_manager.complete("metamap/metamap.xml")))
{
  // FIXME: a bit ugly because of the proteced member, but should work
  // FIXME: well enough. GUIScreen could also use multi-inheritage,
  // FIXME: but that could lead to member function name conflicts
  worldmap_component = new WorldMapComponent();
  gui_manager->add (worldmap_component, true);
  gui_manager->add(new WorldMapManagerCloseButton(), true);
  gui_manager->add(new WorldMapManagerEnterButton(), true);

  gui_manager->add(new WorldMapManagerStoryButton(), true);
}

void
WorldMapManager::load (const std::string& filename)
{
  if (worldmap)
    delete worldmap;

  worldmap = new WorldMap(filename);
	
  bool credits_unlocked = false;
  StatManager::instance()->get_bool(worldmap->get_shortname() + "-endstory-seen", 
		credits_unlocked);
  if (credits_unlocked)
	{
		gui_manager->add(new WorldMapManagerCreditsButton(), true);
	}
}

void
WorldMapManager::on_startup ()
{
  exit_worldmap = false;
  Sound::PingusSound::stop_music();

#if 0
  if (!worldmap)
    {
      load(path_manager.complete("worldmaps/" + metamap->get_default_worldmap()));
    }
#endif

  if (worldmap)
    worldmap->on_startup ();
}

WorldMapManager::~WorldMapManager ()
{
  delete worldmap;
  delete new_worldmap;
  ////delete metamap;
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

WorldMapComponent::WorldMapComponent()
{
  scene_context = new SceneContext();
}

WorldMapComponent::~WorldMapComponent()
{
  delete scene_context;
}

void
WorldMapComponent::draw (DrawingContext& gc)
{
  WorldMap* worldmap = WorldMapManager::instance()->worldmap;

  Rect cliprect(Vector2i(Math::max((Display::get_width()  - worldmap->get_width())/2,  0),
                         Math::max((Display::get_height() - worldmap->get_height())/2, 0)), 
                Size(Math::min(Display::get_width(),  worldmap->get_width()),
                 Math::min(Display::get_height(), worldmap->get_height())));

  scene_context->clear();
  scene_context->push_modelview();
  scene_context->translate((float)cliprect.left, (float)cliprect.top);

  scene_context->set_cliprect(cliprect);

  //scene_context->color().draw_fillrect(-100, -100, 2000, 2000, Color(255,0,0,0), -10000);
  worldmap->draw(scene_context->color());

  gc.draw(new SceneContextDrawingRequest(scene_context, Vector3f(0,0,-1000)));

  scene_context->pop_modelview();

  // Draw border
  if (cliprect != Rect(Vector2i(0,0), Size(Display::get_width(), Display::get_height())))
  {
    Color border_color(50, 65, 75);
    // top
    gc.draw_fillrect(0, 0, (float)Display::get_width(), (float)cliprect.top,
                     border_color);
    // bottom
    gc.draw_fillrect(0, (float)cliprect.bottom, (float)Display::get_width(), (float)Display::get_height(),
                 border_color);
    // left
    gc.draw_fillrect(0, (float)cliprect.top, (float)cliprect.left, (float)cliprect.bottom,
                 border_color);
    // right
    gc.draw_fillrect((float)cliprect.right, (float)cliprect.top, (float)Display::get_width(), (float)cliprect.bottom,
                 border_color);
  }
}

void
WorldMapComponent::update (float delta)
{
  WorldMapManager::instance()->worldmap->update(delta);
  UNUSED_ARG(delta);
}

void
WorldMapManager::draw_foreground(DrawingContext& gc)
{
  // Draw the levelname
  gc.draw(levelname_bg,
          Vector3f(gc.get_width()/2 - levelname_bg.get_width()/2,
                   gc.get_height() - levelname_bg.get_height()));

  gc.print_center(Fonts::chalk_small, gc.get_width()/2, gc.get_height() - 25,
                  worldmap->get_levelname());
}

void
WorldMapComponent::on_primary_button_press (int x, int y)
{
  WorldMap* worldmap = WorldMapManager::instance ()->worldmap;
  Rect cliprect(Vector2i(Math::max((Display::get_width()  - worldmap->get_width())/2,  0),
                         Math::max((Display::get_height() - worldmap->get_height())/2, 0)), 
                Size(Math::min(Display::get_width(),  worldmap->get_width()),
                 Math::min(Display::get_height(), worldmap->get_height())));

  WorldMapManager::instance ()->worldmap->on_primary_button_press(x - cliprect.left,
                                                                  y - cliprect.top);
}


void
WorldMapComponent::on_pointer_move (int x, int y)
{
  WorldMap* worldmap = WorldMapManager::instance ()->worldmap;
  Rect cliprect(Vector2i(Math::max((Display::get_width()  - worldmap->get_width())/2,  0),
                         Math::max((Display::get_height() - worldmap->get_height())/2, 0)), 
                Size(Math::min(Display::get_width(),  worldmap->get_width()),
                 Math::min(Display::get_height(), worldmap->get_height())));

  WorldMapManager::instance ()->worldmap->on_pointer_move(x - cliprect.left,
                                                          y - cliprect.top);
}

void
WorldMapComponent::on_secondary_button_press (int x, int y)
{
  WorldMap* worldmap = WorldMapManager::instance ()->worldmap;
  Rect cliprect(Vector2i(Math::max((Display::get_width()  - worldmap->get_width())/2,  0),
                         Math::max((Display::get_height() - worldmap->get_height())/2, 0)), 
                Size(Math::min(Display::get_width(),  worldmap->get_width()),
                 Math::min(Display::get_height(), worldmap->get_height())));

  WorldMapManager::instance ()->worldmap->on_secondary_button_press(x - cliprect.left,
                                                                    y - cliprect.top);
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
