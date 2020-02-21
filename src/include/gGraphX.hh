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
# define gGRAPHX_H
# ifdef WIN32									  // WIN //

LPDIRECTDRAW            lpDD;	  // objet DirectDraw
LPDIRECTDRAWSURFACE     lpDDSPrimary;	  // Surface primaire DirectDraw
LPDIRECTDRAWSURFACE     lpDDSBack;	  // surface de fond DirectDraw
LPDIRECTDRAWSURFACE     lpDDSOne,lpDDSTwo;// Offscreen surface 1
LPDIRECTDRAWPALETTE     lpDDPal;	  // palette DirectDraw
bool                    bActive;	// est-ce que l'application est active?
HDC			ghdc;
static CBrush		BB1;
int			DrawMode = 0;
# endif

# ifdef GTK_Linux								  // GTK //
// données "locales"
GdkWindow		*BigFenetre;
GdkGC			*BigGC;
GdkPixmap		*IcoPixmap;
GdkPixbuf		*MainPixbuf;
GdkPixmap		*FlipPM;
GdkColor		MainTransp;
//GdkPixmap *TargetPixmap;//Le curseur
GdkFont			*police1;
bool			bActive = false;
//données "globales"
GdkEvent		*evenement;
//Pour les coordonnées
GdkColor		GDKColor1;
GdkGC			*GDKGC1;
# endif
#ifdef SF_SDL
//donnees globales
SDL_Event *evenement;
//donnees locales (graphique)
TTF_Font *SFfont=0;
SDL_Color fontColor;
SDL_Surface *image, *screen;
bool bActive=true;
#endif

class SFColor SFC;
struct SFFont font;

#endif
