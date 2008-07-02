//  $Id: preview_renderer.cpp 2986 2007-08-17 16:20:09Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "world.hpp"
//#include "gui/buffer_graphic_context.hpp"
#include "preview_renderer.hpp"


void
PreviewRenderer::render(const PingusLevel& plf,
                        const std::string& output_file)
{
#if 0 // FIXME:
  std::cout << "PreviewRenderer::render: " << output_file << std::endl;
  World* world = new World(*plf);
  BufferDrawingContext buffer(world->get_width(), world->get_height());
  world->draw(buffer);
  buffer.write(output_file);
  delete world;
#endif
}


/* EOF */
