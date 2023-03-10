///////////////////////////////////////////////////////////////////////////////
//
//	GTKInterface.h: Procédures d'interface de GTK
//
///////////////////////////////////////////////////////////////////////////////
#ifdef SF_SDL
#ifndef SDLInterface_H
#define SDLInterface_H

extern int StartInterface(void);
extern void LButtonDown(int X, int Y);
extern void MouseMove(int X, int Y);
extern void LButtonUp(int X, int Y);
extern void RButtonDown(int X, int Y);
extern void AnalyseTouche(SDL_KeyboardEvent *key, bool up);
extern void AnalyseEvent(SDL_Event *evenement);
// Pour ignorer les constantes DirectX
#define DDBLTFAST_NOCOLORKEY 0
#define DDBLTFAST_WAIT 0
#define DDBLTFAST_SRCCOLORKEY 0
#define TRUE true
#define FALSE false

#endif
#endif
