#include <iostream>

#include "engine/display/display.hpp"
#include "engine/display/font.hpp"
#include "engine/display/framebuffer.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/resource.hpp"
#include "pingus/world.hpp"
#include "util/log.hpp"
#include "util/pathname.hpp"

int main(int argc, char** argv)
{
  g_path_manager.set_path("data");
  Resource::init();

  Display::create_window(OPENGL_FRAMEBUFFER, Size(800, 600), false, false);

  Fonts::init();
  Sound::PingusSound::init();

  std::vector<Sprite> sprites;
  for(int i = 1; i < argc; ++i)
  {
    sprites.push_back(Sprite(argv[i]));
  }

  while(true)
  {
    Display::get_framebuffer()->fill_rect(Rect(0, 0, 800, 600), Color(155, 0, 155));
    for(auto it = sprites.begin(); it != sprites.end(); ++it)
    {
      (*it).render(400 - (*it).get_width()/2, 300 - (*it).get_height()/2, *Display::get_framebuffer());
      (*it).update();
    }
    Display::flip_display();
    SDL_Delay(30);
  }

  return 0;
}

/* EOF */
