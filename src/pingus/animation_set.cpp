// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/animation_set.hpp"

#include <map>
#include <stdexcept>

#include "util/pathname.hpp"

namespace pingus {

AnimationClock
AnimationDef::make_clock() const
{
  AnimationClock clock = AnimationClock::from_sprite(left);
  if (loop) {
    clock.set_loop(*loop);
  }
  return clock;
}

DirectionalAnimationClock
AnimationDef::make_directional_clock() const
{
  AnimationClock left_clock = AnimationClock::from_sprite(left);
  AnimationClock right_clock = AnimationClock::from_sprite(right);
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

  auto set = std::make_shared<AnimationSet const>(from_reader(doc.get_root()));
  cache.emplace(name, set);
  return set;
}

AnimationSet
AnimationSet::from_reader(ReaderObject const& reader)
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
    if (mapping.read("sprite", sprite))
    {
      anim.left = sprite;
      anim.right = sprite;
    }
    else if (!mapping.read("left", anim.left) || !mapping.read("right", anim.right))
    {
      throw std::runtime_error("animation '" + anim.name + "' needs 'sprite' or 'left' and 'right'");
    }

    geom::ipoint offset;
    if (mapping.read("offset", offset)) {
      anim.offset = Vector2f(static_cast<float>(offset.x()), static_cast<float>(offset.y()));
    }

    bool loop;
    if (mapping.read("loop", loop)) {
      anim.loop = loop;
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
