// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/screens/sprite_viewer_screen.hpp"

#include <algorithm>
#include <stdexcept>

#include <logmich/log.hpp>
#include <strut/to_string.hpp>

#include "engine/display/display.hpp"
#include "engine/display/drawing_context.hpp"
#include "engine/display/sprite_description.hpp"
#include "engine/input/event.hpp"
#include "engine/screen/screen_manager.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/fonts.hpp"
#include "util/pathname.hpp"

namespace pingus {

namespace {

std::string effect_label(Effect const& effect)
{
  if (std::holds_alternative<SoundEffect>(effect)) {
    return "sound:" + std::get<SoundEffect>(effect).name;
  }
  if (std::holds_alternative<ParticlesEffect>(effect)) {
    return "particles:" + std::get<ParticlesEffect>(effect).kind;
  }
  if (std::holds_alternative<OverlayEffect>(effect)) {
    return "overlay:" + std::get<OverlayEffect>(effect).animation;
  }
  return "?";
}

} // namespace

SpriteViewerScreen::SpriteViewerScreen(Pathname const& file) :
  Screen(Display::get_size()),
  m_file(file),
  m_mode(file.get_raw_path().ends_with(".animset") ? Mode::Animset : Mode::Sprite),
  m_sprite(),
  m_clock(),
  m_paused(false),
  m_show_offset(true),
  m_animset(),
  m_anim_index(0),
  m_direction(),
  m_sprites(),
  m_last_effect_step(-1),
  m_overlays(),
  m_particles(),
  m_rng(1),
  m_tick_accum(0.0f)
{
  m_direction.right();
  reload();
}

void
SpriteViewerScreen::reload()
{
  m_overlays.clear();
  m_particles.clear();
  m_sprites.clear();
  m_last_effect_step = -1;
  m_tick_accum = 0.0f;
  m_paused = false;

  try {
    if (m_mode == Mode::Animset) {
      load_animset_file();
    } else {
      load_sprite_file();
    }
  } catch (std::exception const& e) {
    log_error("sprite viewer: failed to load {}: {}", m_file.str(), e.what());
  }
}

void
SpriteViewerScreen::load_sprite_file()
{
  auto desc = SpriteDescription::from_file(m_file);
  m_sprite = Sprite(*desc);
  m_clock = AnimationClock::from_description(*desc);
  m_clock.restart();
  m_clock.apply_to(m_sprite);
}

void
SpriteViewerScreen::load_animset_file()
{
  m_animset = AnimationSet::from_file(m_file);
  if (m_animset.get_animations().empty()) {
    throw std::runtime_error(m_file.str() + ": no animations");
  }
  if (m_anim_index >= static_cast<int>(m_animset.get_animations().size())) {
    m_anim_index = 0;
  }
  select_animation(m_anim_index);
}

Sprite
SpriteViewerScreen::load_sprite_by_name(std::string const& res_name)
{
  // Prefer loading from the .sprite file on disk so reload picks up art
  // changes without going through the resource cache.
  Pathname const path("images/" + res_name + ".sprite", Pathname::DATA_PATH);
  if (path.exist()) {
    auto desc = SpriteDescription::from_file(path);
    return Sprite(*desc);
  }
  return Sprite(res_name);
}

void
SpriteViewerScreen::select_animation(int index)
{
  auto const& anims = m_animset.get_animations();
  if (anims.empty()) {
    return;
  }
  if (index < 0) {
    index = static_cast<int>(anims.size()) - 1;
  } else if (index >= static_cast<int>(anims.size())) {
    index = 0;
  }
  m_anim_index = index;
  restart_playback();
}

void
SpriteViewerScreen::restart_playback()
{
  m_overlays.clear();
  m_last_effect_step = -1;
  m_tick_accum = 0.0f;

  if (m_mode == Mode::Sprite) {
    m_clock.restart();
    m_clock.apply_to(m_sprite);
    return;
  }

  AnimationDef const& def = m_animset.get_animations()[static_cast<size_t>(m_anim_index)];
  std::string const& sprite_name = def.sprite_name(m_direction);
  auto it = m_sprites.find(sprite_name);
  if (it == m_sprites.end()) {
    it = m_sprites.emplace(sprite_name, load_sprite_by_name(sprite_name)).first;
  }
  m_sprite = it->second;
  m_clock = AnimationClock::from_sprite(sprite_name);
  if (def.loop) {
    m_clock.set_loop(*def.loop);
  }
  m_clock.restart();
  m_clock.apply_to(m_sprite);

  // Step 0 effects fire when the animation starts
  fire_effects_up_to(0);
}

void
SpriteViewerScreen::step_frame()
{
  if (m_mode == Mode::Animset && m_animset.get_animations().empty()) {
    return;
  }

  int const before = m_clock.frame();
  m_clock.update();
  m_clock.apply_to(m_sprite);

  if (m_mode == Mode::Animset) {
    // Detect loop restart so effects re-fire
    if (m_clock.frame() < before) {
      m_last_effect_step = -1;
    }
    fire_effects_up_to(m_clock.frame());
  }
}

void
SpriteViewerScreen::fire_effects_up_to(int step)
{
  if (m_mode != Mode::Animset) {
    return;
  }
  AnimationDef const& def = m_animset.get_animations()[static_cast<size_t>(m_anim_index)];
  for (auto const& trigger : def.effects) {
    if (m_last_effect_step < trigger.step && trigger.step <= step) {
      fire_effect(trigger.effect);
    }
  }
  m_last_effect_step = step;
}

void
SpriteViewerScreen::fire_effect(Effect const& effect)
{
  Vector2f const origin(static_cast<float>(size.width() / 2),
                        static_cast<float>(size.height() / 2));

  if (auto const* sound = std::get_if<SoundEffect>(&effect)) {
    sound::PingusSound::play_sound(sound->name, sound->volume);
  } else if (auto const* particles = std::get_if<ParticlesEffect>(&effect)) {
    for (int i = 0; i < particles->count; ++i) {
      float const sx = particles->spread.x() != 0.0f
        ? m_rng.next_float() * particles->spread.x()
        : 0.0f;
      float const sy = particles->spread.y() != 0.0f
        ? m_rng.next_float() * particles->spread.y()
        : 0.0f;
      float const x = origin.x() + particles->offset.x() + sx;
      float const y = origin.y() + particles->offset.y() + sy;

      ViewerParticle p;
      if (particles->kind == "pingu") {
        p.sprite = Sprite("particles/pingu_explo");
        p.velocity = Vector2f(m_rng.next_float() * 7.0f - 3.5f,
                              m_rng.next_float() * -9.0f);
        p.livetime = 50 + m_rng.next_int(75);
      } else {
        p.sprite = Sprite(m_rng.next_int(2) == 0 ? "particles/smoke" : "particles/smoke2");
        p.velocity = Vector2f(m_rng.next_float() - 0.5f,
                              m_rng.next_float() - 0.5f);
        p.livetime = 50 + m_rng.next_int(30);
      }
      p.pos = Vector2f(x, y);
      m_particles.push_back(std::move(p));
    }
  } else if (auto const* overlay = std::get_if<OverlayEffect>(&effect)) {
    AnimationDef const* odef = m_animset.find(overlay->animation);
    if (!odef) {
      log_warn("sprite viewer: overlay animation '{}' not in set", overlay->animation);
      return;
    }
    OverlayPlayback play;
    play.animation = overlay->animation;
    play.offset = overlay->offset;
    std::string const& sprite_name = odef->sprite_name(m_direction);
    play.sprite = load_sprite_by_name(sprite_name);
    play.clock = AnimationClock::from_sprite(sprite_name);
    if (odef->loop) {
      play.clock.set_loop(*odef->loop);
    } else {
      play.clock.set_loop(false);
    }
    play.clock.restart();
    play.clock.apply_to(play.sprite);
    m_overlays.push_back(std::move(play));
  }
}

void
SpriteViewerScreen::update_overlays()
{
  for (auto& o : m_overlays) {
    o.clock.update();
    o.clock.apply_to(o.sprite);
  }
  m_overlays.erase(
    std::remove_if(m_overlays.begin(), m_overlays.end(),
                   [](OverlayPlayback const& o) {
                     return o.clock.is_finished();
                   }),
    m_overlays.end());
}

void
SpriteViewerScreen::update_particles()
{
  for (auto& p : m_particles) {
    if (p.livetime <= 0) {
      continue;
    }
    p.pos = Vector2f(p.pos.x() + p.velocity.x(),
                     p.pos.y() + p.velocity.y());
    p.velocity = Vector2f(p.velocity.x(), p.velocity.y() + 0.2f);
    --p.livetime;
    p.sprite.update();
  }
  m_particles.erase(
    std::remove_if(m_particles.begin(), m_particles.end(),
                   [](ViewerParticle const& p) { return p.livetime <= 0; }),
    m_particles.end());
}

void
SpriteViewerScreen::update(float delta)
{
  // Advance at the same tick rate as AnimationClock / the game (33 ms)
  m_tick_accum += delta * 1000.0f;
  while (m_tick_accum >= static_cast<float>(AnimationClock::kTickMillis)) {
    m_tick_accum -= static_cast<float>(AnimationClock::kTickMillis);
    if (!m_paused) {
      step_frame();
    }
    update_overlays();
    update_particles();
  }
}

void
SpriteViewerScreen::draw_crosshair(DrawingContext& gc, int x, int y)
{
  int const arm = 12;
  Color const c(0, 255, 0);
  gc.draw_line(geom::ipoint(x - arm, y), geom::ipoint(x + arm, y), c);
  gc.draw_line(geom::ipoint(x, y - arm), geom::ipoint(x, y + arm), c);
  gc.draw_rect(geom::irect(x - 2, y - 2, x + 3, y + 3), c);
}

void
SpriteViewerScreen::draw_hud(DrawingContext& gc)
{
  Font const& font = fonts::verdana11;
  int const pad = 4;
  int const x = 8;
  int const line_h = font.get_height() + 2;

  std::vector<std::string> lines;
  auto add = [&](std::string text) {
    lines.push_back(std::move(text));
  };

  add(m_file.get_raw_path());
  add(std::string("Mode: ") + (m_mode == Mode::Animset ? "animset" : "sprite")
      + (m_paused ? "  [PAUSED]" : ""));

  if (m_mode == Mode::Sprite) {
    add("Frame: " + strut::to_string(m_clock.frame()) + " / "
        + strut::to_string(m_clock.frame_count())
        + "  loop=" + (m_clock.is_looping() ? "yes" : "no")
        + (m_clock.is_finished() ? "  finished" : ""));
    add("Size: " + strut::to_string(m_sprite.get_width()) + "x"
        + strut::to_string(m_sprite.get_height()));
  } else if (!m_animset.get_animations().empty()) {
    AnimationDef const& def = m_animset.get_animations()[static_cast<size_t>(m_anim_index)];
    add("Animation: " + def.name + "  ("
        + strut::to_string(m_anim_index + 1) + "/"
        + strut::to_string(static_cast<int>(m_animset.get_animations().size())) + ")");
    add("Sprite: " + def.sprite_name(m_direction)
        + "  dir=" + (m_direction.is_left() ? "left" : "right"));
    add("Frame: " + strut::to_string(m_clock.frame()) + " / "
        + strut::to_string(m_clock.frame_count())
        + "  loop=" + (m_clock.is_looping() ? "yes" : "no")
        + (m_clock.is_finished() ? "  finished" : ""));
    add("Offset: " + strut::to_string(static_cast<int>(def.offset.x())) + ", "
        + strut::to_string(static_cast<int>(def.offset.y()))
        + (m_show_offset ? "" : "  (hidden)"));

    if (!def.effects.empty()) {
      std::string effects_line = "Effects:";
      for (auto const& tr : def.effects) {
        effects_line += "  [" + strut::to_string(tr.step) + ":" + effect_label(tr.effect) + "]";
      }
      add(effects_line);
    }

    add("");
    add("Animations (Up/Down):");
    int const list_budget = std::max(1, (size.height() - 80) / line_h - static_cast<int>(lines.size()));
    int start = std::max(0, m_anim_index - list_budget / 2);
    int end = std::min(static_cast<int>(m_animset.get_animations().size()), start + list_budget);
    start = std::max(0, end - list_budget);
    for (int i = start; i < end; ++i) {
      std::string prefix = (i == m_anim_index) ? "> " : "  ";
      add(prefix + m_animset.get_animations()[static_cast<size_t>(i)].name);
    }
  }

  float max_w = 0.0f;
  for (auto const& s : lines) {
    max_w = std::max(max_w, font.get_width(s));
  }

  int const top_y = 8;
  int const top_h = static_cast<int>(lines.size()) * line_h + pad * 2;
  int const top_w = static_cast<int>(max_w) + pad * 2 + 4;
  gc.draw_fillrect(geom::irect(x - pad, top_y - pad,
                               x - pad + top_w, top_y - pad + top_h),
                   Color(240, 240, 220, 220));

  int y = top_y;
  for (auto const& s : lines) {
    if (!s.empty()) {
      gc.print_left(font, geom::ipoint(x, y), s);
    }
    y += line_h;
  }

  std::string const help =
    "Space pause  . step  R reload  O offset  Left/Right dir  Up/Down anim  Esc quit";
  int const help_w = static_cast<int>(font.get_width(help)) + pad * 2 + 4;
  int const help_y = size.height() - font.get_height() - 8;
  gc.draw_fillrect(geom::irect(x - pad, help_y - pad,
                               x - pad + help_w, help_y + font.get_height() + pad),
                   Color(240, 240, 220, 220));
  gc.print_left(font, geom::ipoint(x, help_y), help);
}

void
SpriteViewerScreen::draw(DrawingContext& gc)
{
  int const checker = 16;
  for (int cy = 0; cy < size.height(); cy += checker) {
    for (int cx = 0; cx < size.width(); cx += checker) {
      bool dark = ((cx / checker) + (cy / checker)) % 2 != 0;
      gc.draw_fillrect(geom::irect(cx, cy, cx + checker, cy + checker),
                       dark ? Color(40, 40, 40) : Color(60, 60, 60));
    }
  }

  int const cx = size.width() / 2;
  int const cy = size.height() / 2;
  Vector2f origin(static_cast<float>(cx), static_cast<float>(cy));

  if (m_mode == Mode::Sprite) {
    if (m_sprite) {
      gc.draw(m_sprite, geom::ipoint(cx, cy));
    }
  } else if (!m_animset.get_animations().empty()) {
    AnimationDef const& def = m_animset.get_animations()[static_cast<size_t>(m_anim_index)];
    if (m_sprite) {
      Vector2f pos = origin + geom::foffset(def.offset.x(), def.offset.y());
      gc.draw(m_sprite, geom::ipoint(static_cast<int>(pos.x()), static_cast<int>(pos.y())));
    }

    for (auto const& trigger : def.effects) {
      bool active = (trigger.step == m_clock.frame());
      int mx = cx + static_cast<int>(def.offset.x()) + trigger.step * 4;
      int my = cy + static_cast<int>(def.offset.y()) + m_sprite.get_height() / 2 + 8;
      Color col = active ? Color(255, 200, 0) : Color(180, 80, 0);
      gc.draw_fillrect(geom::irect(mx - 2, my - 2, mx + 3, my + 6), col);
    }
  }

  for (auto const& o : m_overlays) {
    AnimationDef const* odef = m_animset.find(o.animation);
    Vector2f extra = odef ? odef->offset : Vector2f();
    Vector2f pos = origin + geom::foffset(o.offset.x() + extra.x(), o.offset.y() + extra.y());
    gc.draw(o.sprite, geom::ipoint(static_cast<int>(pos.x()), static_cast<int>(pos.y())));
  }

  for (auto const& p : m_particles) {
    if (p.livetime > 0) {
      gc.draw(p.sprite, geom::ipoint(static_cast<int>(p.pos.x()), static_cast<int>(p.pos.y())));
    }
  }

  if (m_show_offset) {
    draw_crosshair(gc, cx, cy);
  }
  draw_hud(gc);
}

void
SpriteViewerScreen::update_input(pingus::input::Event const& event)
{
  if (event.type != pingus::input::KEYBOARD_EVENT_TYPE) {
    return;
  }
  if (!event.keyboard.state) {
    return;
  }

  SDL_Keycode const key = event.keyboard.keysym.sym;

  switch (key) {
    case SDLK_ESCAPE:
      ScreenManager::instance()->pop_screen();
      break;

    case SDLK_r:
      reload();
      break;

    case SDLK_o:
      m_show_offset = !m_show_offset;
      break;

    case SDLK_SPACE:
      m_paused = !m_paused;
      break;

    case SDLK_PERIOD:
    case SDLK_n:
      if (m_paused) {
        step_frame();
      }
      break;

    case SDLK_LEFT:
      m_direction.left();
      if (m_mode == Mode::Animset) {
        restart_playback();
      }
      break;

    case SDLK_RIGHT:
      m_direction.right();
      if (m_mode == Mode::Animset) {
        restart_playback();
      }
      break;

    case SDLK_UP:
      if (m_mode == Mode::Animset) {
        select_animation(m_anim_index - 1);
      }
      break;

    case SDLK_DOWN:
      if (m_mode == Mode::Animset) {
        select_animation(m_anim_index + 1);
      }
      break;

    case SDLK_HOME:
      if (m_mode == Mode::Animset) {
        select_animation(0);
      }
      break;

    case SDLK_END:
      if (m_mode == Mode::Animset && !m_animset.get_animations().empty()) {
        select_animation(static_cast<int>(m_animset.get_animations().size()) - 1);
      }
      break;

    default:
      break;
  }
}

} // namespace pingus

/* EOF */
