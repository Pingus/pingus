//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2021 Ingo Ruhnke <grumbel@gmail.com>
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

#include "application.hpp"

#include <argpp/argpp.hpp>
#include <logmich/log.hpp>
#include <strut/from_string.hpp>

#include "editor/editor_level.hpp"
#include "editor/editor_screen.hpp"
#include "engine/input/driver_factory.hpp"
#include "engine/input/manager.hpp"
#include "engine/screen/screen_manager.hpp"
#include "engine/sound/sound.hpp"
#include "engine/system/sdl_system.hpp"
#include "gettext.h"
#include "pingus/config_manager.hpp"
#include "pingus/event_name.hpp"
#include "pingus/globals.hpp"
#include "pingus/path_manager.hpp"
#include "pingus/plf_res_mgr.hpp"
#include "pingus/resource.hpp"
#include "pingus/savegame_manager.hpp"
#include "pingus/screens/credits.hpp"
#include "pingus/screens/demo_session.hpp"
#include "pingus/screens/font_test_screen.hpp"
#include "pingus/screens/level_menu.hpp"
#include "pingus/screens/pingus_menu.hpp"
#include "pingus/screens/start_screen.hpp"
#include "pingus/screens/story_screen.hpp"
#include "pingus/stat_manager.hpp"
#include "pingus/worldmap/worldmap_screen.hpp"
#include "pingus/worldobj_factory.hpp"
#include "tinygettext/dictionary_manager.hpp"
#include "tinygettext/log.hpp"

namespace pingus {

namespace {

input::ControllerDescription pingus_controller_description()
{
  input::ControllerDescription desc;

  desc.add_axis("action-axis",  ACTION_AXIS);

  desc.add_keyboard("standard-keyboard",  STANDARD_KEYBOARD);

  desc.add_pointer("standard-pointer",   STANDARD_POINTER);
  desc.add_scroller("standard-scroller", STANDARD_SCROLLER);

  desc.add_button("primary-button",      PRIMARY_BUTTON);
  desc.add_button("secondary-button",    SECONDARY_BUTTON);
  desc.add_button("fast-forward-button", FAST_FORWARD_BUTTON);
  desc.add_button("armageddon-button",   ARMAGEDDON_BUTTON);
  desc.add_button("pause-button",        PAUSE_BUTTON);
  desc.add_button("single-step-button",  SINGLE_STEP_BUTTON);
  desc.add_button("escape-button",       ESCAPE_BUTTON);

  desc.add_button("action-up-button",    ACTION_UP_BUTTON);
  desc.add_button("action-down-button",  ACTION_DOWN_BUTTON);

  desc.add_button("action-1-button",     ACTION_1_BUTTON);
  desc.add_button("action-2-button",     ACTION_2_BUTTON);
  desc.add_button("action-3-button",     ACTION_3_BUTTON);
  desc.add_button("action-4-button",     ACTION_4_BUTTON);
  desc.add_button("action-5-button",     ACTION_5_BUTTON);
  desc.add_button("action-6-button",     ACTION_6_BUTTON);
  desc.add_button("action-7-button",     ACTION_7_BUTTON);
  desc.add_button("action-8-button",     ACTION_8_BUTTON);
  desc.add_button("action-9-button",     ACTION_9_BUTTON);
  desc.add_button("action-10-button",    ACTION_10_BUTTON);

  return desc;
}

} // namespace

Application::Application(CommandLineOptions const& cmd_options) :
  m_driver_factory(std::make_unique<input::SDLDriverFactory>()),
  m_input_manager(std::make_unique<input::Manager>(*m_driver_factory,
                                                   pingus_controller_description())),
  m_input_controller(),
  m_screen_manager()
{
  if (!cmd_options.controller.is_set())
  {
    m_input_controller = m_input_manager->create_controller(Pathname("controller/default.scm",
                                                                     Pathname::DATA_PATH).get_sys_path());
  }
  else
  {
    m_input_controller = m_input_manager->create_controller(Pathname(cmd_options.controller.get(),
                                                                     Pathname::SYSTEM_PATH).get_sys_path());
  }

  m_screen_manager = std::make_unique<ScreenManager>(*m_input_manager, m_input_controller);

  if (cmd_options.editor.is_set() && cmd_options.editor.get())
  { // Editor
    std::shared_ptr<pingus::editor::EditorScreen> editor = std::make_shared<pingus::editor::EditorScreen>();
    // optionally load a map in the editor if it was given
    if (cmd_options.rest.is_set()) {
      editor->load(Pathname(cmd_options.rest.get(), Pathname::SYSTEM_PATH));
    }

    m_screen_manager->push_screen(editor);
  }
  else if (cmd_options.rest.is_set())
  { // just start the map that was passed on the command line
    if (cmd_options.rest.get().ends_with(".pingus-demo"))
    { // Demo file
      m_screen_manager->push_screen
        (std::make_shared<DemoSession>(Pathname(cmd_options.rest.get(), Pathname::SYSTEM_PATH)));
    }
    else if (cmd_options.rest.get().ends_with(".font"))
    {
      Pathname filename(cmd_options.rest.get(), Pathname::SYSTEM_PATH);
      m_screen_manager->push_screen(std::make_shared<FontTestScreen>(filename));
    }
    else if (cmd_options.rest.get().ends_with(".credits"))
    {
      Pathname filename(cmd_options.rest.get(), Pathname::SYSTEM_PATH);
      m_screen_manager->push_screen(std::make_shared<Credits>(filename));
    }
    else if (cmd_options.rest.get().ends_with(".worldmap"))
    {
      Pathname filename(cmd_options.rest.get(), Pathname::SYSTEM_PATH);

      std::shared_ptr<pingus::worldmap::WorldmapScreen> worldmap_screen = std::make_shared<pingus::worldmap::WorldmapScreen>();
      worldmap_screen->load(filename);
      ScreenManager::instance()->push_screen(worldmap_screen);
    }
    else if (cmd_options.rest.get().ends_with(".story"))
    {
      auto story_desc = ReaderDocument::from_file(cmd_options.rest.get());
      m_screen_manager->push_screen(std::make_shared<StoryScreen>(story_desc.get_mapping()));
    }
    else if (cmd_options.rest.get().ends_with(".levelset"))
    {
      std::shared_ptr<LevelMenu> lvlm = std::make_shared<LevelMenu>();
      std::unique_ptr<Levelset> levelset = Levelset::from_file(Pathname(cmd_options.rest.get(), Pathname::SYSTEM_PATH));
      lvlm->set_levelset(levelset.release());
      m_screen_manager->push_screen(lvlm);
    }
    else
    { // Level file
      m_screen_manager->push_screen
        (std::make_shared<StartScreen>(PLFResMgr::load_plf_from_filename(Pathname(cmd_options.rest.get(),
                                                                                  Pathname::SYSTEM_PATH))));
    }
  }
  else // start a normal game
  {
    log_info("starting normal game");
    m_screen_manager->push_screen(std::make_shared<PingusMenu>());
    log_info("done: starting normal game");
  }
}

void
Application::run()
{
  assert(m_input_controller != nullptr);

  m_screen_manager->display();
}

} // namespace pingus

/* EOF */
