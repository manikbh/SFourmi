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
#include "config.h"
#ifdef WIN32
# include "WinInterface.h"
#endif

#ifdef GTK_Linux
# include <gdk/gdk.h>
# include <gdk-pixbuf/gdk-pixbuf.h>
# include "GTKInterface.h"
#endif

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"
#include "../../images/All.xpm"
#include "../../images/Icon1.xpm"

#ifdef WIN32
 bool
GraphX_Init(HWND hwnd)
#endif
#ifdef GTK_Linux
  GdkGC			*jauneGC = 0,
  *rougeGC = 0,
  *vertGC = 0,
  *violetGC = 0;
extern GdkWindow	*BigFenetre;
 bool
GraphX_Init(void)
#endif
#ifdef SF_SDL
  /*
     SDL_Color fontColor;
     SDL_Surface *image, *screen;*/
  bool
GraphX_Init(void)
#endif
{
#ifdef WIN32
  DDSURFACEDESC       ddsd;
  DDSCAPS             ddscaps;
  HRESULT             ddrval;

  /*
   * create the main DirectDraw object
   */
  ddrval = DirectDrawCreate(NULL, &lpDD, NULL);
  if (ddrval != DD_OK)
    return initFail(hwnd);

  // Get exclusive mode
  ddrval = lpDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);

  if (ddrval != DD_OK)
    return initFail(hwnd);
  //!!!!! !!!! MODE IMPOSE !!!! !!!!!
  ddrval = lpDD->SetDisplayMode(DF_X, DF_Y, 24);
  if (ddrval != DD_OK)
    return initFail(hwnd);

  // Create the primary surface with 1 back buffer
  ddsd.dwSize = sizeof(ddsd);
  ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
    DDSCAPS_FLIP |
    DDSCAPS_COMPLEX;
  ddsd.dwBackBufferCount = 2;
  ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
  if(ddrval != DD_OK)
  {
    return initFail(hwnd);
  }

  ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
  ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);
  if(ddrval != DD_OK)
  {
    return initFail(hwnd);
  }

  // create and set the palette
  //lpDDPal = DDLoadPalette(lpDD, szBitmap);

  if (lpDDPal)
    lpDDSPrimary->SetPalette(lpDDPal);

  // Create the offscreen surface, by loading our bitmap.
  lpDDSOne = DDLoadBitmap(lpDD, szBitmap, 0, 0);
  lpDDSTwo = DDLoadBitmap(lpDD, szBit,0,0);
  if(lpDDSOne == NULL)
  {
    return initFail(hwnd);
  }

  // Set the color key for this bitmap (black)
  // NOTE this bitmap has black as entry 255 in the color table.
  //    ddck.dwColorSpaceLowValue = 0xff;
  //    ddck.dwColorSpaceHighValue = 0xff;
  //    lpDDSOne->SetColorKey(DDCKEY_SRCBLT, &ddck);

  // if we did not want to hard code the palette index (0xff)
  // we can also set the color key like so...
  DDSetColorKey(lpDDSOne, RGB(4,4,4));
  DDSetColorKey(lpDDSTwo, RGB(4,4,4));
  return(true);
#endif //FIN WIN32
#ifdef GTK_Linux	// GTK

  GdkWindowAttr		attributs;
  GdkBitmap		*IcoMasq;
  GdkColor		IcoTransp;
  GdkColor		jaune,vert,rouge,violet;
  GdkPixmap		*IcoPixmap;
  /*******************************Déclarations****************************/

  //GdkCursor *Target = gdk_cursor_new_from_pixmap;
  /*if (argc>0){ //les args sont utilisés par GTK...
    if (strncmp(argv[0],"sfelkheg",8)==0)
    info_plus();
    }
    else
    splash(); */

  /*******************************Initialisation de la fenêtre****************************/
  //Pour les conversions des GdkPixbuf
  gdk_rgb_init();

  //voir p129 bouquin GTK+
  attributs.event_mask = GDK_BUTTON_MOTION_MASK | GDK_KEY_PRESS_MASK;
  attributs.width = DF_X;
  attributs.height = DF_Y;
  attributs.wclass = GDK_INPUT_OUTPUT;
  attributs.window_type = GDK_WINDOW_TOPLEVEL;
  //gdk_set_show_events(true);
  BigFenetre= gdk_window_new(NULL, &attributs, 0);
  //sprintf(PATH,"%s/GTKFourmi/gtk/All.xpm",HOME);
  //printf("%s\n",PATH);
  char titreFenetre[80];
  sprintf(titreFenetre,"SuperFourmis - version %s par T&T et Kinam",VERSION);
  gdk_window_set_title(BigFenetre,titreFenetre);
  gdk_window_set_hints (BigFenetre,0,0,DF_X, DF_Y, DF_X, DF_Y,GDK_HINT_MIN_SIZE | GDK_HINT_MAX_SIZE);
  //gdk_pixmap_new(BigFenetre,800,600,gdk_window_get_visual(BigFenetre)->depth);
  MainPixbuf = gdk_pixbuf_new_from_xpm_data((const char **)&All_xpm);
  cout<<"Transparence :"<<gdk_pixbuf_get_has_alpha (MainPixbuf);
  //GDK_COLORSPACE_RGB
  //gdk_pixbuf_new(GDK_COLORSPACE_RGB,true,gdk_pixbuf_get_bits_per_sample (MainPixbuf),800,600);
  FlipPM = gdk_pixmap_new(BigFenetre,800,600,gdk_window_get_visual(BigFenetre)->depth);
  //gdk_beep();
  //sprintf(PATH,"%s/GTKFourmi/gtk/Icon1/xpm",HOME);
  //printf("%s\n",PATH);
  IcoPixmap = gdk_pixmap_create_from_xpm_d(BigFenetre,&IcoMasq,&IcoTransp,Icon1_xpm);
  BigGC = gdk_gc_new (BigFenetre);
  gdk_gc_set_line_attributes(BigGC,1,GDK_LINE_SOLID,GDK_CAP_BUTT,GDK_JOIN_MITER);
  police1=gdk_font_load("-*-courier-*-*-*-*-10-*-*-*-*-*");
  //gdk_window_set_back_pixmap(BigFenetre,MainPixmap,false);
  //gdk_draw_pixmap((GdkDrawable*)BigFenetre,BigGC,MainPixmap,0,600,100,140,32,32);
  gdk_window_set_icon(BigFenetre,NULL,IcoPixmap,IcoMasq);
  //Small Map
  jaune.red=65535;jaune.green=65535;jaune.blue=0;
  vert.red=0;vert.green=65535;vert.blue=0;

  rouge.red=65535;rouge.green=0;rouge.blue=0;
  violet.red=45535;violet.green=0;violet.blue=45535;
  //gdk_colormap_alloc_color(gdk_colormap_get_system(),&MainTransp,false,true);//PAS SUR ! ! !
  gdk_colormap_alloc_color(gdk_colormap_get_system(),&jaune,false,true);
  gdk_colormap_alloc_color(gdk_colormap_get_system(),&rouge,false,true);
  gdk_colormap_alloc_color(gdk_colormap_get_system(),&vert,false,true);
  gdk_colormap_alloc_color(gdk_colormap_get_system(),&violet,false,true);
  gdk_colormap_alloc_color(gdk_colormap_get_system(),&GDKColor1,false,true);

  jauneGC=gdk_gc_new(BigFenetre);
  vertGC=gdk_gc_new(BigFenetre);
  rougeGC=gdk_gc_new(BigFenetre);
  violetGC=gdk_gc_new(BigFenetre);
  GDKGC1 = gdk_gc_new(BigFenetre);

  gdk_gc_set_line_attributes(jauneGC,1,GDK_LINE_SOLID,GDK_CAP_BUTT,GDK_JOIN_MITER);

  gdk_gc_copy(vertGC, jauneGC);				  //Pour conserver les attributs de ligne de jaune...
  gdk_gc_copy(rougeGC, jauneGC);
  gdk_gc_copy(violetGC, jauneGC);
  gdk_gc_copy(GDKGC1, jauneGC);

  gdk_gc_set_foreground(jauneGC,&jaune); gdk_gc_set_foreground(vertGC,&vert);
  gdk_gc_set_foreground(rougeGC,&rouge); gdk_gc_set_foreground(violetGC,&violet);
  gdk_gc_set_foreground(GDKGC1,&GDKColor1);
  //Fin Small Map
  if((!FlipPM)||(!MainPixbuf)||(!IcoPixmap))
  {
    cout << "ERROR: Cannot load Pixmap\n";
    exit(0);
  }

  gdk_window_show(BigFenetre);
  return(true);
#endif //Fin GTK_Linux
#ifdef SF_SDL
  if (SDL_Init(/*SDL_INIT_AUDIO|*/SDL_INIT_VIDEO)<0){
    SDEBUG(W0, "Echec de l'initialisation de SDL !");
    exit(0);
  }
  //Nettoyer SDL a la fin de l'execution
  atexit(SDL_Quit);
  //S'attacher a l'ecran en 800,600, le bpp du systeme (0), acceleration hardware et double buffer
  if (!(screen = SDL_SetVideoMode(800,600,0,SDL_HWSURFACE|SDL_DOUBLEBUF))){ //TODO 2020 resolution should be a variable (but Map Image must be updated to reflect this)
    SDEBUG(W0, "Echec de l'initialisation du mode video !");
    exit(1);
  }
  //idem pour la librairie des polices
  if (TTF_Init()<0){
    SDEBUG(W0, "Echec de l'initialisation de TTF !");
    exit(1);
  }	
  atexit(TTF_Quit);

  //  image = IMG_Load("/home/dodeskaden/Test/SFourmi/lib/Os/win/All.bmp");
  image = IMG_ReadXPMFromArray (All_xpm);
  if(image == NULL){
    SDEBUG(W0, "Impossible de charger l'image !");
    if(!fopen("/home/dodeskaden/Test/SFourmi/lib/Os/win/All.bmp","r"))
    {SDEBUG(W0, "Image inaccessible !");}
    else
    {SDEBUG(W0, "Image accessible !");}
    exit(1);
  }
  SDL_SetColorKey(image, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(image->format, 4, 4, 4));
  if((image = SDL_DisplayFormat(image)) == NULL){
     SDEBUG(W0, "Impossible de convertir l'image au format d'affichage !");
     exit(1);
  }
  SFRect sfr(0,0,800,600);
  SFDrawSurface(0,0,sfr);

  if(! (SFfont=TTF_OpenFont(SFOURMI_DATADIR "/images/font.ttf", 10))){//On charge la police taille 10pt
    SDEBUG(W0, "Impossible de charger la police !");
    SDEBUG(W0, SFOURMI_DATADIR "/images/font.ttf");
    exit(1);
  }
  TTF_SetFontStyle(SFfont, TTF_STYLE_NORMAL);
  evenement = new SDL_Event;		
  return true;
#endif
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
