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

#ifndef HEADER_PINGUS_FWD_HPP
#define HEADER_PINGUS_FWD_HPP

namespace pingus {
class ActionHolder;
class ArmageddonButton;
class BButton;
class ButtonPanel;
class CollisionMap;
class DemoPlayer;
class DrawingContext;
class FPSCounter;
class ForwardButton;
class GameDelta;
class GroundMap;
class LayerManager;
class LevelSelector;
class LevelsetSelector;
class MenuButton;
class PauseButton;
class Pingu;
class PinguAction;
class PinguHolder;
class PingusCounter;
class PingusDemo;
class PingusLevel;
class Playfield;
class SceneContext;
class Screen;
class Server;
class SmallMap;
class StoryScreenComponent;
class WorldObj;
class WorldObjAbstractFactory;
} // namespace pingus

namespace pingus::input {
class Controller;
class Manager;
struct AxisEvent;
struct ButtonEvent;
struct Event;
struct ScrollEvent;
} // namespace pingus::input

namespace pingus::gui {
class GUIManager;
class SurfaceButton;
} // pingus::gui

namespace pingus::particles {
class PinguParticleHolder;
class RainParticleHolder;
class SmokeParticleHolder;
class SnowParticleHolder;
} // namespace pingus::particles

namespace pingus::worldmap {
class Dot;
class Drawable;
class PathGraph;
class Pingus;
class Worldmap;
class WorldmapStory;
} // namespace pingus::worldmap

#endif

/* EOF */
