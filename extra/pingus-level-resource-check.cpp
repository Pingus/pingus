#include <iostream>

#include "engine/sound/sound.hpp"
#include "engine/display/display.hpp"
#include "engine/display/font.hpp"
#include "pingus/world.hpp"
#include "pingus/resource.hpp"
#include "pingus/fonts.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/globals.hpp"
#include "util/log.hpp"

/** Load a level and create the World for each given level, this
    allows to track missing resources and other problems in level
    creation */
int main(int argc, char** argv)
{
  g_path_manager.set_path("data");
  Resource::init();
 
  globals::framebuffer_type = NULL_FRAMEBUFFER;
  Display::set_video_mode(Size(640, 480),false);

  Fonts::init();
  Sound::PingusSound::init();

  for(int i = 1; i < argc; ++i)
  {
    try 
    {
      std::cout << "Processing: " << argv[i] << " " << i << "/" << argc << std::endl;
      PingusLevel plf(Pathname(argv[i], Pathname::SYSTEM_PATH));
      World world(plf);
    }
    catch(const std::exception& err)
    {
      log_error(argv[i] << ": exception catched: " << err.what());
    }
  }

  Sound::PingusSound::deinit();
  Fonts::deinit();
  Resource::deinit();

  return 0;
}

/* EOF */
