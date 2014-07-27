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
 
  Display::create_window(NULL_FRAMEBUFFER, Size(640, 480), false, false);

  Fonts::init();
  Sound::PingusSound::init();

  for(int i = 1; i < argc; ++i)
  {
    try 
    {
      std::cout << "Processing: " << argv[i] << " " << i << "/" << (argc-1) << std::endl;
      Pathname filename(argv[i], Pathname::SYSTEM_PATH);

      std::cout << "Loading: " << filename.str() << std::endl;
      std::unique_ptr<Editor::EditorLevel> level = Editor::EditorLevel::from_level_file(filename);
      std::cout << "Saving:  " << argv[i] << std::endl;
      level->save_level(argv[i]);
    }
    catch(const std::exception& err)
    {
      log_error("%1%: exception catched: %2%", argv[i], err.what());
    }
  }

  Sound::PingusSound::deinit();
  Fonts::deinit();
  Resource::deinit();

  return 0;

}

/* EOF */
