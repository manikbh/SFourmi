//
// InitX.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Mon Feb  4 13:40:49 2002 Ghost in the Shell
// Last update Tue Oct 22 21:11:29 2002 Ghost in the Shell
//

#include "SFourmis.h"

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"
#include "../../images/All.xpm"
//#include "../../images/Icon1.xpm" // Icon is not used


bool GraphX_Init(void)
{
  if (SDL_Init(/*SDL_INIT_AUDIO|*/SDL_INIT_VIDEO)<0){
    SDEBUG(W0, "Echec de l'initialisation de SDL !");
    exit(0);
  }
  //Nettoyer SDL a la fin de l'execution
  atexit(SDL_Quit);
  //Créer une fenêtre 800x600 
  SDL_Window *sdlWindow = SDL_CreateWindow("SuperFourmi",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             800, 600,
                             SDL_WINDOW_RESIZABLE/*SDL_WINDOW_FULLSCREEN_DESKTOP*/);
  if(!sdlWindow){
    SDEBUG(W0, "Echec de l'initialisation du mode video !");
    exit(1);
  }
  sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0/*SDL_RENDERER_PRESENTVSYNC*/);
  if(!sdlRenderer){
    SDEBUG(W0, "Echec de l'initialisation du mode video !");
    exit(1);
  }
  // Render 800x600, scale automatically with linear interpolation
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  
  SDL_RenderSetLogicalSize(sdlRenderer, 800, 600);

  
  //idem pour la librairie des polices
  if (TTF_Init()<0){
    SDEBUG(W0, "Echec de l'initialisation de TTF !");
    exit(1);
  }	
  atexit(TTF_Quit);

  //  image = IMG_Load("/home/dodeskaden/Test/SFourmi/lib/Os/win/All.bmp");
  SDL_Surface *image = IMG_ReadXPMFromArray (const_cast<char **>(All_xpm)); //FIXME if All_xpm is not const, warnings, if it is SDL does not like it -> ugly fix
  if(image == NULL){
    SDEBUG(W0, "Impossible de charger l'image !");
    if(!fopen("/home/dodeskaden/Test/SFourmi/lib/Os/win/All.bmp","r"))
    {SDEBUG(W0, "Image inaccessible !");}
    else
    {SDEBUG(W0, "Image accessible !");}
    exit(1);
  }
  SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 4, 4, 4));
  /*if((image = SDL_DisplayFormat(image)) == NULL){
     SDEBUG(W0, "Impossible de convertir l'image au format d'affichage !");
     exit(1);
  }*/
  SFRect sfr(0,0,800,600);
  SFDrawSurface(0,0,sfr);

  if(! (SFfont=TTF_OpenFont("images/font.ttf", 10))){//On charge la police taille 10pt
    SDEBUG(W0, "Impossible de charger la police !");
    SDEBUG(W0, "images/font.ttf");
    exit(1);
  }
  TTF_SetFontStyle(SFfont, TTF_STYLE_NORMAL);
  /*SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer,
                               SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               800, 600);*/
  //Set image as a Texture in GPU memory
  sdlImageTexture = SDL_CreateTextureFromSurface(sdlRenderer, image);

  evenement = new SDL_Event;		
  return true;
}


///////////////////////////////////////////////////////////////////////////////
//						EXCLUSIVE WIN32 MODE
///////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
/*
 * retrouve les objets perdus
 *
 */
HRESULT restoreAll(void)
{
  HRESULT     ddrval;

  ddrval = lpDDSPrimary->Restore();
  if(ddrval == DD_OK)
  {
    ddrval = lpDDSOne->Restore();
    if(ddrval == DD_OK)
    {
      DDReLoadBitmap(lpDDSOne, szBitmap);
    }
    ddrval = lpDDSTwo->Restore();
    if(ddrval == DD_OK)
    {
      DDReLoadBitmap(lpDDSTwo, szBit);
    }
  }
  return ddrval;
}												  /* restoreAll */

///////////////////////////////////////////////////////////////////////////////
void finiObjects(void)
{
  if(lpDD != NULL)
  {
    if(lpDDSPrimary != NULL)
    {
      lpDDSPrimary->Release();
      lpDDSPrimary = NULL;
    }
    if(lpDDSOne != NULL)
    {
      lpDDSOne->Release();
      lpDDSOne = NULL;
    }

    if(lpDDSTwo != NULL)
    {
      lpDDSTwo->Release();
      lpDDSTwo = NULL;
    }
    if(lpDDPal != NULL)
    {
      lpDDPal->Release();
      lpDDPal = NULL;
    }
    lpDD->Release();
    lpDD = NULL;
  }
}												  /* finiObjects */


/*
 * Cette fonction est appelée si l'initialisation rate
 */
bool initFail(HWND hwnd)
{
  finiObjects();
  MessageBox(hwnd, "DirectDraw Init FAILED", "SFOURMIS", MB_OK);
  DestroyWindow(hwnd);
  return false;
}
#endif
