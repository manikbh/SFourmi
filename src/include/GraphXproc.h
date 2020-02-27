///////////////////////////////////////////////////////////////////////////////
//
//	GraphXproc.h: Utiliser les même procédures graphiques sous Win et GTK
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GRAPHXPROC_H
# define GRAPHXPROC_H

# ifdef WIN32		// WINDOWS //
static char			szBitmap[] = "ALL";
static char			szBit[] = "SPLASH";
static char			CURSEUR[] = "POINTEUR";
static char			CBOUFFE[] = "PBOUFFE";
static char			szMont[] = "MONT";
extern LPDIRECTDRAW		lpDD;
extern LPDIRECTDRAWSURFACE	lpDDSPrimary;
extern LPDIRECTDRAWSURFACE	lpDDSBack;
extern LPDIRECTDRAWSURFACE	lpDDSOne,lpDDSTwo;
extern LPDIRECTDRAWPALETTE	lpDDPal;

extern HRESULT			restoreAll(void);
extern void			finiObjects(void);
extern bool			initFail(HWND hwnd);
extern bool			GraphX_Init(HWND hwnd);
# endif

# ifdef GTK_Linux	// GTK //
extern void		GraphInit();
extern void		MessageBox(char *info);
extern void		GDKDestroy();
extern GdkEvent		*evenement;
extern bool		GraphX_Init();
extern GdkFont		*police1;
extern GdkPixmap	*FlipPM;
extern GdkGC		*jauneGC;
extern GdkGC		*BigGC;
extern GdkPixbuf	*MainPixbuf;
extern GdkColor		GDKColor1;
extern GdkGC		*GDKGC1;
# endif

#ifdef SF_SDL
extern SDL_Event         *evenement;
extern bool             GraphX_Init();
extern TTF_Font          *SFfont;
extern SDL_Color	fontColor;		
extern SDL_Surface	*screen, *image;
#endif

#ifdef WIN32
  extern int DrawMode;
  extern HDC	ghdc;
#endif

extern bool		bActive;        // Est-ce que l'application est active ?
extern void		updateFrame(BYTE Frame);
extern void		DisplayInfo(class DataMap &MData, class User &ZSF);
extern void		info_plus(void);
extern void		Flips(void);
extern void		SmallMap(void);
extern void		SelectMultiple(void);
extern struct SFFont		font;
#endif
