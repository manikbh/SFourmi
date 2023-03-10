//
// Pheromone.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
//
// Started on  Mon Feb  4 12:29:15 2002 Ghost in the Shell
// Last update Mon Feb  4 12:35:44 2002 Ghost in the Shell
//

#include "SFourmis.h"

CPheromone::CPheromone(categorie categ, int intense, int vectX, int vectY)
{
  Genre = categ;
  Vex = vectX;
  Vey = vectY;
  Intensite = intense;
  NbTour = ZSF.Counter();
}

CPheromone::~CPheromone() {}

int CPheromone::Interaction(int num)
{
  //  intense += num;
  //  if (intense > PheroMax)
  //    intense = PheroMax;
  return 0;
}

enum ordre CPheromone::Action()
{
  Intensite -= (Volutabily_meat + Genre * Volutabily_danger) * (ZSF.Counter() - NbTour);

  NbTour = ZSF.Counter();

  if (Intensite <= 0)
  {
    Genre = NUL;
    return DEAD;
  }
  else
    return OK;
}

void CPheromone::Serialize(FILE *fichier, bool sauv, IOTablePointeur &table)
{
  if (sauv)
  {
    char vx = Vex, vy = Vey;
    fwrite(&vx, sizeof(char), 1, fichier);
    fwrite(&vy, sizeof(char), 1, fichier);
    Action(); // Mise é jour
    fwrite(&Intensite, sizeof(int), 1, fichier);
    fwrite(&Genre, sizeof(char), 1, fichier);
  }
  else if (fichier != NULL)
  {
    char vx, vy, gen;
    fread(&vx, sizeof(char), 1, fichier);
    fread(&vy, sizeof(char), 1, fichier);
    fread(&Intensite, sizeof(int), 1, fichier);
    fread(&gen, sizeof(char), 1, fichier);
    NbTour = ZSF.Counter();
    Vex = vx;
    Vey = vy;
    Genre = static_cast<categorie>(gen);
  }
}
