///////////////////////////////////////////////////////////////////////////////
//
// Animal.cpp: implémentation de la classe CAnimal
//
///////////////////////////////////////////////////////////////////////////////

#include "SFourmis.h"

///////////////////////////////////////////////////////////////////////////////

CAnimal::CAnimal(int x, int y, espece espec, int num, CRoom *room)
{
  Room = room;
  Esp = espec;
  Numero = num;
  for (int i = 1; i < 5; i++)
  {
    Oldpos[i][1] = y;
    Oldpos[i][0] = x;
  } // Initialise les anciennes positions
  SetPos(x, y);
  SetDest(0, 0);
  Clan = NULL; // Pour que les cafards n'aient pas un pointeur de valeur indéterminée
}

///////////////////////////////////////////////////////////////////////////////

CAnimal::~CAnimal() {}

///////////////////////////////////////////////////////////////////////////////
void CAnimal::AjouteAnimalXY(int x, int y)
{
  CAnimal *a;

  if ((a = Room->map[x][y].firstAnimal) == NULL)
  {
    Room->map[x][y].firstAnimal = this;
  }
  else
  {
    while (a->next != NULL)
      a = a->next;
    a->next = this;
  }
}
///////////////////////////////////////////////////////////////////////////////
void CAnimal::SetPos(int x, int y)
{
  for (int i = 1; i < 5; i++)
    for (int j = 0; j < 2; j++)
      Oldpos[i][j] = Oldpos[i - 1][j];
  Oldpos[0][0] = Xc;
  Xc = x;
  Oldpos[0][1] = Yc;
  Yc = y;
  Room->map[x][y].firstAnimal = this;
}

///////////////////////////////////////////////////////////////////////////////

int CAnimal::Avancer_vers(int lx, int ly, int dist)
// Paramètres:
// lx		: direction x
// ly		: direction y
// dist	: distance d'approche

// Valeurs de retour:
// 1		: en marche
// 2		: déjà arrivé
{
  int x = Xc + GETSIGN_I(lx - Xc);
  int y = Yc + GETSIGN_I(ly - Yc);

  if (Distance(lx, ly) <= dist)
    return 2; // Dejà arrivée

  if (!CASE_VIDE(x, y, this->Room))
    return Avancer_hasard(); // Chemin bloqué

  Aller_sur(x, y);
  return 1;
}

///////////////////////////////////////////////////////////////////////////////

int CAnimal::Avancer_hasard()
{
  int x, y, i, newdir, dir;
  int vx = Oldpos[0][0] - Xc, vy = Oldpos[0][1] - Yc;

  if (vx == 0 && vy == 0)
    dir = 2 * (rand() % 8);
  else
    for (dir = 0; dir < 16; dir += 2)
      if (Deplacements[dir] == vx && Deplacements[dir + 1] == vy)
        break;

  for (i = 0; i < 20; i++)
  {
    newdir = dir + 2 * ((rand() % 5) + 2);
    if (rand() % 100 < 80)
      newdir = dir + 8;
    x = Xc + Deplacements[newdir % 16];
    y = Yc + Deplacements[newdir % 16 + 1];
    if (CASE_VIDE(x, y, this->Room))
      break;
  }
  if (i == 20)
  {
    x = Xc;
    y = Yc;
  }

  Aller_sur(x, y);
  return 1;
}
///////////////////////////////////////////////////////////////////////////////

CAnimal *
CAnimal::RegardeEnnemi(int Peri)
// Paramètre
// Peri	: distance de vision

// Valeurs de sortie
// NULL si pas d'ennemis en vue,
// sinon pointeur vers l'ennemi
{
  CAnimal *ani;

  for (int x = Xc - Peri; x <= Xc + Peri; x++)
    for (int y = Yc - Peri; y <= Yc + Peri; y++)
    {
      if ((x == Xc && y == Yc) || !CASE_VIDE(x, y, this->Room))
        continue;
      if ((ani = TEST_ANIMAL(x, y, Room)) != NULL)
        if (ani->Esp != this->Esp)
          return ani;
    }
  return NULL; // Si pas d'ennemi en vue
}
///////////////////////////////////////////////////////////////////////////////
void CAnimal::Meurt()
// Tout animal doit un jour connaître sa mort...(faut-il plus détailler l'utilité de cette procédure ?)
{
  Room->map[Xc][Yc].firstAnimal = NULL;
  Room->map[Xc][Yc].decor->genre = CADAVRE;
  Room->map[Xc][Yc].decor->etat = Nourriture + 100; // Un petit bonus de nourriture.
}
