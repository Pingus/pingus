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
#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/display.h>
#include <ClanLib/Display/mouse.h>
#include "../fonts.hxx"
#include "../path_manager.hxx"
#include "../stat_manager.hxx"
#include "../system.hxx"
#include "../resource.hxx"
#include "../globals.hxx"
#include "../sound/sound.hxx"
#include "../xml_helper.hxx"
#include "../pingus_error.hxx"
#include "../gettext.h"
#include "pingus.hxx"
#include "../globals.hxx"
#include "../debug.hxx"
#include "worldmap.hxx"
#include "drawable_factory.hxx"
#include "drawable.hxx"
#include "dot.hxx"
#include "level_dot.hxx"
#include "path_graph.hxx"
#include "../math.hxx"
#include "../stat_manager.hxx"

#include "../story.hxx"
#include "../story_screen.hxx"
#include "../gui/screen_manager.hxx"

namespace Pingus {
namespace WorldMapNS {

struct z_pos_sorter
{
  bool operator()(Drawable* a, Drawable* b)
  {
    return a->get_z_pos() < b->get_z_pos();
  }
};

WorldMap::WorldMap(const std::string& arg_filename)
  : /*display_gc (0, 0, CL_Display::get_width()-1, CL_Display::get_height()-1,
      0, 0),*/
    filename(arg_filename),
    width(1161), height(600), // FIXME: ugly..
    mouse_x(0),
    mouse_y(0)
{

  xmlDocPtr doc = xmlParseFile(filename.c_str());

  if (!doc)
    {
      PingusError::raise (_("WorldMap: File not found: ") + filename);
    }

  xmlNodePtr cur = doc->ROOT;
  cur = XMLhelper::skip_blank(cur);

  parse_file(doc, cur);

  pingus = new Pingus(path_graph);
  //std::cout << "PingusPtr: " << pingus << std::endl;

  std::string node;
  if (StatManager::instance()->get_string("current-tutorial-node", node))
    {
      NodeId id = path_graph->lookup_node(node);
      if (id == NoNode)
        {
          pingus->set_position(0);
        }
      else
        {
          pingus->set_position(id);
        }
    }
  else
    {  // FIXME: This should not be hardcoded, but instead be noted in the
      // savegame or worldmap
      pingus->set_position(0);
    }

  add_drawable(pingus);

  levelname_bg = Resource::load_sprite("core/worldmap/levelname_bg");
  xmlFreeDoc(doc);
}

WorldMap::~WorldMap()
{
  for (DrawableLst::iterator i = drawables.begin (); i != drawables.end (); ++i)
    {
      delete (*i);
    }
//  for (ObjectLst::iterator j = objects.begin (); j != objects.end (); ++j)
//    {
//      delete (*j);
//    }

//  delete pingus;
  delete path_graph;
}

void
WorldMap::parse_file(xmlDocPtr doc, xmlNodePtr cur)
{
  if (cur && XMLhelper::equal_str(cur->name, "pingus-worldmap"))
    {
      cur = cur->children;
      cur = XMLhelper::skip_blank(cur);

      while (cur)
	{
	  if (XMLhelper::equal_str(cur->name, "graph"))
	    {
	      parse_graph(doc, cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "objects"))
	    {
	      parse_objects(doc, cur);
	    }
	  else if (XMLhelper::equal_str(cur->name, "properties"))
	    {
	      parse_properties(doc, cur);
	    }
	  else
	    {
	      perr(PINGUS_DEBUG_WORLDMAP) << "WorldMap: Unknown node name: " << cur->name << std::endl;
	    }

	  cur = cur->next;
	  cur = XMLhelper::skip_blank(cur);
	}
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
WorldMap::parse_objects(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);

  while (cur)
    {
      Drawable* drawable = DrawableFactory::create(doc, cur);

      if (drawable)
        {
          objects.push_back(drawable);
          drawables.push_back(drawable);
        }
      else
        {
          std::cout << "WorldMap::parse_objects: Parse Error" << std::endl;
        }

      cur = cur->next;
      cur = XMLhelper::skip_blank(cur);
    }
}

void
WorldMap::parse_graph(xmlDocPtr doc, xmlNodePtr cur)
{
  path_graph = new PathGraph(this, doc, cur);
}

void
WorldMap::parse_properties(xmlDocPtr doc, xmlNodePtr cur)
{
  cur = cur->children;
  cur = XMLhelper::skip_blank(cur);

  UNUSED_ARG(doc);
}

void
WorldMap::draw (DrawingContext& gc)
{
  Vector pingu_pos = pingus->get_pos();

  pingu_pos.x = Math::mid(float(gc.get_width()/2),
                          pingu_pos.x,
                          float(width - gc.get_width()/2));

  pingu_pos.y = Math::mid(float(gc.get_height()/2),
                          pingu_pos.y,
                          float(height - gc.get_height()/2));

  DrawingContext* display_gc = new DrawingContext();

  display_gc->translate(-pingu_pos.x, -pingu_pos.y);

  std::stable_sort(drawables.begin(), drawables.end(), z_pos_sorter());

  for (DrawableLst::iterator i = drawables.begin (); i != drawables.end (); ++i)
    {
      (*i)->draw(*display_gc);
    }

  gc.draw(levelname_bg,
          Vector(gc.get_width()/2 - levelname_bg.get_width()/2,
                 gc.get_height() - levelname_bg.get_height()));

  if (pingus->get_node() != NoNode)
    {
      LevelDot* leveldot = dynamic_cast<LevelDot*>(path_graph->get_dot(pingus->get_node()));

      if (leveldot)
        {
          gc.print_center(Fonts::chalk_small,
                          display_gc->get_width ()/2,
                          display_gc->get_height() - 20,
                          System::translate(leveldot->get_plf().get_levelname()));
          
        }
      else
        {
          gc.print_center(Fonts::chalk_small,
                          display_gc->get_width ()/2,
                          display_gc->get_height() - 20,
                          "---");
        }
    }
  else
    {
          gc.print_center(Fonts::chalk_small,
                          display_gc->get_width ()/2,
                          display_gc->get_height() - 20,
                          _("...walking..."));
    }

  Vector mpos = display_gc->screen_to_world(Vector(mouse_x, mouse_y));
  Dot* dot = path_graph->get_dot(mpos.x, mpos.y);
  if (dot)
    {
      dot->draw_hover(*display_gc);
    }
  gc.draw(display_gc);
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
  //FIXME: const Vector& click_pos = display_gc->screen_to_world(Vector(x, y));
  Vector click_pos(x, y);

  if (pingus_debug_flags & PINGUS_DEBUG_WORLDMAP)
    {
      std::cout
        << "\n<leveldot>\n"
        << "  <dot>\n"
        << "    <name>leveldot_X</name>\n"
        << "    <position>\n"
        << "      <x-pos>" << (int)click_pos.x << "</x-pos>\n"
        << "      <y-pos>" << (int)click_pos.y << "</y-pos>\n"
        << "      <z-pos>0</z-pos>\n"
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
                  StatManager::instance()->set_string("current-tutorial-node", dot->get_name());
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
      //FIXME: const Vector& click_pos = display_gc->screen_to_world(Vector(x, y));
      Vector click_pos(x, y);

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
  StatManager::instance()->get_bool("credits-unlocked", credits_unlocked);

  if (!credits_unlocked)
    {
      Dot* dot = path_graph->get_dot(path_graph->lookup_node("leveldot_19"));
      if (dot)
        {
          if (dot->finished())
            {
              ScreenManager::instance()->replace_screen(new StoryScreen(Story::credits), true);
              StatManager::instance()->set_bool("credits-unlocked", true);
            }
        }
      else
        {
          std::cout << "Error: WorldMap: Last level missing" << std::endl;
        }
    }
}

} // namespace WorldMapNS
} // namespace Pingus

/* EOF */
