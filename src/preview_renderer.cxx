//  $Id: preview_renderer.cxx,v 1.2 2003/10/18 23:17:27 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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
#include "plf.hxx"
#include "world.hxx"
#include "gui/buffer_graphic_context.hxx"
#include "preview_renderer.hxx"

namespace Pingus {

void
PreviewRenderer::render(PLFHandle plf,
                        const std::string& output_file)
{
  std::cout << "PreviewRenderer::render: " << output_file << std::endl;
  World* world = new World(*plf);
  BufferGraphicContext buffer(world->get_width(), world->get_height());
  world->draw(buffer);
  buffer.write(output_file);
  delete world;
}

} // namespace Pingus

/* EOF */
