// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/worldmap/worldmap.hpp"

#include <iostream>

#include "engine/sound/sound.hpp"
#include "pingus/gettext.h"
#include "pingus/globals.hpp"
#include "pingus/stat_manager.hpp"
#include "pingus/worldmap/drawable_factory.hpp"
#include "pingus/worldmap/level_dot.hpp"
#include "pingus/worldmap/pingus.hpp"
#include "util/writer.hpp"
#include "util/pathname.hpp"

namespace pingus::worldmap {

Worldmap* Worldmap::current_ = nullptr;

Worldmap::Worldmap(Pathname const& filename) :
  worldmap(),
  default_node(),
  final_node(),
  pingus(),
  gc_state(),
  path_graph(),
  drawables()
{
  current_ = this;

  worldmap = PingusWorldmap(filename);

  ReaderCollection const& objects = worldmap.get_objects();
  for (auto const& reader_object : objects.get_objects()) {
    std::unique_ptr<Drawable> drawable = DrawableFactory::create(reader_object);
    if (drawable) {
      drawables.push_back(std::move(drawable));
    } else {
      log_info("Worldmap::parse_objects: Parse Error");
    }
  }

  ReaderMapping const& path_graph_reader = worldmap.get_graph();
  path_graph.reset(new PathGraph(this, path_graph_reader));

  default_node = path_graph->lookup_node(worldmap.get_default_node());
  final_node   = path_graph->lookup_node(worldmap.get_final_node());

  auto pingus_up = std::make_unique<Pingus>(path_graph.get());
  pingus = pingus_up.get();
  set_starting_node();
  add_drawable(std::move(pingus_up));

  gc_state.set_limit(geom::irect(geom::ipoint(0, 0), geom::isize(worldmap.get_width(), worldmap.get_height())));
}

Worldmap::~Worldmap()
{
}

void
Worldmap::draw(DrawingContext& gc)
{
  Vector2i pingu_pos(static_cast<int>(pingus->get_pos().x()),
                     static_cast<int>(pingus->get_pos().y()));
  int min, max;
  int width  = worldmap.get_width();
  int height = worldmap.get_height();

  if (width >= gc.get_width())
  {
    min = gc.get_width()/2;
    max = width - gc.get_width()/2;
  }
  else
  {
    min = width - gc.get_width()/2;
    max = gc.get_width()/2;
  }
  pingu_pos = Vector2i(std::clamp(pingu_pos.x(), min, max),
                       pingu_pos.y());

  if (height >= gc.get_height())
  {
    min = gc.get_height()/2;
    max = height - gc.get_height()/2;
  }
  else
  {
    min = height - gc.get_height()/2;
    max = gc.get_height()/2;
  }

  pingu_pos = Vector2i(pingu_pos.x(),
                       std::clamp(pingu_pos.y(), min, max));

  gc_state.set_size(gc.get_width(), gc.get_height());
  gc_state.set_pos(Vector2i(pingu_pos.x(), pingu_pos.y()));

  gc_state.push(gc);

  for (auto& drawable : drawables) {
    drawable->draw(gc);
  }

  for(Dot* dot : path_graph->dots) {
    if (dot->get_highlight()) {
      dot->draw_hover(gc);
    }
  }

  gc_state.pop(gc);
}

void
Worldmap::update(float delta)
{
  for (auto i = drawables.begin(); i != drawables.end(); ++i)
  {
    (*i)->update (delta);
  }
}

void
Worldmap::on_startup()
{
  pingus::sound::PingusSound::play_music(worldmap.get_music());
  update_locked_nodes();
}

void
Worldmap::add_drawable(std::unique_ptr<Drawable> drawable)
{
  drawables.push_back(std::move(drawable));
}

void
Worldmap::on_pointer_move(int x, int y)
{
  Vector2f const mpos = Vector2f(gc_state.screen2world(Vector2i(x, y)));
  Dot* const hover_dot = path_graph->get_dot(mpos.x(), mpos.y());

  for(Dot* dot : path_graph->dots) {
    dot->set_highlight(dot == hover_dot);
  }
}

void
Worldmap::on_primary_button_press(int x, int y)
{
  Vector2f click_pos = Vector2f(gc_state.screen2world(Vector2i(x, y)));

  if (globals::developer_mode)
  {
    Writer writer(std::cout);
    writer.begin_mapping("leveldot");
    writer.write("levelname", "");
    writer.begin_mapping("dot");
    writer.write("name", "leveldot_X");
    writer.write("position", OutVector2fZ{click_pos, 0.0f});
    writer.end_mapping();
    writer.end_mapping();
    std::cout << std::endl;
    std::cout << std::endl;
  }

  Dot* dot = path_graph->get_dot(click_pos.x(), click_pos.y());
  if (dot)
  {
    if (globals::developer_mode)
      log_info("Worldmap: Clicked on: {}", dot->get_name());

    if (path_graph->lookup_node(dot->get_name()) == pingus->get_node())
    {
      if (globals::developer_mode)
        log_info("Worldmap: Pingu is on node, issue on_click()");
      dot->on_click();
    }
    else
    {
      if (dot->is_accessible())
      {
        if (!pingus->walk_to_node(path_graph->lookup_node(dot->get_name())))
        {
          if (globals::developer_mode)
            log_info("Worldmap: NO PATH TO NODE FOUND!");
        }
        else
        {
          StatManager::instance()->set_string(worldmap.get_short_name() + "-current-node", dot->get_name());
        }
      }
      else
      {
        pingus::sound::PingusSound::play_sound("chink");
      }
    }
  }
}

void
Worldmap::on_secondary_button_press(int x, int y)
{
  if (globals::developer_mode)
  {
    Vector2f click_pos(gc_state.screen2world(Vector2i(x, y)));

    Dot* dot = path_graph->get_dot(click_pos.x(), click_pos.y());
    if (dot)
    { // FIXME: Dot NodeID missmatch...
      NodeId id = path_graph->get_id(dot);
      pingus->set_position(id);
    }
  }
}

void
Worldmap::enter_level()
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
    if (globals::developer_mode)
      log_info("Worldmap: Pingus not on level");
  }
}

struct unlock_nodes
{
  PathGraph* path_graph;

  unlock_nodes(PathGraph* g) :
    path_graph(g)
  {
  }

  void operator()(Node<Dot*>& node) const
  {
    if (node.data->is_finished())
    {
      //log_info("Unlocking neightbours of: " << node.data);
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
Worldmap::update_locked_nodes()
{
  // FIXME: This shouldn't be a polling function
  path_graph->graph.for_each_node(unlock_nodes(path_graph.get()));

#if 0
  bool credits_unlocked = false;
  StatManager::instance()->get_bool(worldmap.get_short_name() + "-endstory-seen", credits_unlocked);

  if (!credits_unlocked)
  {
    // See if the last level is finished
    Dot* dot = path_graph->get_dot(final_node);
    if (dot)
    {
      if (dot->is_finished())
      {
        ScreenManager::instance()->push_screen(new StoryScreen(worldmap.get_end_story()));
      }
    }
    else
    {
      log_info("Error: Worldmap: Last level missing");
    }
  }
#endif
}

// Determine starting node
void
Worldmap::set_starting_node()
{
  // See if the user has played this map before.  If not, use the <default-node>
  // tag from the XML file.
  NodeId id;
  std::string node_name;

  if (StatManager::instance()->get_string(worldmap.get_short_name() + "-current-node", node_name))
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
  if (leveldot)
  {
    leveldot->unlock();
  }
}

bool
Worldmap::is_final_map()
{
  return pingus->get_node() == final_node;
}

int
Worldmap::get_width()  const
{
  return worldmap.get_width();
}

int
Worldmap::get_height() const
{
  return worldmap.get_height();
}

} // namespace pingus::worldmap

/* EOF */
