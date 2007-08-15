//  $Id: worldmap.cxx,v 1.53 2003/12/14 00:30:04 grumbel Exp $
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

#include <assert.h>
#include <iostream>
#include "../gui/display.hpp"
#include "../fonts.hpp"
#include "../path_manager.hpp"
#include "../stat_manager.hpp"
#include "../savegame_manager.hpp"
#include "../system.hpp"
#include "../resource.hpp"
#include "../globals.hpp"
#include "../sound/sound.hpp"
#include "../pingus_error.hpp"
#include "../gettext.h"
#include "../globals.hpp"
#include "../sexpr_file_reader.hpp"
#include "../debug.hpp"
#include "../lisp/parser.hpp"
#include "../lisp/lisp.hpp"
#include "worldmap.hpp"
#include "worldmap_story.hpp"
#include "metamap.hpp"
#include "manager.hpp"
#include "pingus.hpp"
#include "drawable_factory.hpp"
#include "drawable.hpp"
#include "dot.hpp"
#include "level_dot.hpp"
#include "path_graph.hpp"
#include "../math.hpp"
#include "../stat_manager.hpp"

#include "../story_screen.hpp"
#include "../gui/screen_manager.hpp"

namespace WorldMapNS {

WorldMap::WorldMap(const std::string& arg_filename)
  : filename(arg_filename),
    mouse_x(0),
    mouse_y(0)
{
  parse_file(FileReader::parse(filename));

  pingus = new Pingus(path_graph);
  set_starting_node();
  add_drawable(pingus);

  gc_state.set_limit(Rect(Vector2i(0, 0), Size(width, height)));
}

WorldMap::~WorldMap()
{
  for (DrawableLst::iterator i = drawables.begin (); i != drawables.end (); ++i)
    {
      delete (*i);
    }
  delete intro_story;
  delete end_story;
  delete path_graph;
}

void
WorldMap::parse_file(FileReader reader)
{
  if (reader.get_name() == "pingus-worldmap")
    {
      parse_graph(reader.read_section("graph"));
      parse_objects(reader.read_section("objects"));
      parse_properties(reader.read_section("head"));
      intro_story = new WorldMapStory(reader.read_section("intro_story"));
      end_story   = new WorldMapStory(reader.read_section("end_story"));
    }
  else
    {
      PingusError::raise("WorldMap:" + filename + ": not a Worldmap file");
    }

  if (!path_graph)
    {
      PingusError::raise("WorldMap: " + filename + " missed Graph");
    }
}

void
WorldMap::parse_objects(FileReader reader)
{
  const std::vector<FileReader>& childs = reader.get_sections();
  
  for(std::vector<FileReader>::const_iterator i = childs.begin(); 
      i != childs.end(); ++i)
    {
      Drawable* drawable = DrawableFactory::create(*i);

      if (drawable)
        {
          objects.push_back(drawable);
          drawables.push_back(drawable);
        }
      else
        {
          std::cout << "WorldMap::parse_objects: Parse Error" << std::endl;
        }
    }
}

void
WorldMap::parse_graph(FileReader reader)
{
  path_graph = new PathGraph(this, reader);
}

void
WorldMap::parse_properties(FileReader reader)
{
  reader.read_string("music", music);
  reader.read_string("author", author);
  reader.read_string("name", name);
  reader.read_string("short-name", short_name);
  reader.read_string("email", email);
  reader.read_int("width", width);
  reader.read_int("height", height);

  // Get beginning and ending nodes.
  std::string node_name;
  reader.read_string("default-node", node_name);
  default_node = path_graph->lookup_node(node_name);
  reader.read_string("final-node", node_name);
  final_node = path_graph->lookup_node(node_name);
}

void
WorldMap::draw(DrawingContext& gc)
{
  Vector3f pingu_pos = pingus->get_pos();
  float min, max;

  if (width >= gc.get_width())
    {
      min = float(gc.get_width()/2);
      max = float(width - gc.get_width()/2);
    }
  else
    {
      min = float(width - gc.get_width()/2);
      max = float(gc.get_width()/2);
    }
  pingu_pos.x = Math::clamp(min, pingu_pos.x, max);

  if (height >= gc.get_height())
    {
      min = float(gc.get_height()/2);
      max = float(height - gc.get_height()/2);
    }
  else
    {
      min = float(height - gc.get_height()/2);
      max = float(gc.get_height()/2);
    }
  pingu_pos.y = Math::clamp(min, pingu_pos.x, max);

  gc_state.set_pos(Vector2f(pingu_pos.x, pingu_pos.y));
	
  gc_state.push(gc);
  
  
  for (DrawableLst::iterator i = drawables.begin (); i != drawables.end (); ++i)
    (*i)->draw(gc);

  Vector3f mpos = gc.screen_to_world(Vector3f((float)mouse_x, (float)mouse_y));
  Dot* dot = path_graph->get_dot(mpos.x, mpos.y);
  if (dot)
    dot->draw_hover(gc);
  
  gc_state.pop(gc);
}

void
WorldMap::update (float delta)
{
  for (DrawableLst::iterator i = drawables.begin (); i != drawables.end (); ++i)
    {
      (*i)->update (delta);
    }
}

void
WorldMap::on_startup()
{
  Sound::PingusSound::play_music(music);
  update_locked_nodes();
}

void
WorldMap::add_drawable(Drawable* drawable)
{
  drawables.push_back(drawable);
}

void
WorldMap::remove_drawable(Drawable* drawable)
{
  UNUSED_ARG(drawable);
}

void
WorldMap::set_pingus(NodeId id)
{
  UNUSED_ARG(id);
}

void
WorldMap::on_pointer_move(int x, int y)
{
  mouse_x = x;
  mouse_y = y;
}

void
WorldMap::on_primary_button_press(int x, int y)
{
  Vector2f click_pos = gc_state.screen2world(Vector2i(x, y));

  if (pingus_debug_flags & PINGUS_DEBUG_WORLDMAP)
    {
      std::cout
        << "\n<leveldot>\n"
        << "  <dot>\n"
        << "    <name>leveldot_X</name>\n"
        << "    <position>\n"
        << "      <x>" << (int)click_pos.x << "</x>\n"
        << "      <y>" << (int)click_pos.y << "</y>\n"
        << "      <z>0</z>\n"
        << "    </position>\n"
        << "  </dot>\n"
        << "  <levelname>level10.pingus</levelname>\n"
        << "</leveldot>\n" << std::endl;
    }

  Dot* dot = path_graph->get_dot(click_pos.x, click_pos.y);
  if (dot)
    {
      if (maintainer_mode)
        std::cout << "WorldMap: Clicked on: " << dot->get_name() << std::endl;

      if (path_graph->lookup_node(dot->get_name()) == pingus->get_node())
        {
          if (maintainer_mode)
            std::cout << "WorldMap: Pingu is on node, issue on_click()" << std::endl;
          dot->on_click();
        }
      else
        {
          if (dot->accessible())
            {
              if (!pingus->walk_to_node(path_graph->lookup_node(dot->get_name())))
                {
                  if (maintainer_mode)
                    std::cout << "WorldMap: NO PATH TO NODE FOUND!" << std::endl;
                }
              else
                {
                  StatManager::instance()->set_string(short_name + "-current-node", dot->get_name());
                }
            }
          else
            {
              Sound::PingusSound::play_sound("chink");
            }
        }
    }
}

void
WorldMap::on_secondary_button_press(int x, int y)
{
  if (maintainer_mode)
    {
      Vector3f click_pos = gc_state.screen2world(Vector2i(x, y));

      Dot* dot = path_graph->get_dot(click_pos.x, click_pos.y);
      if (dot)
        { // FIXME: Dot NodeID missmatch...
          NodeId id = path_graph->get_id(dot);
          pingus->set_position(id);
        }
    }
}

void
WorldMap::enter_level()
{
  NodeId node = get_pingus()->get_node();

  if (node != NoNode)
    {
      Dot* dot = path_graph->get_dot(node);
      if (dot)
        {
          dot->on_click();
        }
    }
  else
    {
      if (maintainer_mode)
        std::cout << "WorldMap: Pingus not on level" << std::endl;
    }
}

struct unlock_nodes
{
  PathGraph* path_graph;
  unlock_nodes(PathGraph* g)
  {
    path_graph = g;
  }

  void operator()(Node<Dot*>& node)
  {
    if (node.data->finished())
      {
        //std::cout << "Unlocking neightbours of: " << node.data << std::endl;
        for (std::vector<EdgeId>::iterator i = node.next.begin(); i != node.next.end(); ++i)
          {
            Edge<Path*>& edge = path_graph->graph.resolve_edge(*i);

            // FIXME: This should be identical to node.data->unlock(), but not sure
            path_graph->graph.resolve_node(edge.source).data->unlock();
            path_graph->graph.resolve_node(edge.destination).data->unlock();
          }
      }
  }
};

void
WorldMap::update_locked_nodes()
{ // FIXME: This shouldn't be a polling function
  path_graph->graph.for_each_node(unlock_nodes(path_graph));

  bool credits_unlocked = false;
  StatManager::instance()->get_bool(short_name + "-endstory-seen", credits_unlocked);

  if (!credits_unlocked)
    {
      // See if the last level is finished
      Dot* dot = path_graph->get_dot(final_node);
      if (dot)
        {
          if (dot->finished())
            {
              ////WorldMapManager::instance()->get_metamap()->finish_node(short_name);
              ScreenManager::instance()->replace_screen(new StoryScreen(get_end_story()), true);
            }
        }
      else
        {
          std::cout << "Error: WorldMap: Last level missing" << std::endl;
        }
    }
}

// Determine starting node
void
WorldMap::set_starting_node()
{
  // See if the user has played this map before.  If not, use the <default-node>
  // tag from the XML file.
  NodeId id;
  std::string node_name;

  if (StatManager::instance()->get_string(short_name + "-current-node", node_name))
    {
      // Just in case that level doesn't exist, look it up.
      id = path_graph->lookup_node(node_name);
      if (id == NoNode)
        id = default_node;
    }
  else
    id = default_node;
		
  pingus->set_position(id);

  LevelDot* leveldot = dynamic_cast<LevelDot*>(path_graph->get_dot(id));
  leveldot->unlock();
}

bool
WorldMap::is_final_map()
{
  return false; ////(WorldMapManager::instance()->get_metamap()->get_final_worldmap() == short_name);
}

std::string
WorldMap::get_levelname()
{
  if (pingus->get_node() != NoNode)
    {
      LevelDot* leveldot = dynamic_cast<LevelDot*>(path_graph->get_dot(pingus->get_node()));
      if (leveldot)
        return _(leveldot->get_plf().get_levelname());
      else 
        return "---";
    }
  else
    {
      return _("...walking...");
    }
}

} // namespace WorldMapNS

/* EOF */
