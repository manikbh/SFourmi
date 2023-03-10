///////////////////////////////////////////////////////////////////////////////
//
//	GTKInterface.h: Procédures d'interface de GTK
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GTKInterface_H
#define GTKInterface_H

extern int StartInterface(void);
extern void LButtonDown(int X, int Y);
extern void MouseMove(int X, int Y);
extern void LButtonUp(int X, int Y);
extern void RButtonDown(int X, int Y);
extern void AnalyseTouche(GdkEventKey *ev);
extern void AnalyseEvent(GdkEvent *evenement);

#endif
