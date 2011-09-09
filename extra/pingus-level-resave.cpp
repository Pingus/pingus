#include <iostream>

#include "editor/editor_level.hpp"
#include "engine/display/display.hpp"
#include "engine/display/font.hpp"
#include "engine/sound/sound.hpp"
#include "math/size.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/resource.hpp"
#include "util/log.hpp"

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
      std::cout << "Processing: " << argv[i] << " " << i << "/" << (argc-1) << std::endl;
      Pathname filename(argv[i], Pathname::SYSTEM_PATH);
      Editor::EditorLevel level;
      std::cout << "Loading: " << filename.str() << std::endl;
      level.load_level(filename);
      std::cout << "Saving:  " << argv[i] << std::endl;
      level.save_level(argv[i]);
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
