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
# define SFOURMIS_H

#include "Systeme.h"
#include "macros.h"

#include "debutils.h"
#include <math.h>
#include <fstream>
#include <iostream>
#include <assert.h>

#ifdef WIN32		// WIN //

#include "StdAfx.h"
//#include <assert.h>
#include <ddraw.h>
//#include <dmusicc.h>
//#include <dmusici.h>
#include "resrc1.h"
#include "resource.h"
#include "ddutil.h"
#include "WinInterface.h"

#endif

#ifdef GTK_Linux	// GTK //
# include <gdk/gdk.h>
# include <time.h>
# include <stdlib.h>
# include <string.h>
#endif

#ifdef SF_SDL      // SDL //
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include "SDLInterface.h"
#endif

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

extern class DataMap	MData;
extern class User	ZSF;


extern CRoom*		terre;
extern CClan**		les_clans;
extern CAnimal**	les_ennemis;

// DONNEES GRAPHIQUES //
// Bons à jeter
extern int		MouseXY[2];
extern int		MouseXYD[2];

// DONNEES FOURMI //
extern BYTE		PHERO_SIZE;

extern int 		VIEILLE;	// On verra ça plus tard
extern int		Pherom_N;	// Phéromone de danger
extern int		Pherom_D;	// Phéromone de danger
extern int		PheroMax;
extern int		population;
extern CAnimal*		Autour[48];
// LOG pour FILE //
extern ofstream		Log;
#endif

