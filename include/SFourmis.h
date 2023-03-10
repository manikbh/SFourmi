//
// SFourmis.h
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
//
// Started on  Mon Feb  4 13:38:47 2002 Ghost in the Shell
// Last update Tue Oct 15 23:13:01 2002 Ghost in the Shell
//

#ifndef SFOURMIS_H
#define SFOURMIS_H

#include "Systeme.h"
#include "macros.h"

#include "debutils.h"
#include <cmath>
#include <fstream>
#include <iostream>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include "SDLInterface.h"

#include "IOsf.h"
#include "Utils.h"
#include "Wonders.h"
#include "Tunnel.h"
#include "Room.h"
#include "Pheromone.h"
#include "Clan.h"
#include "Animal.h"
#include "Cafard.h"
#include "Fourmi.h"
#include "Ouvrier.h"
#include "Larve.h"
#include "Reine.h"
#include "Sentinelle.h"
#include "Citerne.h"

#include "DataMap.h"
#include "User.h"

extern class DataMap MData; // TODO 2020 cleanup "extern" and global variables from Global.h
extern class User ZSF;

extern CRoom *terre;
extern CClan **les_clans;
extern CAnimal **les_ennemis;

// DONNEES GRAPHIQUES //
// Bons é jeter
extern int MouseXY[2];
extern int MouseXYD[2];

// DONNEES FOURMI //
extern BYTE PHERO_SIZE;

extern int VIEILLE;  // On verra ça plus tard
extern int Pherom_N; // Phéromone de danger
extern int Pherom_D; // Phéromone de danger
extern int PheroMax;
extern int population;
extern CAnimal *Autour[48];
// LOG pour FILE //
extern ofstream Log;
#endif
