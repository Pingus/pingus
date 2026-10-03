// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/pingu_action_view.hpp"

#include <cassert>

#include "pingus/actions/angel.hpp"
#include "pingus/actions/basher.hpp"
#include "pingus/actions/blocker.hpp"
#include "pingus/actions/boarder.hpp"
#include "pingus/actions/bomber.hpp"
#include "pingus/actions/bridger.hpp"
#include "pingus/actions/climber.hpp"
#include "pingus/actions/digger.hpp"
#include "pingus/actions/drown.hpp"
#include "pingus/actions/exiter.hpp"
#include "pingus/actions/faller.hpp"
#include "pingus/actions/floater.hpp"
#include "pingus/actions/jumper.hpp"
#include "pingus/actions/laser_kill.hpp"
#include "pingus/actions/miner.hpp"
#include "pingus/actions/slider.hpp"
#include "pingus/actions/smashed.hpp"
#include "pingus/actions/splashed.hpp"
#include "pingus/actions/superman.hpp"
#include "pingus/actions/waiter.hpp"
#include "pingus/actions/walker.hpp"

namespace pingus {

namespace {

template<typename View, typename Action>
std::unique_ptr<PinguActionView> make_view(Pingu& pingu, PinguAction& action)
{
  return std::make_unique<View>(pingu, static_cast<Action const&>(action));
}

} // namespace

std::unique_ptr<PinguActionView>
create_action_view(Pingu& pingu, PinguAction& action)
{
  using namespace actions;

  switch (action.get_type())
  {
    case ActionName::ANGEL:     return make_view<AngelView, Angel>(pingu, action);
    case ActionName::BASHER:    return make_view<BasherView, Basher>(pingu, action);
    case ActionName::BLOCKER:   return make_view<BlockerView, Blocker>(pingu, action);
    case ActionName::BOARDER:   return make_view<BoarderView, Boarder>(pingu, action);
    case ActionName::BOMBER:    return make_view<BomberView, Bomber>(pingu, action);
    case ActionName::BRIDGER:   return make_view<BridgerView, Bridger>(pingu, action);
    case ActionName::CLIMBER:   return make_view<ClimberView, Climber>(pingu, action);
    case ActionName::DIGGER:    return make_view<DiggerView, Digger>(pingu, action);
    case ActionName::DROWN:     return make_view<DrownView, Drown>(pingu, action);
    case ActionName::EXITER:    return make_view<ExiterView, Exiter>(pingu, action);
    case ActionName::FALLER:    return make_view<FallerView, Faller>(pingu, action);
    case ActionName::FLOATER:   return make_view<FloaterView, Floater>(pingu, action);
    case ActionName::JUMPER:    return make_view<JumperView, Jumper>(pingu, action);
    case ActionName::LASERKILL: return make_view<LaserKillView, LaserKill>(pingu, action);
    case ActionName::MINER:     return make_view<MinerView, Miner>(pingu, action);
    case ActionName::SLIDER:    return make_view<SliderView, Slider>(pingu, action);
    case ActionName::SMASHED:   return make_view<SmashedView, Smashed>(pingu, action);
    case ActionName::SPLASHED:  return make_view<SplashedView, Splashed>(pingu, action);
    case ActionName::SUPERMAN:  return make_view<SupermanView, Superman>(pingu, action);
    case ActionName::WAITER:    return make_view<WaiterView, Waiter>(pingu, action);
    case ActionName::WALKER:    return make_view<WalkerView, Walker>(pingu, action);
    default:
      assert(false && "no view for action");
      return {};
  }
}

} // namespace pingus

/* EOF */
