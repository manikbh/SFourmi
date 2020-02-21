/*
** GraphXtools.h
**
** Made by (Ghost in the Shell)
** Login   <dodeskaden@epita.fr>
** 
** Started on  Sat Jan 26 15:22:25 2002 Ghost in the Shell
** Last update Thu Jan 31 00:05:46 2002 Ghost in the Shell
*/

#ifndef GRAPHXTOOLS_H
# define GRAPHXTOOLS_H

extern void SFDrawMode(int Mode);
extern void SFDisplay(int X, int Y, char *chaine);
extern void SFTextColor(int r, int g, int b);
extern void SFDrawSingleRectangle(SFRect& rcRect,SFColor& color);
extern void SFDrawDCRectangle(SFRect& rcRect,SFColor& color);
extern void SFSelectFont(BYTE n);
extern void SFGetDC();
extern void SFReleaseDC();
extern void SFDrawSurface(int x0,int y0, SFRect& rcRect);
extern void SFFastDrawSurface(unsigned long x0,unsigned long y0, SFRect& rcRect);

#endif
