/*
** GraphXstruct.h
**
** Made by (Ghost in the Shell)
** Login   <dodeskaden@epita.fr>
**
** Started on  Sat Jan 26 15:16:08 2002 Ghost in the Shell
** Last update Sat Feb  2 10:10:34 2002 Ghost in the Shell
*/

#ifndef GRAPHXSTRUCT_H
#define GRAPHXSTRUCT_H

class SFRect
{
public:
  SFRect() { left = top = right = bottom = 0; }
  SFRect(int l, int t, int r, int b)
  {
    left = l;
    top = t;
    right = r;
    bottom = b;
  }

  SDL_Rect toSDL_Rect()
  {
    SDL_Rect r;
    r.x = Left();
    r.y = Top();
    r.w = Right() - r.x;
    r.h = Bottom() - r.y;
    return r;
  }

  ~SFRect(){};
  inline void setSFRect(int l, int t, int r, int b)
  {
    left = l;
    top = t;
    right = r;
    bottom = b;
  }
  inline int Left() const { return left; }
  inline int Right() const { return right; }
  inline int Top() const { return top; }
  inline int Bottom() const { return bottom; }

private:
  int left, top, right, bottom;
};

struct SFFont
{
  TTF_Font *genre;
};

class SFColor
{
public:
  SFColor() { red = green = blue = 0; };
  SFColor(int r, int g, int b) { setSFColor(r, g, b); };
  ~SFColor(){};
  inline void setSFColor(int r, int g, int b)
  {
    red = r;
    green = g;
    blue = b;
  }
  inline int Red() const { return red; }
  inline int Green() const { return green; }
  inline int Blue() const { return blue; }

private:
  int red, green, blue;
};

#endif
