// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Run the game simulation without a display, for regression and smoke
// testing. A .pingus-demo file is replayed, a .pingus level is run without
// player input and armageddon is triggered after a fixed number of ticks.
// Prints one result line per file.
//
// With --screenshot the world is rendered offscreen with the SDL renderer
// at the given ticks and saved as PNG, for comparing rendering output.

#include <algorithm>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include <argpp/argpp.hpp>
#include <logmich/log.hpp>

#include "engine/display/display.hpp"
#include "engine/display/framebuffer.hpp"
#include "engine/display/scene_context.hpp"
#include "engine/display/surface.hpp"
#include "engine/system/sdl_system.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/fonts.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/pingus_demo.hpp"
#include "pingus/pingus_level.hpp"
#include "pingus/resource.hpp"
#include "pingus/server.hpp"
#include "pingus/server_event.hpp"
#include "pingus/world.hpp"
#include "util/pathname.hpp"
#include "util/system.hpp"

using namespace pingus;

namespace {

struct RunOptions
{
  int armageddon_tick = 6000;
  int max_ticks = 100000;

  /** Directory for screenshots, empty when disabled */
  std::filesystem::path screenshot_dir = {};
  std::set<int> screenshot_ticks = {};
};

std::set<int> parse_ticks(std::string const& text)
{
  std::set<int> ticks;
  std::istringstream in(text);
  std::string item;
  while (std::getline(in, item, ',')) {
    ticks.insert(std::stoi(item));
  }
  return ticks;
}

/** Render the whole world offscreen and save it as PNG */
void save_screenshot(Server& server, Pathname const& path, RunOptions const& opts)
{
  World& world = *server.get_world();
  Framebuffer& fb = *Display::get_framebuffer();

  // The window is created once at the maximum size, larger worlds get cropped
  geom::isize const size(std::min(world.get_width(), fb.get_size().width()),
                         std::min(world.get_height(), fb.get_size().height()));
  geom::irect const rect(geom::ipoint(0, 0), size);
  fb.fill_rect(geom::irect(geom::ipoint(0, 0), fb.get_size()), Color(0, 0, 0, 255));

  SceneContext sc(rect);
  world.draw(sc);
  sc.render(fb, rect);

  std::filesystem::path const filename = opts.screenshot_dir /
    (std::filesystem::path(path.get_raw_path()).stem().string() + "-" +
     std::to_string(server.get_time()) + ".png");
  Surface const screen = fb.make_screenshot().subsection(rect);
  if (IMG_SavePNG(screen.get_surface(), filename.string().c_str()) != 0) {
    throw std::runtime_error("failed to write " + filename.string() + ": " + SDL_GetError());
  }
}

/** Run the server until the level is finished, sending the given events
    at their time stamps. Returns false if max_ticks was reached. */
bool run(Server& server, Pathname const& path, std::vector<ServerEvent> events, RunOptions const& opts, bool armageddon)
{
  // Reverse so that the next event can be taken with pop_back()
  std::reverse(events.begin(), events.end());

  while (!server.is_finished())
  {
    if (server.get_time() >= opts.max_ticks) {
      return false;
    }

    server.update();

    while (!events.empty() && events.back().time_stamp <= server.get_time())
    {
      events.back().send(&server);
      events.pop_back();
    }

    if (armageddon && server.get_time() == opts.armageddon_tick) {
      server.send_armageddon_event();
    }

    if (!opts.screenshot_dir.empty() && opts.screenshot_ticks.contains(server.get_time())) {
      save_screenshot(server, path, opts);
    }
  }
  return true;
}

void print_result(Pathname const& path, Server& server, bool finished)
{
  PinguHolder* pingus = server.get_world()->get_pingus();
  std::cout << path.get_raw_path() << ":"
            << " time=" << server.get_time()
            << " released=" << pingus->get_number_of_released()
            << " exited=" << pingus->get_number_of_exited()
            << " killed=" << pingus->get_number_of_killed()
            << (finished ? "" : " TIMEOUT")
            << std::endl;
}

void process_demo(Pathname const& path, RunOptions const& opts)
{
  PingusDemo demo(path);
  PingusLevel plf(Pathname("levels/" + demo.get_levelname() + ".pingus", Pathname::DATA_PATH));
  Server server(plf, false);
  bool const finished = run(server, path, demo.get_events(), opts, false);
  print_result(path, server, finished);
}

void process_level(Pathname const& path, RunOptions const& opts)
{
  PingusLevel plf(path);
  Server server(plf, false);
  bool const finished = run(server, path, {}, opts, true);
  print_result(path, server, finished);
}

} // namespace

int main(int argc, char** argv)
{
  std::vector<Pathname> files;
  RunOptions opts;
  std::string datadir = "data/";

  argpp::Parser argp;
  argp.add_usage(argv[0], "[OPTIONS]... FILE...")
    .add_text("Run .pingus-demo or .pingus files without a display")
    .add_option('h', "help", "", "Displays this help")
    .add_option('d', "datadir", "DIR", "Load game datafiles from DIR (default: data/)")
    .add_option('a', "armageddon", "TICKS", "Trigger armageddon in levels after TICKS (default: 6000)")
    .add_option('t', "max-ticks", "TICKS", "Give up after TICKS (default: 100000)")
    .add_option('s', "screenshot", "DIR", "Save renderings of the world to DIR")
    .add_option('T', "screenshot-ticks", "LIST", "Comma separated ticks for --screenshot (default: 1,500,2000)");

  for (auto const& opt : argp.parse_args(argc, argv))
  {
    switch (opt.key)
    {
      case 'h':
        argp.print_help();
        return EXIT_SUCCESS;

      case 'd':
        datadir = opt.argument;
        break;

      case 'a':
        opts.armageddon_tick = std::stoi(opt.argument);
        break;

      case 't':
        opts.max_ticks = std::stoi(opt.argument);
        break;

      case 's':
        opts.screenshot_dir = opt.argument;
        break;

      case 'T':
        opts.screenshot_ticks = parse_ticks(opt.argument);
        break;

      case argpp::ArgumentType::REST:
        files.emplace_back(opt.argument, Pathname::SYSTEM_PATH);
        break;
    }
  }

  if (files.empty())
  {
    argp.print_help();
    return EXIT_FAILURE;
  }

  g_path_manager.set_path(datadir);
  Resource::init();

  std::unique_ptr<SDLSystem> sdl_system;
  if (opts.screenshot_dir.empty())
  {
    Display::create_window(FramebufferType::NULL_FRAMEBUFFER, geom::isize(640, 480), false, false);
  }
  else
  {
    if (opts.screenshot_ticks.empty()) {
      opts.screenshot_ticks = {1, 500, 2000};
    }
    std::filesystem::create_directories(opts.screenshot_dir);
    // Render without a visible window and with the software renderer for
    // reproducible output, unless the caller picked something else
    SDL_setenv("SDL_VIDEODRIVER", "offscreen", 0);
    SDL_setenv("SDL_RENDER_DRIVER", "software", 0);
    sdl_system = std::make_unique<SDLSystem>();
    Display::create_window(FramebufferType::SDL, geom::isize(4096, 2048), false, false);
  }
  fonts::init();
  sound::PingusSound::init();

  int errors = 0;
  for (auto const& path : files)
  {
    try
    {
      if (System::get_file_extension(path.get_raw_path()) == "pingus-demo") {
        process_demo(path, opts);
      } else {
        process_level(path, opts);
      }
    }
    catch (std::exception const& err)
    {
      std::cout << path.get_raw_path() << ": ERROR " << err.what() << std::endl;
      errors += 1;
    }
  }

  sound::PingusSound::deinit();
  fonts::deinit();
  Resource::deinit();

  return errors ? EXIT_FAILURE : EXIT_SUCCESS;
}

/* EOF */
