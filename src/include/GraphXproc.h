///////////////////////////////////////////////////////////////////////////////
//
//	GraphXproc.h: Utiliser les même procédures graphiques sous Win et GTK
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GRAPHXPROC_H
# define GRAPHXPROC_H
#ifdef SF_SDL
extern SDL_Event         *evenement;
extern bool             GraphX_Init();
extern TTF_Font          *SFfont;
extern SDL_Color	fontColor;		
extern SDL_Surface	*screen, *image;
#endif

extern bool		bActive;        // Est-ce que l'application est active ?
extern void		UpdateFrame(BYTE Frame);
extern void		DisplayInfo(class DataMap &MData, class User &ZSF);
extern void		info_plus(void);
extern void		splash(void);
extern void		Init_screen(void);
extern void		s_ini(BYTE phase, char what[]);
extern void		s_sauver(BYTE phase);
extern void		Flips(void);
extern void		SmallMap(void);
extern void		SelectMultiple(void);
extern struct SFFont		font;
extern float Cos[360], Sin[360], Tan[360];
#endif
