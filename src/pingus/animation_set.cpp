// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/animation_set.hpp"

#include <map>
#include <stdexcept>

#include "util/pathname.hpp"

namespace pingus {

namespace {

Vector2f read_vector(ReaderMapping const& mapping, std::string_view key)
{
  geom::ipoint value;
  if (mapping.read(key, value)) {
    return Vector2f(static_cast<float>(value.x()), static_cast<float>(value.y()));
  }
  return Vector2f();
}

EffectTrigger read_effect(std::string const& animation, ReaderObject const& reader)
{
  ReaderMapping const mapping = reader.get_mapping();
  std::string const type = reader.get_name();

  EffectTrigger trigger{0, SoundEffect()};
  if (!mapping.read("at-step", trigger.step)) {
    throw std::runtime_error("animation '" + animation + "': effect '" + type + "' needs 'at-step'");
  }

  if (type == "sound")
  {
    SoundEffect sound;
    if (!mapping.read("name", sound.name)) {
      throw std::runtime_error("animation '" + animation + "': sound effect needs 'name'");
    }
    mapping.read("volume", sound.volume);
    trigger.effect = sound;
  }
  else if (type == "particles")
  {
    ParticlesEffect particles;
    mapping.read("kind", particles.kind);
    if (particles.kind != "pingu" && particles.kind != "smoke") {
      throw std::runtime_error("animation '" + animation + "': unknown particle kind '" + particles.kind + "'");
    }
    mapping.read("count", particles.count);
    particles.offset = read_vector(mapping, "offset");
    particles.spread = read_vector(mapping, "spread");
    trigger.effect = particles;
  }
  else if (type == "overlay")
  {
    OverlayEffect overlay;
    if (!mapping.read("animation", overlay.animation)) {
      throw std::runtime_error("animation '" + animation + "': overlay effect needs 'animation'");
    }
    overlay.offset = read_vector(mapping, "offset");
    trigger.effect = overlay;
  }
  else
  {
    throw std::runtime_error("animation '" + animation + "': unknown effect '" + type + "'");
  }

  return trigger;
}

} // namespace

Sprite
AnimationDef::make_sprite(Direction const& dir) const
{
  if (SpriteDescription const* desc = frames(dir)) {
    return Sprite(*desc);
  } else {
    return Sprite(sprite_name(dir));
  }
}

AnimationClock
AnimationDef::make_clock() const
{
  AnimationClock clock = left_frames ? AnimationClock::from_description(*left_frames) : AnimationClock::from_sprite(left);
  if (loop) {
    clock.set_loop(*loop);
  }
  return clock;
}

DirectionalAnimationClock
AnimationDef::make_directional_clock() const
{
  AnimationClock left_clock = left_frames ? AnimationClock::from_description(*left_frames) : AnimationClock::from_sprite(left);
  AnimationClock right_clock = right_frames ? AnimationClock::from_description(*right_frames) : AnimationClock::from_sprite(right);
  if (loop) {
    left_clock.set_loop(*loop);
    right_clock.set_loop(*loop);
  }
  return DirectionalAnimationClock(left_clock, right_clock);
}

std::shared_ptr<AnimationSet const>
AnimationSet::get(std::string const& name)
{
  // Animation sets are immutable data, like the sprite resources
  static std::map<std::string, std::shared_ptr<AnimationSet const>> cache;

  auto it = cache.find(name);
  if (it != cache.end()) {
    return it->second;
  }

  Pathname const path("animsets/" + name + ".animset", Pathname::DATA_PATH);
  ReaderDocument doc = load_document(path);
  if (doc.get_root().get_name() != "pingus-animset") {
    throw std::runtime_error(path.str() + ": not a pingus-animset file");
  }

  auto set = std::make_shared<AnimationSet const>(from_reader(doc.get_root(), path));
  cache.emplace(name, set);
  return set;
}

AnimationSet
AnimationSet::from_file(Pathname const& path)
{
  ReaderDocument doc = load_document(path);
  if (doc.get_root().get_name() != "pingus-animset") {
    throw std::runtime_error(path.str() + ": not a pingus-animset file");
  }
  return from_reader(doc.get_root(), path);
}

AnimationSet
AnimationSet::from_reader(ReaderObject const& reader, Pathname const& context)
{
  AnimationSet set;

  ReaderCollection collection;
  reader.get_mapping().read("animations", collection);
  for (auto const& item : collection.get_objects())
  {
    ReaderMapping const mapping = item.get_mapping();

    AnimationDef anim;
    anim.name = item.get_name();

    std::string sprite;
    ReaderMapping frames;
    ReaderMapping left_frames;
    ReaderMapping right_frames;
    bool const has_frames = mapping.read("frames", frames);
    bool const has_left_frames = mapping.read("left-frames", left_frames);
    bool const has_right_frames = mapping.read("right-frames", right_frames);

    if (has_frames || has_left_frames || has_right_frames)
    {
      // Inline definition: 'frames' for both directions, 'left-frames' and
      // 'right-frames' add to or override it
      if (!has_frames && !(has_left_frames && has_right_frames)) {
        throw std::runtime_error("animation '" + anim.name + "' needs 'frames' or 'left-frames' and 'right-frames'");
      }

      auto make_frames = [&](bool has_dir, ReaderMapping const& dir_frames) {
        auto desc = std::make_shared<SpriteDescription>();
        if (has_frames) {
          desc->read(frames, context);
        }
        if (has_dir) {
          desc->read(dir_frames, context);
        }
        if (desc->filename.empty()) {
          throw std::runtime_error("animation '" + anim.name + "': inline frames need an 'image'");
        }
        return desc;
      };

      anim.left_frames = make_frames(has_left_frames, left_frames);
      anim.right_frames = make_frames(has_right_frames, right_frames);
      anim.left = context.get_raw_path() + ":" + anim.name + "/left";
      anim.right = context.get_raw_path() + ":" + anim.name + "/right";
    }
    else if (mapping.read("sprite", sprite))
    {
      anim.left = sprite;
      anim.right = sprite;
    }
    else if (!mapping.read("left", anim.left) || !mapping.read("right", anim.right))
    {
      throw std::runtime_error("animation '" + anim.name + "' needs 'sprite', 'left' and 'right', or 'frames'");
    }

    anim.offset = read_vector(mapping, "offset");

    bool loop;
    if (mapping.read("loop", loop)) {
      anim.loop = loop;
    }

    ReaderCollection effects;
    if (mapping.read("effects", effects)) {
      for (auto const& effect : effects.get_objects()) {
        anim.effects.push_back(read_effect(anim.name, effect));
      }
    }

    set.m_animations.push_back(anim);
  }

  return set;
}

AnimationSet::AnimationSet() :
  m_animations()
{
}

AnimationDef const*
AnimationSet::find(std::string_view name) const
{
  for (auto const& anim : m_animations) {
    if (anim.name == name) {
      return &anim;
    }
  }
  return nullptr;
}

AnimationDef const&
AnimationSet::get_animation(std::string_view name) const
{
  AnimationDef const* anim = find(name);
  if (!anim) {
    throw std::runtime_error("animation set has no animation '" + std::string(name) + "'");
  }
  return *anim;
}

} // namespace pingus

/* EOF */
