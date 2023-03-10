//
// GraphXproc.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
//
// Started on  Sat Feb  2 10:47:09 2002 Ghost in the Shell
// Last update Sat Oct 19 00:09:00 2002 Ghost in the Shell
//

/////////////////////////////////////////////////////////////////////////////////
//
//	PROCEDURES GRAPHIQUES
//
///////////////////////////////////////////////////////////////////////////////

#include "SFourmis.h"

#include "GraphXstruct.h"
#include "gGraphX.hh"
#include "GraphXproc.h"
#include "GraphXtools.h"

///////////////////////////////////////////////////////////////////////////////
void SelectMultiple() // dessine un rectangle de sélection
{
  SFColor colors;
  SFRect rc;

  rc.setSFRect(min(MouseXY[0], MouseXYD[0]), min(MouseXY[1], MouseXYD[1]), max(MouseXYD[0], MouseXY[0]), max(MouseXYD[1], MouseXY[1]));

  colors.setSFColor(80, 159, 207);

  SFDrawSingleRectangle(rc, colors);
}

///////////////////////////////////////////////////////////////////////////////
void SmallMap() // TODO 2020 remove globals and pass a SFourmiWorld by reference
{
  class SFRect rc(15, 440, 115, 540);
  class SFColor colors;
  int PosX, PosY;
  static SFRect Yellow(0, 825, 1, 826),
      Vert(10, 880, 11, 881),
      Red(78, 880, 79, 881),
      Violet(365, 709, 366, 710);

  colors.setSFColor(255, 255, 255);

  rc.setSFRect(LSMAP * ZSF.column / ZSF.This_room()->size.largeur + XSMAP, HSMAP * ZSF.line / ZSF.This_room()->size.hauteur + YSMAP, LSMAP * (ZSF.column + 20) / ZSF.This_room()->size.largeur + XSMAP, HSMAP * (ZSF.line + 18) / ZSF.This_room()->size.hauteur + YSMAP);
  SFDrawSingleRectangle(rc, colors);

  for (int ceclan = 0; ceclan < MData.nb_clan; ceclan++)
    for (int i = 0; i < MData.max_ants; i++)
      if ((les_clans[ceclan]->les_fourmis[i]->Statut() != NIHIL) && (les_clans[ceclan]->les_fourmis[i]->Room == ZSF.This_room()))
      {
        PosX = LSMAP * les_clans[ceclan]->les_fourmis[i]->Xc / ZSF.This_room()->size.largeur + XSMAP;
        PosY = HSMAP * les_clans[ceclan]->les_fourmis[i]->Yc / ZSF.This_room()->size.hauteur + YSMAP;
        switch (les_clans[ceclan]->les_fourmis[i]->Statut())
        {
        case OUVRIER:
          SFDrawSurface(PosX, PosY, Yellow);
          break;
        case CITERNE:
          SFDrawSurface(PosX, PosY, Red);
          break;
        case SENTINELLE:
          SFDrawSurface(PosX, PosY, Violet);
          break;
        default:
          break;
        }
      }
  // Ennemis :
  for (int i = 0; i < MData.max_ennemies; i++)
    if (les_ennemis[i] != NULL && les_ennemis[i]->Room == ZSF.This_room())
    {
      PosX = LSMAP * les_ennemis[i]->Xc / MData.length + XSMAP;
      PosY = HSMAP * les_ennemis[i]->Yc / MData.height + YSMAP;
      switch (les_ennemis[i]->Esp)
      {
      case CAFARD:
        SFDrawSurface(PosX, PosY, Vert);
        break;
      default:
        SFDrawSurface(PosX, PosY, Vert);
        break;
      }
    }
}

/**
 * Flip screen buffer
 */
void Flips(void)
{
  // SDL_Flip(screen); //Affiche reellement en echangeant les buffers hardware
  SDL_RenderPresent(sdlRenderer);
}
