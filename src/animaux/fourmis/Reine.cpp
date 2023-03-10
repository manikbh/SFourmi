//
// Reine.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
//
// Started on  Sun Feb  3 23:56:35 2002 Ghost in the Shell
// Last update Sun Feb  3 23:57:19 2002 Ghost in the Shell
//

#include "SFourmis.h"

///////////////////////////////////////////////////////////////////////////////

Reine::Reine(int x, int y, int num, CClan *clan, CRoom *room) : Fourmi(x, y, REINE, num, NULL, room)
{
  Clan = clan;
  Clan->ReineClan = this;
}

//////////////////////////////////////////////////////////////////////////////

Reine::~Reine() {}

/////////////////////////////////////////////////////////////////////////////

ordre Reine::Action()
// Valeurs de sortie
// DEAD	:plus un souffle de Vie
// OK		:c'est je peux encore pondre
{
  bool offre = false;
  int resp, c, o, x, y;
  Fourmi *fourmi;

  if (Vie <= 0)
    return DEAD;
  Nourriture = MAX_I(0, Nourriture - rand() % 2);

  if (bParle() == true)
  {
    setbParle(false);
    return OK;
  }

  if (Nourriture < 10000)
  {
    memset(Autour, 0, 48 * sizeof(CAnimal *));
    resp = RegardeAnimalAutour(2);
    for (o = 0; o < resp; o++)
    {
      switch (static_cast<Fourmi *>(Autour[o])->Statut())
      {
      case CITERNE:
        if (static_cast<Fourmi *>(Autour[o])->Repondre() == OFFRE)
        {
          offre = true;
          setbParle(true);
          c = static_cast<Fourmi *>(Autour[o])->Nourriture / 5;
          Nourriture += c;
          static_cast<Fourmi *>(Autour[o])->Nourriture -= c;
        }
        break;
      default:
        break;
      }
      if (offre == true)
        break;
    }
  }
  fourmi = Plus_Proche(CITERNE);
  if (Nourriture < 4000 && fourmi != NULL)
  {
    if (Distance(fourmi->Xc, fourmi->Yc) <= 2) // Si elle est à côté d'une citerne
    {
      if (fourmi->Repondre() == OFFRE)
      {
        offre = true;
        setbParle(true);
        c = MIN_I(fourmi->Nourriture, 400);
        Nourriture += c;
        fourmi->Nourriture -= c;
      }
    }
    if (!offre && fourmi != NULL)
      Avancer_vers(fourmi->Xc, fourmi->Yc, 1);
  }
  // ligne bizarre (en effet qui est PCiterne pour la reine ?
  //	else
  //		Avancer_vers_under(PCiterne->Xc, PCiterne->Yc, 5);

  if (Clan->Population < MData.max_ants - 2 && Nourriture > 2000 && LastPonte > 100)
  { // Assez de place dans le tableau ? et de Nourriture ? et de temps de repos ?
    for (x = Xc - 1; x <= Xc + 1; x++)
      for (y = Yc - 1; y <= Yc + 1; y++)
      {
        if ((x == Xc && y == Yc) || !CASE_VALIDE(x, y, Room))
          continue;

        if (TEST_ANIMAL(x, y, Room) == NULL) // Ponte
        {
          o = 0;
          while (Clan->les_fourmis[o]->Statut() != NIHIL)
            o++;

          delete Clan->les_fourmis[o];
          Clan->les_fourmis[o] = new Larve(x, y, Numero, o, this, this->Room);
          Nourriture -= 200;
          Clan->Population++;
          LastPonte = 0;
          goto fin;
        }
      }
  }

fin:
  LastPonte++;
  return OK;
}
