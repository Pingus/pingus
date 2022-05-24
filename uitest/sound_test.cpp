#include <uitest/uitest.hpp>
#include <logmich/log.hpp>

#include "engine/sound/sound.hpp"
#include "pingus/globals.hpp"

UITEST(PingusSound, play_sound, "[NAME]...")
{
  logmich::set_log_level(logmich::LogLevel::DEBUG);

  globals::sound_enabled = true;
  pingus::sound::PingusSound::init();
  for(auto const& arg : rest)
  {
    std::cout << "Playing " << arg << std::endl;
    pingus::sound::PingusSound::play_sound(arg);
  }
  pingus::sound::PingusSound::deinit();
}

/* EOF */
