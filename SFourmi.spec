#
# SFourmi -- Super Fourmi (C) SF Team
# This is a template.  The dist target uses it to create the real file.
#
%define version 0.42.4
%define name SFourmi
Summary: Program simulates ant's activities with complex algorithms
Summary(fr): Programme simulant l'activité des fourmis
Vendor: SF Team
Name: %{name}
Version: %{version}
Release: 1
Copyright: SF Team (restricted licence, see the README please)
Group: Applications/Divers
Group(fr): Applications/Divers
Source: http://sfoumi.free.fr/pub/%{name}-%{version}.tar.gz
URL: http://sfourmi.free.fr
Provides: SFourmi
BuildRoot: /tmp/rpmbuild_%{name}

%changelog
* Tue Oct 15 2002 GiTs <sfourmi.log@online.fr> Version 0.42r2
Updates for autotools
Correct some mistakes in the Makefile.am (no more needs to have INCLUDE
field) and add sdl detection support in configure for a better behavior
Add path support for fonts loading by $prefix$
Then add SFourmi.spec to be able to generate a rpm via
'rpm -ta SFourmi-{version}.tar.gz'
 
* Sat Oct 7 2002 Kinam <manik-fourmi@altern.org> Version 0.42r1 'SDL' Version
Ported SFourmi to SDL. Display works well and is much faster than GTK.
However, events are not completely supported for now. Should be really soon.
Saving seems to work but is sloooooooow. Idea by GiTs : to split the pointer
table by categories of objects should be implemented. Loading is fast on the
contrary. Did not check the potential problems with saving.
Autotools files aren't clean ( -lSDL everywhere and so on), no tests under Windows.
 
* Fri Jul 12 2002 GiTs <sfourmi.log@online.fr> Version 0.41r222
Correction of crashes under win98.
I tested some time under Win98 and gtk. The saving seems to have some
lacks. It doesn't save the 'nourri' that I placed on the grid. Moreover,
I feel that pheromons aren't keep too. Manik : action ! We need that
correction to update to 0.42
 
* Wed Jul 10 2002 Kinam <manik-fourmi@altern.org>,GiTs <sfourmi.log@online.fr> Version 0.41r200
Window version again !!! The save module should have been fixed now. Try
F9 to test it. I need to add a load option to see if the loading'll work.
But it should. I believe it's time to change the version number since a
lot of work have been made. I may update the web site (?)
You can also load an sfourmi.sf by configuring the sfourmi.ini
For now it's very simple and unconvivial; but we could dream that a day
it would be very very easy and fast to do that kind of trick

%description
Super fourmi is a simulation of one or more ants' nest with the outdoor too.
This program that will use fuzzy logic and maybe genetic algorithm was entirely
written in C++. For convenience and for speed, we use directX under win32 and
SDL or gtk under linux.

(Note from the web-page)
If you are interested in ants because you were impressed by their organization
when you were a child, if you are still wondering how grows and lives an ants'
nest, if you want to learn something usefull for your social life, just
navigate through this site for information (project, photos) and don't forget to
download the binary in the yellow box. 

%description -l fr
Super fourmi est une simulation d'une ou plusieurs fourmillières et le terrain
extérieur. Ce programme qui utilisera de la logique floue et peut-étre des
algorithmes génétiques, a été entiérement écrit en C++. Par commodité et pour
la vitesse, nous utilisons DirectX sous Windows et SDL ou gtk sous linux.

(Note du site)
Si les fourmis vous intéressent, si vous avez été impressionné(e) par leur
organisation quand vous étiez enfant, si vous vous demandez toujours comment
vit et grandit une fourmillière, si vous voulez apprendre comment gérer votre
vie sociale, parcourez ce site pour des informations plus détaillées (projet,
photos) et n'oubliez pas de télécharger l'exécutable dans le cadre jaune.
%prep
rm -rf $RPM_BUILD_ROOT
rm -rf $RPM_BUILD_DIR/%{name}-%{version}

%setup

%build
if test -n "$LINGUAS"; then
 unset LINGUAS
fi    
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=/usr 
make

%install
make install-strip prefix=$RPM_BUILD_ROOT/usr

%files

%doc %attr (-,root,root) README
%doc %attr (-,root,root) docs/sfourmi.ini

#%attr (-,root,root) /usr/man/man1/gpg*.1*
#%attr (4755,root,root) /usr/bin/gpg
#%attr (-,root,root) /usr/bin/gpgkeys_*
%attr (4755,root,root) /usr/bin/SFourmi
#%config %attr (-,root,root) /usr/share/SFourmi
#%config %attr (-,root,root) /usr/share/SFourmi/images
%config %attr (-,root,root) /usr/share/SFourmi/images/font.ttf

#%attr (-,root,root) /usr/share/locale/*/*/%{name}.mo

#%attr (-,root,root) /usr/lib/%{name}

#%config %attr (-,root,root) /usr/share/%{name}/options.skel

%clean
rm -rf $RPM_BUILD_ROOT
rm -rf $RPM_BUILD_DIR/%{name}-%{version}
