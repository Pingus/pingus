// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_SCREENS_PINGUS_MENU_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_PINGUS_MENU_HPP

#include <memory>

#include "engine/display/sprite.hpp"
#include "engine/screen/gui_screen.hpp"
#include "pingus/fonts.hpp"

namespace pingus {

class PingusMenu : public GUIScreen
{
public:
  bool is_init;
  std::string hint;
  std::string help;
  float text_scroll_offset;

private:
  std::unique_ptr<LayerManager> background;
  Sprite logo;

  MenuButton* start_button;
  MenuButton* quit_button;
  MenuButton* editor_button;
  MenuButton* contrib_button;
  MenuButton* options_button;

  void show_credits();

  void do_quit();
  void do_start(const std::string &filename);
  void do_edit();

  void create_background(Size const& size);

public:
  PingusMenu();
  ~PingusMenu() override;

  void on_click(MenuButton* button);
  void set_hint(std::string const& str);

  /// Load all images and other stuff for the menu
  void on_escape_press() override;
  void draw_background(DrawingContext& gc) override;

  void update(float delta) override;

  void resize(Size const& size) override;

private:
  PingusMenu (PingusMenu const&);
  PingusMenu& operator= (PingusMenu const&);
};

} // namespace pingus

#endif

/* EOF */
