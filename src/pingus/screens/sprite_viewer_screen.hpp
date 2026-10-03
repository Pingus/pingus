// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_SCREENS_SPRITE_VIEWER_SCREEN_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_SPRITE_VIEWER_SCREEN_HPP

#include <map>
#include <string>
#include <vector>

#include "engine/display/sprite.hpp"
#include "engine/screen/screen.hpp"
#include "math/random.hpp"
#include "math/vector2f.hpp"
#include "pingus/animation_clock.hpp"
#include "pingus/animation_set.hpp"
#include "pingus/direction.hpp"
#include "util/pathname.hpp"

namespace pingus {

/** Preview .sprite / .animset assets:

      pingus --sprite-view
      pingus --sprite-view path/to/file.sprite
      pingus path/to/file.animset

    Shows the same frames the game would draw. Catalog keys ([ ] or
    PageUp/PageDown) cycle every .sprite under images/ and every
    .animset under animsets/. Within an animset: up/down select
    animation, left/right direction, space pause, '.' step when paused,
    R reload, O offset crosshair, Escape exit. */
class SpriteViewerScreen : public Screen
{
public:
  /** Browse the full datadir catalog, starting at the first entry. */
  SpriteViewerScreen();

  /** Open a specific file; still builds the datadir catalog for browsing. */
  explicit SpriteViewerScreen(Pathname const& file);

  void draw(DrawingContext& gc) override;
  void update_input(pingus::input::Event const& event) override;
  void update(float delta) override;

private:
  enum class Mode { Sprite, Animset };

  struct ViewerParticle
  {
    Sprite sprite;
    Vector2f pos;
    Vector2f velocity;
    int livetime;

    ViewerParticle() :
      sprite(),
      pos(),
      velocity(),
      livetime(0)
    {}
  };

  struct OverlayPlayback
  {
    std::string animation;
    Vector2f offset;
    AnimationClock clock;
    Sprite sprite;

    OverlayPlayback() :
      animation(),
      offset(),
      clock(),
      sprite()
    {}
  };

  void build_catalog(Pathname const& start_file);
  void select_file(int index);
  void reload();
  void load_sprite_file();
  void load_animset_file();
  void select_animation(int index);
  void restart_playback();
  void step_frame();
  void fire_effects_up_to(int step);
  void fire_effect(Effect const& effect);
  void update_overlays();
  void update_particles();
  Sprite load_sprite_by_name(std::string const& res_name);
  void draw_crosshair(DrawingContext& gc, int x, int y);
  void draw_hud(DrawingContext& gc);

  std::vector<Pathname> m_catalog;
  int m_file_index;

  Pathname m_file;
  Mode m_mode;

  Sprite m_sprite;
  AnimationClock m_clock;
  bool m_paused;
  bool m_show_offset;

  AnimationSet m_animset;
  int m_anim_index;
  Direction m_direction;
  std::map<std::string, Sprite> m_sprites;
  int m_last_effect_step;

  std::vector<OverlayPlayback> m_overlays;
  std::vector<ViewerParticle> m_particles;
  Random m_rng;

  float m_tick_accum;

  SpriteViewerScreen(SpriteViewerScreen const&) = delete;
  SpriteViewerScreen& operator=(SpriteViewerScreen const&) = delete;
};

} // namespace pingus

#endif

/* EOF */
