//
// gGraphX.hh
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
//
// Started on  Mon Feb  4 12:29:01 2002 Ghost in the Shell
// Last update Mon Feb  4 12:29:01 2002 Ghost in the Shell
//

#ifndef gGRAPHX_H
#define gGRAPHX_H

// donnees globales //TODO 2020 global vars again ! screen is created in InitX.cpp
SDL_Event *evenement;
// donnees locales (graphique)
TTF_Font *SFfont = 0;
SDL_Color fontColor;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlImageTexture;
bool bActive = true;

class SFColor SFC;
struct SFFont font;

#endif
