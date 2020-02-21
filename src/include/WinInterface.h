////////////////////////////////////////////////////////////////////////////////
//
//	WinInterface.h: En-tête de WinInterface.cpp
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WININTERFACE_H_
#define _WININTERFACE_H_

extern HCURSOR hCursor;
extern HCURSOR hCbouffe;
extern HFONT    hFont1;
extern HFONT    hFont2;
extern HFONT    hFont3;
extern HFONT    hFont4;
extern CBrush BB3;	// Blanc pur

// Déclarations en extern non pas obligatoire mais par convention...
extern void ErrMsg (HWND hwnd, LPCSTR pszFmt, ...);
extern void FontFactory(void);
extern void MouseMove(WPARAM fwKeys, int X, int Y);
extern void LButtonDown(WPARAM fwKeys, int X, int Y);
extern void LButtonUp(WPARAM fwKeys, int X, int Y);
extern void RButtonDown(WPARAM fwKeys, int X, int Y);
extern long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern int StartInterface(void);
extern bool doInit(HINSTANCE hInstance, int nCmdShow);

#endif
