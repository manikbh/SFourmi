//
// Sentinelle.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Sun Feb  3 23:57:35 2002 Ghost in the Shell
// Last update Sun Feb  3 23:57:59 2002 Ghost in the Shell
//

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Sentinelle.cpp: implémentation de la classe Sentinelle
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include "SFourmis.h"

////////////////////////////////////////////////////////////////////////////////////////////////

Sentinelle::Sentinelle(int x, int y, int num, Fourmi *reine, CRoom *room) :
Fourmi(x, y, SENTINELLE, num, reine,room)
{
  Centre[0] = Clan->Gate[0][0];
  Centre[1] = Clan->Gate[1][0];
  Peri = Clan->DistAuto;
  Wait = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////

Sentinelle::~Sentinelle() { }

////////////////////////////////////////////////////////////////////////////////////////////////
ordre Sentinelle::Action()
  // Valeurs de sortie
  // DEAD	:plus un souffle de Vie
  // OK	:c'est je peux encore me battre
{
  int		 c;
  CAnimal 	*animal;
  
  if (Vie <= 0)
    return DEAD;

  memset(Autour, 0, 48*sizeof(CAnimal*));
  if (bParle() == true || bDesti() == true)						  // Passe son tour car elle communique
  {
    if (bDesti() == true)
    {
      Avancer_vers(Dest[0], Dest[1], 0);
      setbDesti(false);
    }
    setbParle(false);
    return Vie <= 0 ? DEAD : OK;
  }
  if (Wait > 0 && bTunnel() == true)
  {
    Wait--;
    if (Wait <= 0)
    {
      if (bTunnel() == true)
      {
	if (!ChangeRoom(Room)) { Wait += 10; return OK; }
	setbTunnel(false);
      }
    }
    else
      return OK;
  }
  Age++;
  Vie -= (Age/(VIEILLE+rand()%15000));		  // Si elle est vieille

  ///////////////////// ELLE MANGE ///////////////////////
  //Nourriture--;
  if (Nourriture < 1)
  {
    Vie -= 20;								  //Crève de faim !
    if (Vie <= 0)
      return DEAD;
  }

  // FUZZIFICATION DES VARIABLES : passage aux variables floues
  BYTE faim = 0;								  // Pourcentages
  BYTE danger = 0;
  BYTE utile = 9;
  BYTE interet = 0;
  BYTE publique =0;

  if (Nourriture < 50)						  // Sur un total de 100
  {
    if (Nourriture < 20)
      faim = 100;							  // Très faim !
    else
      faim = (BYTE)3.3*(50-Nourriture);	  // Une fonction trapèze !
  }
  else
    faim=2;
  if (bDefense() == true)
  {
    danger = 50;
    Lacher_Pheromone(DANGER);
    setbDefense(false);
  }
  else
  {
    if (Phero_autour(DANGER) == ON)
      danger = 50;
  }

  danger += 10 * CountEnnemi();

  BYTE VK[5] = {faim, danger, utile, interet, publique};
  BYTE action = Max(VK);
  Avancer_hasard();

  if (bUnder() == false)
  {
    switch(action)
    {
      case 1:								  // DANGER
	SDEBUG(W2,"S1");
	animal = RegardeEnnemi(1);
	if (animal != NULL)
	{
	  Attaque(animal, Taille_mandibule);
	}
	if (Phero_autour(DANGER) == ON)
	{
	  Centre[0] = Xc;
	  Centre[1] = Yc;
	  Peri = 5;
	  Wait = 1000;
	}

	Avancer_hasard();
	break;
      case 2:								  // UTILE
	SDEBUG(W2,"S2");
	Avancer_hasard();
	Wait--;
	if (Wait < 0)
	{
	  Centre[0] = Clan->Gate[0][0];
	  Centre[1] = Clan->Gate[1][0];
	  Wait = 0;
	  Peri = Clan->DistAuto;
	}
	if (Distance(Centre[0], Centre[1]) > Peri)
	{
	  SetDest(Centre[0], Centre[1]);
	  setbDesti(true);
	}
	if (ZSF.bViolent)
	{
	  c = RegardeAnimalAutour(2, false);
	  if (c > 0 )
	  {
	    Attaque(Autour[(c == 1 ? 0 : rand()%(c-1))], Taille_mandibule);
	    break;
	  }

	}
	break;
    }
  }
  else										  // Sous_terre : chercher la sortie
    if (Avancer_vers(Room->Gate[0][0], Room->Gate[1][0], 2) == 2)
      EnterTunnel(Room->GetTunnel(Room->Gate[0][0], Room->Gate[1][0]));

  return Vie <= 0 ? DEAD : OK;
}


////////////////////////////////////////////////////////////////////////////////////////////////

int Sentinelle::CountEnnemi()
{
  int x, y, c = 0;

  for (x = Xc-1; x <= Xc+1; x++)
    for (y = Yc-1; y <= Yc+1; y++)
    {
      if ((x == Xc && y == Yc) || !CASE_VALIDE(x, y, Room)) continue;
      if (TEST_ANIMAL(x, y, Room) != NULL )
	// Pour ajouter d'autres ennemis faire des ou ou bien !=0 (car 0==FOURMI dans esp)
	if (Room->map[x][y].firstAnimal->Esp == CAFARD) c++;
    }
  return c;
}
