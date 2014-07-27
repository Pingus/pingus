// LogMich - A Trivial Logging Library
// Copyright (C) 2014 Ingo Ruhnke <grumbel@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include <logmich/log.hpp>

int main()
{
  logmich::set_log_level(logmich::kInfo);
  log_error("error level log message");
  log_warn("warring level log message");
  log_info("info level log message");
  log_debug("debug level log message [invisible]");
  log_tmp("tmp level log message [invisible]");

  logmich::set_log_level(logmich::kTemp);
  log_error("error level log message with format: %d", 5);
  log_warn("warring level log message with format: %d", 10);
  log_info("info level log message with format: %s", "Hello World");
  log_debug("debug level log message with format: %d %d %d %d", 1, 2, 3, 4);
  log_tmp("tmp level log message %d", 42);


  logmich::set_log_level(logmich::kInfo);
  log_debug("this should not be visible [invisible]");
  logmich::set_log_level(logmich::kTemp);
  log_debug("this should be visible");
  log_tmp("this should be visible");

  return 0;
}

/* EOF */
