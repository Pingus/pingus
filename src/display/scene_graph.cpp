//  $Id: scene_graph.cpp 3166 2007-09-16 15:29:14Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <SDL.h>
#include "scene_node.hpp"
#include "scene_graph.hpp"

extern SDL_Surface* global_screen;

SceneGraph::SceneGraph()
{
  screen.resize(800/32, 600/32); // could use microtiles instead
}

SceneGraph::~SceneGraph()
{
}

void
SceneGraph::add(SceneNode* node)
{
  nodes.push_back(node);
}

void
SceneGraph::render()
{
  for(Nodes::iterator i = nodes.begin(); i != nodes.end(); ++i)
    { // could limit this to stuff that has changed
      (*i)->render(this);
    }

  return;

#if 0
  screen.clear();
  // Find out what regions of the screen have changed
  for(Nodes::iterator i = nodes.begin(); i != nodes.end(); ++i)
    { // could limit this to stuff that has changed
      (*i)->mark(this);
    }
  
  // Redraw said regions
  for(int y = 0; y < screen.get_height(); ++y)
    for(int x = 0; x < screen.get_width(); ++x)
      {
        if (screen(x, y))
          {
            int width = 1;

            // Join cells horizontally
            while(screen(x+width,y)) { width += 1; }
            
            for(Nodes::iterator i = nodes.begin(); i != nodes.end(); ++i)
              { // FIXME: could optimize this to only draw the ones that touch the region
                SDL_Rect clip_rect;

                clip_rect.x = x*32;
                clip_rect.y = y*32;
                clip_rect.w = 32*width;
                clip_rect.h = 32;

                SDL_SetClipRect(global_screen, &clip_rect);

                (*i)->render(this);
              }

            x += width;
          }
      }
#endif
}

void
SceneGraph::mark_screen_region(SceneNode* node, const Rect& rect)
{
  for(int y = rect.top/32; y < rect.bottom/32; ++y)
    for(int x = rect.left/32; x < rect.left/32; ++x)
      {
        screen(x, y) = true;
        // screen(x, y).push_back(screen_node); bad?
      }
}

/* EOF */
