Pingus
======
Guide your penguins to safety!

Introduction
------------

Pingus is a free, open-source puzzle game inspired by the classic Lemmings™ clone. It is available for GNU/Linux, Win32 and other
operating systems supported by SDL2. Pingus Challenges you to guide a group of penguins through various obstacles to reach the exit safely. The game features engaging graphics, a variety of levels, and a built in level editor for endless possibilities and fun. It is licensed under the GPLv3+.

Information about the game, new versions and additional documentation
can be found at the Pingus webpage:

* https://pingus.github.io/

Table of Contents
-------------

    Features
    Installation
        Linux
        Windows
        macOS
    Building
    Launching the Game
    Game Controls
    Community
        Mailing Lists
        Chat
    Bug Reports
    Contributing
    License

Features
--------

  *  Over 100 Challenging Levels: Navigate through a variety of levels with increasing difficulty.
  *  Level Editor: Create your own levels and share them with the community.
  *  Engaging Graphics: Enjoy cute and colorful graphics that bring the game to life.
  *  Sound Effects and Music: Immerse yourself in the game with delightful audio.
  *  Multilingual Support: Available in multiple languages.
  *  Cross-Platform: Play on Linux, Windows, and other systems supported by SDL2.

Installation
--------
Linux)
Pingus can be installed on Linux using the Nix package manager:
    nix run github:pingus/pingus

Alternatively, Pingus may be available in your distribution's repositories:

Ubuntu/Debian:
    sudo apt-get install pingus

Fedora:
    sudo dnf install pingus
    
Arch Linux:
    sudo pacman -S pingus

Windows)

Download the latest Windows installer from the Pingus Releases page or compile it on Linux using:
    nix build github:pingus/pingus#packages.i686-windows.pingus-win32

macOS)

Currently, Pingus does not have an official macOS installer. You can compile it from source using the instructions below.


Building
--------

Pingus uses [Nix package manager](https://nixos.org/download/), to
automatically download, compile and run it in Linux:

    nix run github:pingus/pingus

The Windows version can be compiled in Linux with:

    nix build github:pingus/pingus#packages.i686-windows.pingus-win32


Launching the Game
-----------
After installation or building from source, you can launch Pingus from your application menu or by running the following command in the terminal:
    pingus

If you used Nix to run the game:
    nix run github:pingus/pingus

Game Controls
----------
Action	                Key
Move cursor	            Mouse 
Select/Deselect	        Left Mouse Button
Assign Action	        Left Mouse Button on penguin
Pause/Unpause Game	    P
Speed Up Game	        + (Plus key)
Slow Down Game	        - (Minus key)
Open In-Game Menu	    Esc
Move Map	            Move mouse to screen edge or use Arrow Keys
Zoom In	                Page Up
Zoom Out	            Page Down
Toggle Fullscreen	    F
Restart Level	        R
Exit Level	            E
Display Help	        H
Quit Game	            Ctrl + Q

Can Change game controls in the settings menu.

Mailing Lists
-------------

* pingus-devel - https://lists.nongnu.org/mailman/listinfo/pingus-devel
  A list for discussing development issues, like level
  creation or code changes, but might as well be used for user issues
  like how to solve levels and such


Chat
----

* IRC: irc://irc.libera.chat:6667/#pingus

* Matrix: https://matrix.to/#/#pingus:matrix.org


Bug Reports
-----------

Bug reports, new levels and feature requests can be submitted to the
issue tracker at:

* https://github.com/pingus/pingus/issues


Contributing
--------

Contributions are welcome! Whether it's reporting bugs, creating levels, improving documentation, or contributing code, your help is appreciated.

   * Code Contributions: Fork the repository, make your changes, and submit a pull request.
   * Level Creation: Use the built-in level editor and share your levels with the community.
   * Translations: Help translate Pingus into other languages.
