#  $Id: Makefile.am,v 1.41 2000/10/12 20:18:12 grumbel Exp $
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

# The next line is for gettext
LIBS = @INTLLIBS@ @LIBS@

SUBDIRS = \
	actions             \
	backgrounds         \
	caimagemanipulation \
	editor              \
	entrances           \
	generic             \
	particles           \
	traps               \
	war                 \
	worldmap

EXTRA_DIST = README \
             win32/Getopt.c  \
             win32/Getopt.h  \
             win32/Getopt1.c \
             win32/config.h  \
             make_docs.sh

bin_PROGRAMS = pingus

LDADD = caimagemanipulation/libcaimagemanipulation.a \
	traps/libpingu_traps.a          \
        actions/libpingu_actions.a      \
        entrances/libpingus_entrances.a \
        particles/libpingus_particle.a  \
        editor/libpingus_editor.a       \
        backgrounds/libpingus_backgrounds.a       \
        war/libpingus_war.a \
        worldmap/libpingus_worldmap.a \
        generic/libpingus_generic.a

pingus_SOURCES = \
                 GenericMain.cc       GenericMain.hh       \
                 PingusMain.cc        PingusMain.hh        \
		 CaptureRectangle.cc  CaptureRectangle.hh  \
		 FVec.cc	      FVec.hh              \
                 ActionButton.cc      ActionButton.hh      \
                 ActionHolder.cc      ActionHolder.hh      \
                 AlphaButton.cc       AlphaButton.hh       \
                 AnimCounter.cc       AnimCounter.hh       \
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
                 XMLPLF.cc            XMLPLF.hh               \
                 PLFPLF.cc            PLFPLF.hh               \
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
                 PingusCounter.cc     PingusCounter.hh     \
                 DemoRecorder.cc      DemoRecorder.hh      \
                 DemoPlayer.cc        DemoPlayer.hh        \
                 PingusError.cc       PingusError.hh       \
                 PingusGame.cc        PingusGame.hh        \
                 PingusLevelDesc.cc   PingusLevelDesc.hh   \
                 PingusLevelResult.cc PingusLevelResult.hh \
                 PingusMenu.cc        PingusMenu.hh        \
                 PingusMessageBox.cc  PingusMessageBox.hh  \
                 PingusResource.cc    PingusResource.hh    \
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
                 WorldObjData.cc      WorldObjData.hh      \
                 algo.cc              algo.hh              \
                 blitter.cc           blitter.hh	   \
                 ActionData.hh        \
	         EntranceData.hh     \
                 ExitData.hh         \
                 globals.cc           globals.hh           \
                 HotspotData.hh       \
                 LiquidData.hh        \
                 TrapData.hh          \
                 Display.cc           Display.hh           \
                 Loading.cc           Loading.hh           \
                 Credits.cc           Credits.hh           \
                 SmallMap.cc          SmallMap.hh          \
                 Config.cc            Config.hh            \
                 PingusSound.cc       PingusSound.hh       \
                 PingusSoundReal.cc       PingusSoundReal.hh       \
                 PingusMusicProvider.cc PingusMusicProvider.hh \
                 PingusWavProvider.cc PingusWavProvider.hh \
                 audio.cc             audio.hh \
                 Timer.cc             Timer.hh \
                 MultiLineText.cc     MultiLineText.hh \
                 GlobalEvent.cc       GlobalEvent.hh \
                 Screenshot.cc        Screenshot.hh  \
                 FPSCounter.cc        FPSCounter.hh  \
                 Console.cc           Console.hh \
                 Story.cc             Story.hh  \
                 HurryUp.cc           HurryUp.hh \
                 StringConverter.cc   StringConverter.hh \
                 SurfaceData.hh \
                 XMLhelper.cc         XMLhelper.hh \
                 PLTXML.cc            PLTXML.hh   \
                 WeatherData.hh   \
                 Teleporter.cc Teleporter.hh \
                 TargetProvider.hh    TargetProvider.cc \
                 PingusMapManager.hh  PingusMapManager.cc

#                 PinguRandomMap.cc    PinguRandomMap.hh    

## EOF ##
