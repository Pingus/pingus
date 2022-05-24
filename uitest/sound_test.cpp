#include <uitest/uitest.hpp>
#include <logmich/log.hpp>

#include "engine/sound/sound.hpp"
#include "pingus/globals.hpp"

UITEST(PingusSound, play_sound, "[NAME]...")
{
  logmich::set_log_level(logmich::LogLevel::DEBUG);

  globals::sound_enabled = true;
  Sound::PingusSound::init();
  for(auto const& arg : rest)
  {
    std::cout << "Playing " << arg << std::endl;
    Sound::PingusSound::play_sound(arg);
  }
  Sound::PingusSound::deinit();
}

/* EOF */
