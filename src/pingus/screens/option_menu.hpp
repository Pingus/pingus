// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_SCREENS_OPTION_MENU_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_OPTION_MENU_HPP

#include <map>
#include <vector>
#include <sigc++/connection.h>

#include "engine/display/sprite.hpp"
#include "engine/gui/rect_component.hpp"
#include "engine/gui/surface_button.hpp"
#include "engine/screen/gui_screen.hpp"
#include "pingus/components/label.hpp"
#include "tinygettext/language.hpp"

namespace pingus {

class CheckBox;
class SliderBox;

class OptionMenu : public GUIScreen
{
private:
  Sprite m_background;
  Sprite m_blackboard;
  pingus::gui::SurfaceButton* ok_button;
  int x_pos;
  int y_pos;

  struct Option {
    Label*         label;
    pingus::gui::RectComponent* control;

    Option(Label* label_, pingus::gui::RectComponent* control_) :
      label(label_), control(control_)
    {}
  };

  typedef std::vector<Option> Options;
  Options options;

  CheckBox* fullscreen_box;
  CheckBox* software_cursor_box;
  CheckBox* autoscroll_box;
  CheckBox* dragdrop_scroll_box;
  CheckBox* mousegrab_box;
  CheckBox* printfps_box;

  SliderBox* master_volume_box;
  SliderBox* sound_volume_box;
  SliderBox* music_volume_box;

  //Label* defaults_label;
  //CheckBox* defaults_box;

  typedef std::vector<sigc::connection> Connections;
  Connections connections;

  tinygettext::Language m_language;
  std::map<std::string, tinygettext::Language> m_language_map;

public:
  OptionMenu();
  ~OptionMenu() override;

  void draw_background (DrawingContext& gc) override;
  void on_escape_press() override;

  void add_item(std::string const& label, std::unique_ptr<pingus::gui::RectComponent> control);

  void resize(Size const&) override;
  void close_screen();

  void on_software_cursor_change(bool v);
  void on_fullscreen_change(bool v);
  void on_autoscroll_change(bool v);
  void on_drag_drop_scrolling_change(bool v);
  void on_fastmode_change(bool v);
  void on_mousegrab_change(bool v);
  void on_printfps_change(bool v);

  void on_master_volume_change(int v);
  void on_sound_volume_change(int v);
  void on_music_volume_change(int v);

  void on_language_change(std::string const& str);
  void on_resolution_change(std::string const& str);
  void on_renderer_change(std::string const& str);

  void save_language();

private:
  OptionMenu (OptionMenu const&);
  OptionMenu& operator= (OptionMenu const&);
};

} // namespace pingus

#endif

/* EOF */
