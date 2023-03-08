# SFOURMI - SF TEAM 1998-2023


##				    1. ABOUT
SuperFourmi is an artificial life-style simulation of ants. It was
originally developed by highschool students starting in 1998 
and posted [on a website](http://sfourmi.free.fr).
The current version was slightly adapted from the original 2D, 
SDL1 version from 2002, and can be compiled with CMake on Linux
using SDL2.  
It is available [on Github](https://github.com/manikbh/SFourmi)


##				   2. AUTHORS
The SFourmi Program has be written by two students Michel Tran
Ngoc (T&T) and Manik Bhattacharjee (aka Kinam) with the help of FL 
and TATMF (graphics). 


##				   3. LICENCE
    Copyright (C) 1998-2023  Michel Tran Ngoc, Manik Bhattacharjee, 
    Tristan Millner, François Leiber

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>


##				4. COMPILING

Download the source code [from the website](https://github.com/manikbh/SFourmi) (either using git clone or the zip file).
You need to have SDL2, SDL2_image and SDL2_ttf installed in your system (e.g. libsdl2-dev, libsdl2-image-dev, libsdl2-ttf-dev packages in Ubuntu).
In a terminal, go to the directory of the project, then:
```
mkdir build
cd build
cmake ..
make
cd src
./SFourmi # to run it

```

Some work is in progress to compile SFourmi with emscripten so that it will be compiled to web assembly and run directly in the browser.

##				   5. INSTALL


* Requirements:
- 586-133Mhz with 2Mb of free disk and some RAM (128Mb minimum recommanded)

One of these operating systems is needed:
1 Linux or xBSD with SDL2. (www.libsdl.org)
2 Microsoft Windows(tm) 95 or higher with DirectX installed( >5) (probably 
not working anymore, untested)

LINUX :
Binary packages are not provided anymore, follow compilation instructions.
Then copy /usr/doc/sfourmi-{version}/sfourmi.ini in your $HOME directory.
This file is NEEDED to configure the program, if not found it will
run with default values.

WINDOWS :
Not sure about the current state.


## 6. What is/is not SFourmis ?

* SFourmi is NOT a game. It could be as fun as game but the
  interactivity is limited. Maybe, it will become a true game in the next
  century. :-p

 * SFourmi IS a simulation of artificial life that tries to simulates a
     real ant's life by copying the reality. Each ant is self-dependant and
			  can communicate with others.


  
##		       7. Keys, Actions and Informations.
F2	Go inside the nest.
F3	Focus on the nest's main entrance.
F4	Enable/Disable the mini-map.
F5	Focus on the selected ant.
F9	Save the map and its ants. (under Sfourmi.sf in the current directory)
F11-F12 Change the colony.
V	Enable/Disable the violent mode (Ants kill their ennemies)
S	Show/Hide the statistics
T	Enable/Disable Turbo mode.
F	Full Screen

! To load a saved map, just 'SFourmi name_of_map'

Arrow keys move around the map. You can move by clicking on the mini-map, too.

     For further informations, don't hesitate to mail us and visit our site
		sfourmi.log@online.fr or manik-fourmi@altern.org
