#  $Id: Makefile.am,v 1.3 2000/02/12 20:53:42 grumbel Exp $
#
#  Pingus - A free Lemmings clone
#  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

bindir=$(prefix)/games

SUBDIRS = actions traps particles entrances editor

EXTRA_DIST = README \
             win32/Getopt.c  \
             win32/Getopt.h  \
             win32/Getopt1.c \
             win32/config.h

bin_PROGRAMS = pingus

LDADD = traps/libpingu_traps.a          \
        particles/libpingus_particle.a  \
        actions/libpingu_actions.a      \
        entrances/libpingus_entrances.a \
        editor/libpingus_editor.a


pingus_SOURCES = \
                 Pingus.cc            Pingus.hh            \
		 CaptureRectangle.cc  CaptureRectangle.hh  \
		 FVec.cc	      FVec.hh              \
                 ActionButton.cc      ActionButton.hh      \
                 ActionHolder.cc      ActionHolder.hh      \
                 AlphaButton.cc       AlphaButton.hh       \
                 AnimCounter.cc       AnimCounter.hh       \
                 Background.cc        Background.hh        \
                 ButtonPanel.cc       ButtonPanel.hh       \
                 Client.cc            Client.hh            \
                 ClientEvent.cc  ClientEvent.hh            \
                 ColMap.cc            ColMap.hh            \
                 Counter.cc           Counter.hh           \
                 DebugGui.cc          DebugGui.hh          \
                 Direction.cc         Direction.hh         \
                 Entrance.cc          Entrance.hh          \
                 Exit.cc              Exit.hh              \
                 FadeOut.cc           FadeOut.hh           \
                 GameCounter.cc       GameCounter.hh       \
                 GameTime.cc          GameTime.hh          \
                 GuiObj.cc            GuiObj.hh            \
                 Hotspot.cc           Hotspot.hh           \
                 Intro.cc             Intro.hh             \
                 LevelInterrupt.cc    LevelInterrupt.hh    \
                 Liquid.cc            Liquid.hh            \
                 MainMenu.cc          MainMenu.hh          \
                 OptionMenu.cc        OptionMenu.hh        \
                 PLF.cc               PLF.hh               \
                 PLFParser.cc         PLFParser.hh         \
                 PLFPreview.cc        PLFPreview.hh        \
                 PLTParser.cc         PLTParser.hh         \
                 PSMParser.cc         PSMParser.hh         \
                 Pingu.cc             Pingu.hh             \
                 PinguAction.cc       PinguAction.hh       \
                 PinguBmpMap.cc       PinguBmpMap.hh       \
                 PinguEnums.hh                             \
                 PinguHolder.cc       PinguHolder.hh       \
                 PinguInfo.cc         PinguInfo.hh         \
                 PinguMap.cc          PinguMap.hh          \
                 PinguRandomMap.cc    PinguRandomMap.hh    \
                 PingusCounter.cc     PingusCounter.hh     \
                 PingusCursor.cc      PingusCursor.hh      \
                 PingusDemo.cc        PingusDemo.hh        \
                 PingusError.cc       PingusError.hh       \
                 PingusGame.cc        PingusGame.hh        \
                 PingusLevelDesc.cc   PingusLevelDesc.hh   \
                 PingusLevelResult.cc PingusLevelResult.hh \
                 PingusMenu.cc        PingusMenu.hh        \
                 PingusMessageBox.cc  PingusMessageBox.hh  \
                 PingusMusic.cc       PingusMusic.hh       \
                 PingusResource.cc    PingusResource.hh    \
                 PingusSound.cc       PingusSound.hh       \
                 PingusSpotMap.cc     PingusSpotMap.hh     \
                 Playfield.cc         Playfield.hh         \
                 QuestionDialog.cc    QuestionDialog.hh    \
                 Range.cc             Range.hh             \
                 ResDescriptor.cc     ResDescriptor.hh     \
                 Result.cc            Result.hh            \
                 Server.cc            Server.hh            \
                 SpecsReader.cc       SpecsReader.hh       \
                 SurfaceButton.cc     SurfaceButton.hh     \
                 System.cc            System.hh            \
                 Theme.cc             Theme.hh             \
                 ThemeSelector.cc     ThemeSelector.hh     \
                 TimeDisplay.cc       TimeDisplay.hh       \
                 Trap.cc              Trap.hh              \
                 TrueServer.cc        TrueServer.hh        \
                 View.cc              View.hh              \
                 World.cc             World.hh             \
                 WorldObj.cc          WorldObj.hh          \
                 algo.cc              algo.hh              \
                 background_data.cc   background_data.hh   \
                 blitter.cc           blitter.hh	   \
                 button_data.cc       button_data.hh       \
                 entrance_data.cc     entrance_data.hh     \
                 exit_data.cc         exit_data.hh         \
                 globals.cc           globals.hh           \
                 hotspot_data.cc      hotspot_data.hh      \
                 liquid_data.cc       liquid_data.hh       \
                 trap_data.cc         trap_data.hh         \
                 Display.cc           Display.hh


