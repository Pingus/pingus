// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_PINGU_ACTION_VIEW_HPP
#define HEADER_PINGUS_PINGUS_PINGU_ACTION_VIEW_HPP

#include <memory>

#include "fwd.hpp"

namespace pingus {

/** Draws a pingu doing a particular action. The view holds the action's
    sprites, the action itself only game state, including the
    AnimationClocks the view takes the current frame from. Views are
    created by the drawing system when needed, so running the simulation
    without drawing never loads action sprites. */
class PinguActionView
{
public:
  PinguActionView() {}
  virtual ~PinguActionView() {}

  PinguActionView(PinguActionView const&) = delete;
  PinguActionView& operator=(PinguActionView const&) = delete;

  virtual void draw(SceneContext& gc, Pingu& pingu) = 0;
};

/** Create the view for the given action of the pingu */
std::unique_ptr<PinguActionView> create_action_view(Pingu& pingu, PinguAction& action);

} // namespace pingus

#endif

/* EOF */
