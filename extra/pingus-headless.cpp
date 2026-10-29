// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// Run the game simulation without a display, for regression and smoke
// testing. A .pingus-demo file is replayed, a .pingus level is run without
// player input and armageddon is triggered after a fixed number of ticks.
// Prints one result line per file, including a hash of the final game
// state, so that runs on different builds and platforms can be compared.
//
// With --trace a hash of the game state is written for every tick, to
// find the first tick where two runs diverge. --repeat runs each file
// several times in the same process and reports differing results,
// which points to state leaking between runs or uninitialized memory.
//
// With --screenshot the world is rendered offscreen with the SDL renderer
// at the given ticks and saved as PNG, for comparing rendering output.

#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
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
#include "pingus/state_hash.hpp"
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

  /** Directory for per-tick state traces, empty when disabled */
  std::filesystem::path trace_dir = {};

  /** Include the state of every live pingu in the trace */
  bool trace_pingus = false;

  /** Number of times each file is run */
  int repeat = 1;
};

struct RunResult
{
  bool finished;
  uint64_t hash;
};

/** The terrain is only hashed every so many ticks in traces, hashing
    the whole collision map every tick is too slow */
constexpr int terrain_trace_interval = 100;

std::string hex(uint64_t value)
{
  std::ostringstream out;
  out << std::hex << std::setw(16) << std::setfill('0') << value;
  return out.str();
}

uint64_t final_hash(Server& server)
{
  StateHash hash;
  hash.add_u64(hash_game_state(server));
  hash.add_u64(hash_terrain(*server.get_world()));
  return hash.get();
}

void write_trace(std::ostream& trace, Server& server, RunOptions const& opts)
{
  int const time = server.get_time();
  trace << time << " " << hex(hash_game_state(server)) << "\n";
  if (time % terrain_trace_interval == 0) {
    trace << time << " terrain " << hex(hash_terrain(*server.get_world())) << "\n";
  }
  if (opts.trace_pingus) {
    write_pingu_states(trace, *server.get_world());
  }
}

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
bool run(Server& server, Pathname const& path, std::vector<ServerEvent> events, RunOptions const& opts, bool armageddon,
         std::ostream* trace)
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

    if (trace) {
      write_trace(*trace, server, opts);
    }
  }
  return true;
}

void print_result(Pathname const& path, Server& server, RunResult const& result)
{
  PinguHolder* pingus = server.get_world()->get_pingus();
  std::cout << path.get_raw_path() << ":"
            << " time=" << server.get_time()
            << " released=" << pingus->get_number_of_released()
            << " exited=" << pingus->get_number_of_exited()
            << " killed=" << pingus->get_number_of_killed()
            << " hash=" << hex(result.hash)
            << (result.finished ? "" : " TIMEOUT")
            << std::endl;
}

/** Run a .pingus-demo or .pingus file once, printing the result line
    unless 'quiet' is set */
RunResult process_file(Pathname const& path, RunOptions const& opts, bool quiet, std::ostream* trace)
{
  bool const is_demo = System::get_file_extension(path.get_raw_path()) == "pingus-demo";

  std::vector<ServerEvent> events;
  std::unique_ptr<PingusLevel> plf;
  if (is_demo) {
    PingusDemo demo(path);
    events = demo.get_events();
    plf = std::make_unique<PingusLevel>(Pathname("levels/" + demo.get_levelname() + ".pingus", Pathname::DATA_PATH));
  } else {
    plf = std::make_unique<PingusLevel>(path);
  }

  Server server(*plf, false);
  if (trace) {
    write_trace(*trace, server, opts);
  }
  bool const finished = run(server, path, std::move(events), opts, !is_demo, trace);
  RunResult const result{finished, final_hash(server)};
  if (!quiet) {
    print_result(path, server, result);
  }
  return result;
}

/** Run a file opts.repeat times, returns false if the runs differ */
bool process(Pathname const& path, RunOptions const& opts)
{
  std::unique_ptr<std::ofstream> trace;
  if (!opts.trace_dir.empty()) {
    std::filesystem::path const filename = opts.trace_dir /
      (std::filesystem::path(path.get_raw_path()).filename().string() + ".trace");
    trace = std::make_unique<std::ofstream>(filename);
    if (!*trace) {
      throw std::runtime_error("failed to write " + filename.string());
    }
  }

  RunResult const first = process_file(path, opts, false, trace.get());
  for (int i = 1; i < opts.repeat; ++i)
  {
    RunResult const other = process_file(path, opts, true, nullptr);
    if (other.hash != first.hash || other.finished != first.finished) {
      std::cout << path.get_raw_path() << ": MISMATCH in run " << (i + 1)
                << " hash=" << hex(other.hash) << std::endl;
      return false;
    }
  }
  return true;
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
    .add_option('T', "screenshot-ticks", "LIST", "Comma separated ticks for --screenshot (default: 1,500,2000)")
    .add_option('H', "trace", "DIR", "Write a hash of the game state for every tick to DIR/FILE.trace")
    .add_option('P', "trace-pingus", "", "Include the state of every live pingu in the trace")
    .add_option('r', "repeat", "N", "Run each file N times and report differing results (default: 1)");

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

      case 'H':
        opts.trace_dir = opt.argument;
        break;

      case 'P':
        opts.trace_pingus = true;
        break;

      case 'r':
        opts.repeat = std::stoi(opt.argument);
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

  if (!opts.trace_dir.empty()) {
    std::filesystem::create_directories(opts.trace_dir);
  }

  int errors = 0;
  for (auto const& path : files)
  {
    try
    {
      if (!process(path, opts)) {
        errors += 1;
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
