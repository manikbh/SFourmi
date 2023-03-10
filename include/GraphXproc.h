///////////////////////////////////////////////////////////////////////////////
//
//	GraphXproc.h: Utiliser les même procédures graphiques sous Win et GTK
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GRAPHXPROC_H
#define GRAPHXPROC_H

extern SDL_Event *evenement;
extern bool GraphX_Init();
extern TTF_Font *SFfont;
extern SDL_Color fontColor;
extern SDL_Renderer *sdlRenderer;
extern SDL_Texture *sdlImageTexture;

extern bool bActive; // Est-ce que l'application est active ?
extern void updateFrame(BYTE Frame);
extern void DisplayInfo(class DataMap &MData, class User &ZSF);
extern void Flips(void);
extern void SmallMap(void);
extern void SelectMultiple(void);
extern struct SFFont font;
#endif
