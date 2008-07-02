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

#include <ClanLib/Core/System/system.h>
#include <ClanLib/Display/surface.h>
#include "resource.hpp"
#include "timer.hpp"
#include "blitter.hpp"
#include "blitter_test.hpp"


class BlitterTestImpl 
{
public:
  BlitterTestImpl() {}
  void run() 
  {
    //CL_Surface surf = Resource::load_surface("textures/stone", "textures");
    CL_Surface surf = Resource::load_surface("textures/greentex");

    Surface buf(256, 256, 256*4, CL_PixelFormat::rgba8888);
    Surface data = surf.get_pixeldata();
    Timer bench("Blit");
    Blitter::put_surface(buf, data, 0, 0);
    Blitter::fill_rect(buf, CL_Rect(-50, -50, 250, 250), Color(255, 150, 50, 128));
    Blitter::fill_rect(buf, CL_Rect(50, 50, 150, 150), Color(255, 150, 50, 155));
    bench.stop();

    CL_Surface surf2(buf);

    while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
      {
        CL_Display::clear(Color(155, 0, 0));
        surf.draw(0, 0);
        surf2.draw(400, 0);

        CL_Display::flip();
        CL_System::keep_alive();
        CL_System::sleep(100);
      }
  }
};

BlitterTest::BlitterTest()
{
  impl = new BlitterTestImpl();
}

void
BlitterTest::run()
{
  impl->run();
}


/* EOF */
