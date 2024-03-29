////////////////////////////////////////////////////////////////////////////////
//
//	Ddutil.h: En-téte de Ddutil.cpp
//
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{ /* Assume C declarations for C++ */
#endif

    extern IDirectDrawPalette *DDLoadPalette(IDirectDraw *pdd, LPCSTR szBitmap);
    extern IDirectDrawSurface *DDLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy);
    extern HRESULT DDReLoadBitmap(IDirectDrawSurface *pdds, LPCSTR szBitmap);
    extern HRESULT DDCopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
    extern DWORD DDColorMatch(IDirectDrawSurface *pdds, COLORREF rgb);
    extern HRESULT DDSetColorKey(IDirectDrawSurface *pdds, COLORREF rgb);

#ifdef __cplusplus
}
#endif
