//
// Ouvrier.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Sun Feb  3 23:54:36 2002 Ghost in the Shell
// Last update Mon Nov 24 17:06:26 2003 Ghost in the Shell
//

#include "SFourmis.h"

///////////////////////////////////////////////////////////////////////////////

Ouvrier::Ouvrier(int x, int y, int num, Fourmi *reine, CRoom *room):Fourmi(x,y,OUVRIER,num,reine,room) {}

///////////////////////////////////////////////////////////////////////////////

Ouvrier::~Ouvrier() {}

///////////////////////////////////////////////////////////////////////////////

enum ordre Ouvrier::Action()
  // Valeurs de sortie
  // DEAD	:plus un souffle de Vie
  // OK		:c'est je peux encore travailler
{
  enum nature	what;
  int		c, i;
  
  if (Vie <= 0)
    return DEAD;
  Age++;
  if (ZSF.Counter() % 10 == 0)
  {
    Nourriture -= rand()%2;
    Vie -= (Age/(VIEILLE+rand()%60000));	  // Si elle est vieille
  }

  if (Wait > 0)								  // Test des actions commencées et qui prennent du temps
  {
    Wait--;
    if (Wait <= 0)
    {
      if (bParle() == true)
	setbParle(false);
      if (bTunnel() == true)
      {
	if (!ChangeRoom(Room)) { Wait += 10; return OK; }
	setbTunnel(false);
      }
    }
    else
      return OK;
  }
  if (Nourriture < 1)
    Vie -= 1;								  //Crève de faim !
  if (Vie <= 0)
    return DEAD;

  memset(Autour, 0, 48);

  // FUZZIFICATION DES VARIABLES : passage aux variables floues

  BYTE faim = 0;								  // Pourcentages
  BYTE danger = 0;
  BYTE utile = 0;
  BYTE interet = 40;							  // Travaux d'intérêt commun
  BYTE publique = 100;						  // Creuser et fortifier la demeure de la reine

  if (bCiterne() == false)
    interet = 100;				  // Chercher une citernes ou qqchose comme ça

  if (ZSF.Counter() == 50 && bCiterne() == false)
  {
    setbCiterne(true);
    //!!		SetCiternePos(Clan->Gate[0][0], Clan->Gate[1][0]); // Entrée 0 par défaut
    // Cela implique de revoir quand PCiterne == NULL
  }

  if (Nourriture < 120)
  {
    if (Nourriture < 50)
      faim = 100;							  // Très faim !
    else
      faim = (BYTE)1.6*(120-Nourriture);	  // Une fonction trapèze !
  }

  if (bDefense() == true)	  // Défense
  {
    setbDefense(false);
    Lacher_Pheromone(DANGER);
    danger = 90;
  }
  if (Phero_autour(DANGER) == ON)
    danger += 12;
  if (bPorteObj() == true)
    utile = 80;		  // Ramène objet

  BYTE		VK[5] = {faim, danger, utile, interet, 0};
  BYTE		action = Max(VK);
  int		resp;
  CAnimal	*ani;
  Fourmi	*fourmi;
  bool		bOffre = false;
  categorie	mone;

  if (bUnder() == false)							  //////////////////////// ACTION SUR LA TERRE ///////////////////////////
  {
    switch (action)
    {
      case 0:								  // FAIM
	SDEBUG(W2,"O0");
	resp = RegardeAnimalAutour(2);
	if (resp > 0 && faim > 75)		  // S'il y a du monde autour et qu'elle a très faim
	  for (i = 0; i < resp; i++)
	  {
	    switch (static_cast<Fourmi *>(Autour[i])->Statut())
	    {
	      case OUVRIER:
		if (static_cast<Fourmi *>(Autour[i])->Repondre() == OFFRE)
		{
		  bOffre = true;
		  setbParle(true);
		  Nourriture += Autour[i]->Nourriture/5;
		  static_cast<Fourmi *>(Autour[i])->Nourriture *= 4/5;
		}
		break;
	    }
	    if (bOffre == true) break;
	  }
	break;
      case 1:								  // DANGER
	SDEBUG(W2,"O1");
	ani = RegardeEnnemi(1);
	if (ani != NULL)
	  Attaque(ani, Taille_mandibule/2);
	else
	  Avancer_hasard();
	break;
      case 2:								  // UTILE
	SDEBUG(W2,"O2");
	SetDest(Clan->Gate[0][0], Clan->Gate[1][0]);
	if (Avancer_vers(Dest[0], Dest[1],1) == 2)
	{
	  EnterTunnel(Clan->GetTunnel(Clan->Gate[0][0], Clan->Gate[1][0]));
	  break;
	}
	// *Attention* cette partie fait que les fourmis suivent les phéromones maison donc pas totalement inutile !!
	// Ce devrait d'ailleurs être la réaction première de la fourmi qui porte de la bouffe !!!!
	/*			else
				{
				mone = Phero_autour(PCITERNE);			//Regarde phéromones
				if (mone == ON)
				mone = DoPhero(PCITERNE);
				else if (mone == PCITERNE)
				Avancer_vers(Dest[0],Dest[1],0);
				if (mone == NUL) Avancer_hasard();
				}
				if (Nourriture_dos <= Taille_mandibule)		// Si elle peut en porter plus
				{
				if (RegardeNourritureAutour(2) == -1)
				Prend_Nourriture();
				}
	 */
	Lacher_Pheromone(NOURRITURE);
	break;
      case 3:								  // Interet
	SDEBUG(W2,"O3");
	//				Lacher_Pheromone(PCITERNE);
	resp = RegardeNourritureAutour(2);
	if (resp == -1 || resp == -2)	  // y'a de la bouffe
	{
	  c = 0;						  // Plus elle voit de bouffe autour, plus elle est excitée
	  for (int x = Xc-2; x <= Xc+2; x++)
	    for (int y = Yc-2; y<= Yc+2; y++)
	    {
	      if (!CASE_VALIDE(x, y,Room)) continue;

	      what = Room->map[x][y].decor->genre;
	      if (what == VIANDE)
		c += Room->map[x][y].decor->etat;
	      else if (what == CADAVRE)
	      {
		Lacher_Pheromone(DANGER);
	      }
	    }
	  Excitation = c * Depot / MData.max_meat / 2;
	  Prend_Nourriture();
	}
	else							  // S'il n'y a pas de bouffe, se rattrape sur les phéromones
	{
	  mone = Phero_autour(NOURRITURE);
	  if (mone == ON)
	    mone = DoPhero(NOURRITURE);
//	  else if (mone == NOURRITURE)
//	    Avancer_vers(Dest[0], Dest[1], 0);
//	  if (mone == NUL)
	    Avancer_hasard();
	}
	break;
	/*	case 5:
		if (Avancer_vers(la_fourme[0]->entree_fourmi[0], la_fourme[0]->entree_fourmi[1]) == 2)
		{
		Descend_niveau(1,1);
		}
		break;*/
    }
  }

  else										  //////////////////////// ACTION SOUS LA TERRE ///////////////////////////
  {
    switch (action)
    {
      case 0:								  // FAIM
	SDEBUG(W2,"Os0");
	fourmi = Plus_Proche(CITERNE);
	//ligne à maj
	// S'il n'y a pas de citernes, se rabat sur la reine
	if (fourmi == NULL) fourmi = Clan->ReineClan;
	// Si elle est à côté
	if (Distance(fourmi->Xc, fourmi->Yc) <= 2)
	{
	  if (fourmi->Repondre() == OFFRE)
	  {
	    bOffre = true;
	    setbParle(true);
	    c = min(fourmi->Nourriture, 400-Nourriture);
	    Nourriture += c;
	    fourmi->Nourriture -= c;
	  }
	}
	//ligne bizarre
	if (!bOffre)
	  Avancer_vers(fourmi->Xc, fourmi->Yc, 1);

	break;
      case 2:								  // UTILE
	SDEBUG(W2,"Os2");
	resp = GoNearestCiterne();
	//ligne à maj
	if (PCiterne == NULL)
	{
	  PCiterne = Clan->ReineClan;	  // S'il n'y a pas de citernes, se rabat sur la reine
	  SDEBUG(W5,"anormal");
	}
	if (resp == 2)
	{
	  // Si elle est à côté
	  if (Distance(PCiterne->Xc, PCiterne->Yc) <= 2)
	  {
	    setbPorteObj(false);
	    PCiterne->Nourriture += Nourriture_dos;
	    Nourriture_dos = 0;
	  }
	}
	break;

      case 3:								  //Intérêt : Rien de précis, va chercher bouffe DEHORS
	SDEBUG(W2,"Os3");
	resp = Avancer_vers(Room->Gate[0][0], Room->Gate[1][0],2);
	if (resp == 2)
	  EnterTunnel(Room->GetTunnel(Room->Gate[0][0], Room->Gate[1][0]));
	break;
      case 4:								  // Jamais atteint...
	//Faire_Salle(Salle)
	break;
    }
  }

  return Vie <= 0 ? DEAD : OK;
}


////////////////////////////////////////////////////////////////////////////////////////////////

int Ouvrier::Prend_Nourriture()
  // Valeurs de sortie
  // 0	:Ok
  // 1	:Ouf ! trop de Nourriture déjà !
{
  nature what;
  int x, y, k;

  for (x = Xc-2; x <= Xc+2; x++)
    for (y = Yc-2; y <= Yc+2; y++)
    {
      if (!CASE_VALIDE(x, y,Room)) continue;

      what = Room->map[x][y].decor->genre;
      if (what == VIANDE || what == CADAVRE)
      {
	if (Nourriture < 200)
	{
	  k = min(Room->map[x][y].decor->etat, 250-Nourriture);
	  Nourriture += k;
	  Room->map[x][y].decor->etat -= k;
	}
	// Prend le maximum qu'elle peut porter
	k = min(Room->map[x][y].decor->etat, Taille_mandibule-Nourriture_dos);
	Room->map[x][y].decor->etat -= k;
	if (Room->map[x][y].decor->etat <= 0)
	  Room->map[x][y].decor->genre = ERIEN;
	Nourriture_dos += k;
	setbPorteObj(true);
	if (Nourriture_dos >= Taille_mandibule)
	  return 1;
      }
    }
  return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////

comm Ouvrier::Repondre()
  // Valeurs de sortie
  // OFFRE	:on accepte de donner
  // NICHTS	:tu peux toujours courir
{
  if (Nourriture > 250)
  {
    setbParle(true);
    return OFFRE;
  }
  return NICHTS;
}
