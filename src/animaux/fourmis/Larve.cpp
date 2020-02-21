//
// Larve.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Sun Feb  3 23:56:18 2002 Ghost in the Shell
// Last update Sun Feb  3 23:56:25 2002 Ghost in the Shell
//

#include "SFourmis.h"

////////////////////////////////////////////////////////////////////////////////

// Numéro de la reine qui l'engendre et numéro de la larve
Larve::Larve(int x, int y, int nreine, int num, Fourmi *reine,CRoom *room):Fourmi(x,y,LARVE,num,reine,room)
{
  Vie = 100;									  // Elles peuvent rester pas mal de temps sans manger
  Nourriture = 400;
  Nourriture_dos = 0;
  Age = 0;

  if (rand() % 100 > 95 && nreine != -1)		  // Si nreine = -1 : chargement (on ne connait pas son n°)
    // Pour le moment elles héritent de la reine à +-10 près
    Taille_mandibule = Clan->ReineClan->Taille_mandibule+rand()%20-10;
  else
    Taille_mandibule = 50+rand()%50;

  if (rand()%100 > 90 && nreine != -1)
    setLoquace(Clan->ReineClan->Loquace() + rand() % 20 - 10);
  else
    setLoquace(25 + rand() % 75);

  if (rand()%100 > 95 && nreine != -1)
    Depot = Clan->ReineClan->Depot+rand()%10-5;
  else
    Depot = 20+rand()%10;
  LastPonte = MData.ants_reborn;						  // 1200 tours avant de voir une petite fourmi, fréquence de reproduction
  if (nreine != -1)
    setbUnder(reine->bUnder());
}


////////////////////////////////////////////////////////////////////////////////

Larve::~Larve() {}

////////////////////////////////////////////////////////////////////////////////

ordre Larve::Action()
  // Valeurs de sortie
  // DEAD	:plus un souffle de Vie
  // OK	:c'est je peux encore travailler
{
  if (Vie <= 0)
    return DEAD;
  if (ZSF.Counter() % 4 == 0)
    Nourriture -= rand()%3;

  LastPonte--;
  if (LastPonte <= 0)							  // Il est temps de se métamorphoser
  {
    Fourmi *pF;
    BYTE choix = rand()%100;

    //Type de la nouvelle fourmi
    if (Clan->NbOuvrier < 4*Clan->NbSentinelle) Clan->POUVRIER = 80;
    else Clan->POUVRIER = 25;

    if (choix < Clan->POUVRIER)
    {
      if (choix < 14)
      {
	pF = new Citerne(Xc, Yc, Numero, Clan->ReineClan,this->Room);
	Clan->NbCiterne++;
	Clan->Citernes_under++;
	pF->Nourriture = 1000;
      }
      else
      {
	pF = new Ouvrier(Xc, Yc, Numero, Clan->ReineClan,this->Room);
	Clan->NbOuvrier++;
	pF->setbCiterne(true);
	Fourmi *m = pF->Plus_Proche(CITERNE);
	//pF->SetCiternePos(m->Xc, m->Yc);
	if (m != NULL) PCiterne = Clan->ReineClan;
      }
    }
    else
    {
      pF = new Sentinelle(Xc, Yc, Numero, Clan->ReineClan,this->Room);
      Clan->NbSentinelle++;
    }
    pF->setbUnder(this->bUnder());
    pF->Room = Room;
    pF->Age = Age;
    pF->Taille_mandibule = Taille_mandibule;
    pF->setLoquace(Loquace());
    pF->Nourriture= Nourriture;
    pF->Evolve();
  }
  return Nourriture < 0 ? DEAD : OK;
}
