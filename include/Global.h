//
// Global.h
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Mon Feb  4 12:28:50 2002 Ghost in the Shell
// Last update Mon Feb  4 12:33:32 2002 Ghost in the Shell
//

#ifndef GLOBAL_H
# define GLOBAL_H

//TODO 2020 : clean up this mess - terre is allocated in User but used externally...
//CCase **map;
CRoom*		terre;
CClan**		les_clans;
CAnimal**	les_ennemis;

// DONNEES GRAPHIQUES //
int MouseXY[2];
int MouseXYD[2];

// DONNEES FOURMI //
BYTE PHERO_SIZE = 3;
int VIEILLE = 25000;	// On verra ça plus tard
int Pherom_N = 100;		// Phéromone de nourriture
int Pherom_D = 100;		// Phéromone de danger
int PheroMax = 400;
int population = 0;
int intense = 0;
CAnimal* Autour[48];
int const	CPheromone::Volutabily_meat = 2;
int const	CPheromone::Volutabily_danger = 2;

// Log for FILE //
ofstream Log;

char file[18];

#endif
