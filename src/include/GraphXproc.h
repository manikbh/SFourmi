///////////////////////////////////////////////////////////////////////////////
//
//	GraphXproc.h: Utiliser les m�me proc�dures graphiques sous Win et GTK
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GRAPHXPROC_H
# define GRAPHXPROC_H

extern SDL_Event         *evenement;
extern bool             GraphX_Init();
extern TTF_Font          *SFfont;
extern SDL_Color	fontColor;		
extern SDL_Surface	*screen, *image;

extern bool		bActive;        // Est-ce que l'application est active ?
extern void		updateFrame(BYTE Frame);
extern void		DisplayInfo(class DataMap &MData, class User &ZSF);
extern void		Flips(void);
extern void		SmallMap(void);
extern void		SelectMultiple(void);
extern struct SFFont		font;
#endif
