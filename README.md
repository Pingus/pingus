Pingus
======

Introduction
------------

Pingus is a free Lemmings™ clone for GNU/Linux, Win32 and other
operating systems supported by SDL2. It is licensed under the GPLv3+.

Information about the game, new versions and additional documentation
can be found at the Pingus webpage:

* https://pingus.github.io/


Building
--------

Pingus uses [Nix package manager](https://nixos.org/download/), to
automatically download, compile and run it in Linux:

    nix run github:Pingus/pingus

Windows cross builds are produced from a Linux host (packages live under
the *host* system, not under `packages.*-windows`):

    nix build github:Pingus/pingus#pingus-win32-x64   # mingwW64 → x86_64 PE
    nix build github:Pingus/pingus#pingus-win32-x86   # mingw32  → i686 PE

Zipped redistributables:

    nix build github:Pingus/pingus#pingus-win32-x64-zip
    nix build github:Pingus/pingus#pingus-win32-x86-zip


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

