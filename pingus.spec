%define version 0.1.2
%define release 1
%define prefix /usr

Summary: Pingus, a lemmings style game with penguins.
Name: pingus
Version: %{version}
Release: %{release}
Copyright: GPL
Group: Amusements/Games
BuildRoot: /tmp/pingus-root
Source: pingus-%{version}.tar.gz
URL: http://dark.x.dtu.dk/~grumbel/pingus
Requires: ClanLib >= 0.1.15

%description
A cool lemmings game with penguins instead of lemmings! Try it. :-)
(someone should add a longer description here, with feature sets, etc.)

%prep
%setup

%build
./configure --prefix=%prefix
make

%install
make prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
%doc README* COPYING CREDITS ChangeLog
%{prefix}/games/pingus
%{prefix}/share/games/pingus/pingus.dat
%{prefix}/share/games/pingus/hotspots.spec
%{prefix}/share/games/pingus/levels/*
%{prefix}/info/pingus.info
