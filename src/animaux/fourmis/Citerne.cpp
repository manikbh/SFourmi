//
// Citerne.cpp : Class citerne
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Sun Feb  3 23:48:42 2002 Ghost in the Shell
// Last update Sun Feb  3 23:49:02 2002 Ghost in the Shell
//

#include "SFourmis.h"

////////////////////////////////////////////////////////////////////////////////

Citerne::Citerne(int x, int y, int num, Fourmi *reine,CRoom *room):Fourmi(x, y, CITERNE, num, reine,room)
{
  Nourriture += 1000;
  Wait = 300 + rand() % 300;
}


////////////////////////////////////////////////////////////////////////////////

Citerne::~Citerne() {}

////////////////////////////////////////////////////////////////////////////////

enum ordre
Citerne::Action()
{
  Fourmi	*citerne;
  int		c;
  
  if (Vie <= 0)
    return DEAD;

  memset(Autour, 0, 48);
  Age++;
  Vie -= (Age/(VIEILLE + rand() % 15000));		  // Si elle est vieille
  if (bParle() == true)
    setbParle(false);
  if (Wait > 0 && bTunnel() == true)
  {
    Wait--;
    if (Wait <= 0)
    {
      if (bTunnel() == true)
      {
	if (ChangeRoom(Room) == 0)
	{
	  Wait += 10;
	  return OK;
	}
	else
	{
	  Clan->Citernes_under--;
	  setbStop(false);
	  Wait = 3000+rand()%3000;
	  setbTunnel(false);
	}
      }
    }
    else
      return OK;
  }

  if (ZSF.Counter() % 2 == 0)
  {
    BYTE faim = 0;							  // Pourcentages
    BYTE danger = 0;
    BYTE utile = 0;
    BYTE interet = 40;						  // Travaux d'intérêt commun : recherche de la bouffe pour les ouvrières,
    // ou pour les citernes déplacements et siège,
    BYTE publique = 100;  // creuser et fortifier la demeure de la reine.

    if (Nourriture < 400)
      faim = 100;
    else
      faim = 2;

    //		if (bPorte_objet == true) utile = 80;	// Ramène objet

    BYTE VK[5] = {faim, danger, utile, interet, 0};
    BYTE action = Max(VK);
    bool Offre = false;

    if (bUnder() == false)							  //////////////////////// ACTION SUR LA TERRE ///////////////////////////
    {
      switch (action)
      {
	case 0:							  // Faim
	  SDEBUG(W2,"C0");
	  citerne = Plus_Proche(CITERNE);
	  if (GoNearestCiterne() == 2)  //PCD  && !bCiterneExt)
	  {}
	  if (Offre)
	    setbStop(false);
	  break;

	case 3:							  // Intérêt
	  SDEBUG(W2,"C3");
	  if (Wait-- <= 0)			  // Temps avant de revenir à la fourmilière
	  {
	    SetDest(Clan->Gate[0][0], Clan->Gate[1][0]);
	    if (Avancer_vers(Dest[0], Dest[1],1) == 2)
	    {
	      if (EnterTunnel((Clan->GetTunnel(Dest[0], Dest[1]))) == 1)
		Clan->Citernes_under++;
	    }
	  }
	  else if (bStop() == false)
	  {
	    Avancer_hasard();
	    if (Distance(Clan->Gate[0][0], Clan->Gate[1][0]) > Clan->DistAuto/2)
	      setbStop(true);
	  }
	  break;
      }
    }
    else								 //////////////////////// ACTION SOUS LA TERRE ///////////////////////////
    {
      switch(action)
      {
	case 0:							  // DANGER ?
	  SDEBUG(W2,"Cs0");

	  if (RegardeNourritureAutour(2) == -1)
	    Prend_Nourriture();
	  else
	  {
	    // Plante si pas au moins 2 citernes dans ssol
	    citerne = Plus_Proche(CITERNE);
	    if (citerne == NULL)
	    {
	      Avancer_hasard();
	      break;
	    }
	    if (Avancer_vers(citerne->Xc, citerne->Yc, 1) == 2)
	    {
	      if (citerne->Repondre() == OFFRE)
	      {
		Offre = true;
		setbParle(true);
		c = citerne->Nourriture/2;
		Nourriture += c;
		citerne->Nourriture -= c;
	      }
	    }
	  }
	  break;

	case 3:							  // Ca arrive ?
	  SDEBUG(W2,"Cs3");
	  // La reine manque de Nourriture ! Aidons-la !
	  if (Clan->ReineClan->Nourriture < 10000)
	  {
	    SetDest(Clan->ReineClan->Xc, Clan->ReineClan->Yc);
	    Avancer_vers(Dest[0], Dest[1], 2);
	  }
	  else if (Nourriture > 20000)  // Ce qui suit sert au stockage de Nourriture -> nécessite une meilleure intelligence -> petits paquets
	  {
	    int GoX = 5, GoY = 5;
	    if (Room->map[GoX+1][GoY+1].decor->etat > 200000)
	    {
	      GoX = min(5+rand()%5,F_LARGEUR-2);
	      GoY = min(5+rand()%5,F_HAUTEUR-2);
	    }
	    SetDest(GoX, GoY);
	    if (Avancer_vers(Dest[0], Dest[1], 1) == 2
		&& TEST_ANIMAL(GoX+1,GoY+1,Room) == NULL)
	    {     //A Reconsidérer lorsque extérieur == Room dans ce cas la_fourme[0] == Room)
	      Room->map[GoX+1][GoY+1].decor->genre = VIANDE;
	      Room->map[GoX+1][GoY+1].decor->etat += 15000;
	      Nourriture -= 15000;
	      setbParle(true);
	    }
	  }
	  else if (ZSF.Counter() % 8 == 0)
	    Avancer_hasard();
	  break;
      }

    }
  }				  // fin counter%2 == 0
  return Vie <= 0 ? DEAD : OK;
}


////////////////////////////////////////////////////////////////////////////////

enum comm
Citerne::Repondre()
{
  if (Nourriture > 120 && bParle() == false)
  {
    setbParle(true);
    return OFFRE;
  }
  return NICHTS;
}

