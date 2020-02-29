//
// Fourmi.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Sun Feb  3 23:50:29 2002 Ghost in the Shell
// Last update Sun Feb  3 23:53:35 2002 Ghost in the Shell
//

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Fourmi.cpp: implémentation de la classe Fourmi
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include  <assert.h> 
#include "SFourmis.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//	Procédures d'actions des fourmis
////////////////////////////////////////////////////////////////////////////////////////////////

Fourmi::Fourmi(int x, int y, type social, int num, Fourmi *reine, CRoom *room) :
CAnimal(x, y, FOURMI, num, room)
{
  // Caractéristiques physiques

  setStatut(social);
  Vie = 200;
  Nourriture = 300 + rand() % 200;
  Taille_mandibule = 50 + rand() % 50;
  Nourriture_dos = 0;
  setLoquace(25 + rand() % 75);
  Depot = 20 + rand() % 10;
  Excitation = 0;
  Taille = rand() % 2;
  Age = 0;
  // Caractéristiques informatiques

  PCiterne = NULL;

  LastPonte = 0;
  setbStop(true);
  setbDesti(false);
  if(reine != NULL)
    Clan = reine->Clan;
  setbUnder(Room == terre ? false : true);
  setbParle(false);
  setbPorteObj(false);
  setbTunnel(false);
  setbCiterne(false);
  Wait = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////

Fourmi::~Fourmi() {}

////////////////////////////////////////////////////////////////////////////////////////////////

int
Fourmi::RegardeAnimalAutour(int Peri, bool cl, enum espece esp)
  // Paramètres:
  // Peri   : périmètres de vision
  // cl		: true = ce clan, sinon tous les autres, par défaut true
  // esp	: espèce par défaut FOURMI

  // Valeurs de retour:
  // >0	:nombre de fourmis autour d'elle
  // dans Autour	:numéro des fourmis dans le périmètre de recherche
{
  class CAnimal *ani;
  int		i = 0;
  
  for (int x = Xc - Peri; x <= Xc + Peri; x++)
    for (int y = Yc - Peri; y <= Yc + Peri; y++)
    {
      if ((x == Xc && y == Yc) || !CASE_VALIDE(x, y, Room))
	continue;
      if ((ani = TEST_ANIMAL(x, y, Room)) != NULL)
      {
	if ((ani->Esp == esp) )
	{
	  if((!cl && ani->Clan != Clan)||(cl && ani->Clan == this->Clan))
	  {
	    Autour[i] = ani;
	    i++;
	  }
	}
      }
    }
  return i; // Nombre d'éléments autour d'elle
}


////////////////////////////////////////////////////////////////////////////////

int
Fourmi::RegardeNourritureAutour(int Peri) const
  // Paramètres:
  // Peri   : périmètres de vision

  // Valeurs de retour:
  //  0	:rien
  // -1	:si Nourriture
  // -2	:si cadavre
{
  for (int x = Xc-Peri; x <= Xc+Peri; x++)
    for (int y = Yc-Peri; y <= Yc+Peri; y++)
    {
      if ((x == Xc && y == Yc) || !CASE_VALIDE(x, y, Room))
	continue;
      if (Room->map[x][y].decor->genre == VIANDE)
	return -1;
      else if (Room->map[x][y].decor->genre == CADAVRE)
	return -2;
    }
  return 0;
}


////////////////////////////////////////////////////////////////////////////////

int
Fourmi::ChangeRoom(class CRoom *rm)
  // Paramètres
  // rm	: room où l'on va

  // Valeurs de sortie
  // 0	: pas de déplacement possible (obstruction à la sortie)
  // 1	: sortie sans problème
{
  int x, y, i, dir;
  for (i = 0; i < 20; i++)
  {
    dir = 2 * (rand() % 8);
    x = Xc + CAnimal::Deplacements[dir % 16]; y = Yc + CAnimal::Deplacements[(dir % 16) + 1];
    if (CASE_VIDE(x, y, rm))
      break;
  }
  if (i == 20)
    return 0;
  Room->map[x][y].firstAnimal = NULL;
  Room = rm;
  SetPos(x, y);
  setbUnder(TEST_UNDER(Room));
  return 1;
}


////////////////////////////////////////////////////////////////////////////////

class Fourmi*
Fourmi::Plus_Proche(enum type defourmi) const
  // Paramètres
  // deFourmi	: catégorie de fourmi à rechercher

  // Valeurs de sortie
  // Pointeur vers la fourmi trouvée,
  // ou NULL si aucune trouvée.
{
  Fourmi *fourmi = NULL, *fourmi_tmp;

  for (int i = 0; i < MData.max_ants; i++)
  {
    fourmi_tmp = Clan->les_fourmis[i];
    if (fourmi_tmp == this
	|| fourmi_tmp->Statut() != defourmi
	|| fourmi_tmp->Room != this->Room)
      continue;
    if (fourmi == NULL || DistancePtr(fourmi_tmp) < DistancePtr(fourmi))
      fourmi = fourmi_tmp;
  }
  return fourmi;
}


////////////////////////////////////////////////////////////////////////////////

enum categorie
Fourmi::DoPhero(enum categorie categ)
  // Cette fonction fait avancer la fourmi vers le phéromone categ
  // Paramètres
  // Categ	:catégorie de phéromone à rechercher

  // Valeurs de sortie
  // ON		:la fourmi est sur le type de phéromone décrit par categ et peut avancer
  //		 dans la bonne direction
  // NUL	:sinon
{
  int x = Xc+Room->map[Xc][Yc].phero[categ]->Vex, y = Yc+Room->map[Xc][Yc].phero[categ]->Vey;

  if (!CASE_VALIDE(x, y, this->Room)) return NUL;

  if (Room->map[x][y].phero[categ]->Genre == NUL) return NUL;
  if (Room->map[Xc][Yc].phero[categ]->Vex 
      == -Room->map[x][y].phero[categ]->Vex
      && Room->map[Xc][Yc].phero[categ]->Vey
      == -Room->map[x][y].phero[categ]->Vey)
    return NUL;
  if (Room->map[x][y].decor != NULL || (x == Oldpos[0][0] && y == Oldpos[0][1]))
    return NUL;

  if ((Xc == Oldpos[1][0] && Yc == Oldpos[1][1]) || (Xc == Oldpos[2][0] && Yc == Oldpos[2][1]))
  {
    Room->map[Xc][Yc].phero[categ]->Vex *= -1;
    Room->map[Xc][Yc].phero[categ]->Vey *= -1;
  }

  Aller_sur(x, y);
  return ON;
}


////////////////////////////////////////////////////////////////////////////////

enum categorie
Fourmi::Phero_autour(enum categorie categ)
  // Cette fonction fait une recherche de phéromone Categ
  // Paramètres
  // Categ		:categorie de phéromone à rechercher

  // Valeurs de sortie
  // ON			:la fourmi est sur le type de phéromone décrit par Categ
  // Categ		:si ce type de phéromone est autour d'elle
  // NUL		:sinon
  // dans Dest	:la position de ces phéromones
{
  int xn = Xc, yn = Yc;

  for (int x = Xc-1; x <= Xc+1; x++)
    for (int y = Yc-1; y <= Yc+1; y++)
    {
      if (!CASE_VALIDE(x, y, Room)) continue;

      if (Room->map[x][y].phero[categ]->Action() != OK) continue;

      if (x == Xc && y == Yc)
	return ON;

      if (Room->map[x][y].phero[categ]->Intensite
	  > Room->map[xn][yn].phero[categ]->Intensite)
      {
	xn = x;
	yn = y;
      }
    }

  if (xn != Xc || yn != Yc)
  {
    SetDest(xn, yn);
    return categ;  // Avance dans la direction des phéromones les plus intenses
  }
  else
    return NUL;
}


////////////////////////////////////////////////////////////////////////////////

int
Fourmi::Lacher_Pheromone(categorie cat)
  // Paramètres
  // Machin	:catégorie des phéromones à lâcher

  // Valeurs de sortie
  // 1		:OK
{
  int x, y, intensit = Excitation+Pherom_N+rand()%30;
  for (x = Xc-1; x <= Xc+1; x++)
    for (y = Yc-1; y <= Yc+1; y++)
    {
      if (!CASE_VALIDE(x, y,Room)) continue;
      if (Room->map[x][y].decor->genre == MUR) continue;

      Room->map[x][y].phero[cat]->NbTour = ZSF.Counter();
      switch (cat)
      {
	case NOURRITURE:
	  if (Oldpos[0][0] == Xc && Oldpos[0][0] == Yc) break;
	  Room->map[x][y].phero[cat]->Genre = NOURRITURE;
	  Room->map[x][y].phero[cat]->Intensite = intensit+rand()%600;
	  Room->map[x][y].phero[cat]->Vex = Oldpos[0][0]-Xc;
	  Room->map[x][y].phero[cat]->Vey = Oldpos[0][1]-Yc;
	  break;
	case PCITERNE:
	  if (Oldpos[0][0] == Xc && Oldpos[0][0] == Yc) break;
	  if(PCiterne == NULL) SDEBUG(W5,"anormal");

	  if (Distance(PCiterne->Xc,PCiterne->Yc) < 8)
	  {
	    Room->map[x][y].phero[cat]->Genre = PCITERNE;
	    Room->map[x][y].phero[cat]->Intensite = 1000+rand()%20;
	    Room->map[x][y].phero[cat]->Vex = GETSIGN_I(PCiterne->Xc-x);
	    Room->map[x][y].phero[cat]->Vey = GETSIGN_I(PCiterne->Yc-y);
	  }
	  else
	  {
	    Room->map[x][y].phero[cat]->Genre = PCITERNE;
	    Room->map[x][y].phero[cat]->Intensite = 1000+rand()%20;
	    Room->map[x][y].phero[cat]->Vex = Oldpos[0][0]-Xc;
	    Room->map[x][y].phero[cat]->Vey = Oldpos[0][1]-Yc;
	  }
	  break;
	case DANGER:
	  Room->map[x][y].phero[cat]->Genre = DANGER;
	  Room->map[x][y].phero[cat]->Intensite = 3000+rand()%10;
	  Room->map[x][y].phero[cat]->Vex = 0;
	  Room->map[x][y].phero[cat]->Vey = -1;
	  break;
	default:
	  break;
      }
    }

  return 1;
}


////////////////////////////////////////////////////////////////////////////////

void
Fourmi::Evolve()
{
  // delete Clan->les_fourmis[Numero];//TODO 2020 - this delete was called from the destroyed object...
  Clan->les_fourmis[Numero] = this;
}


////////////////////////////////////////////////////////////////////////////////

void
Fourmi::Paf(int choc)
  // Paramètres
  // choc	: intensité du coup
{
  Vie -= choc;
  setbDefense(true);
}


////////////////////////////////////////////////////////////////////////////////

void
Fourmi::Attaque(CAnimal *ani, int force) const
  // Paramètres
  // ani	: pointeur vers l'ennemi à frapper
  // force	: force du coup
{
  if (ani == NULL)
    return;
  ani->Paf(force);
}


////////////////////////////////////////////////////////////////////////////////

int
Fourmi::GoNearestCiterne()
  // Valeurs de sortie
  // Retourne valeur renvoyée par Avancer_vers_under,
  // dans Dest position de la citerne
  // Et dans PCiterne.
{
  Fourmi*	cit;

  if (bUnder() == 0)
  {
    cit = Plus_Proche(CITERNE);
    if (cit != NULL)
    {
      SetDestPtr(cit);
      PCiterne = cit;
    }
    if (Distance(Clan->Gate[0][0], Clan->Gate[1][0]) < DistancePtr(cit))
    {										  // si la citerne est + loin que la fourmiliere préfère la fourmilière
      SetDest(Clan->Gate[0][0], Clan->Gate[1][0]);
    }
    return Avancer_vers(Dest[0], Dest[1], 1);
  }
  else
  {
    cit = Plus_Proche(CITERNE);
    if (cit != NULL)
    {
      SetDestPtr(cit);
      PCiterne = cit;
    }
    else
    {
      if (Clan->ReineClan->bUnder() == false)
      {
	SDEBUG(W5,"anormal");
	SetDest(0, 0);
      }
      else
      {
	SetDestPtr(Clan->ReineClan);
      }
    }
    return Avancer_vers(Dest[0], Dest[1], 1);
  }
}


////////////////////////////////////////////////////////////////////////////////

int
Fourmi::EnterTunnel(const class CTunnel *Tunnel)
  // Entre dans un tunnel
  // Paramètres
  // tunnel :pointeur sur le tunnel à traverser

  // Valeur de sortie
  // 1	:en marche
  // bTunnel armé
{
  int		x = -1, y = -1;
  CRoom*	in;
  CRoom*	out;

  Wait = 20;
  in = Tunnel->In;
  out = Tunnel->Out;
  Room->map[Xc][Yc].firstAnimal = NULL;
  Room = (in != Room ? in : out);

  for (unsigned int o = 0; o < Room->Pipe.size(); o++)
  {
    if ((Room->Pipe[o]) == Tunnel)
    {
      x = Room->Gate[0][o];
      y = Room->Gate[1][o];
    }
  }
  setbUnder(TEST_UNDER(Room));
  assert(x != -1 && y != -1);
  Xc = x; Yc = y;
  setbTunnel(true);
  return 1;
}


////////////////////////////////////////////////////////////////////////////////

bool
Fourmi::Serialize(FILE * fichier, bool sauv, CClan *clan, IOTablePointeur &table)
  // Paramètres
  // fichier	: ptr du fichier de sauvegarde/chargement
  // sauv		: si true sauve, si false charge
{

	int i; //Pour les pointeurs room, clan, pCiterne, prev, next
  if (sauv&&fichier!=NULL)									  // En cas de sauvegarde
  {
	/*Pointeurs*/
	i=table.FindOrAdd((void *)this); //Sauve son numero dans la table
    if(!fwrite(&i,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
	i=table.FindOrAdd((void *)this->Room);//Sauve le numero de room dans la table
    if(!fwrite(&i,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
	i=table.FindOrAdd((void*)this->Clan);
    if(!fwrite(&i,sizeof(int),1,fichier)){//Sauve le numero de clan dans la table
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
	i=table.FindOrAdd((void*)this->PCiterne);
    if(!fwrite(&i,sizeof(int),1,fichier)){//Sauve le numero de pCiterne dans la table
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
	i=table.FindOrAdd((void*)this->prev);
    if(!fwrite(&i,sizeof(int),1,fichier)){//Sauve le numero de prev dans la table
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
	i=table.FindOrAdd((void*)this->next);
    if(!fwrite(&i,sizeof(int),1,fichier)){//Sauve le numero de next dans la table
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
	/*Variables simples(classes Animal et Fourmi)*/

    if(!fwrite(&Esp, sizeof(enum espece),1,fichier)){
	 SDEBUG(W4,"Erreur d'écriture");
	 return false;
    }
    i=this->Statut();
    if(!fwrite(&i, sizeof(enum type),1,fichier)){
	 SDEBUG(W4,"Erreur d'écriture");
	 return false;
    }
    if(!fwrite(&Numero,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Xc,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Yc,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Vie,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Nourriture,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Nourriture_dos,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Taille_mandibule,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Depot,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Excitation,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&LastPonte,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Taille,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    i=this->Loquace();
    if(!fwrite(&i,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Age,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }
    if(!fwrite(&Wait,sizeof(int),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }

    //Variables sauvées par boucles
    for (int g = 0; g < 5; g++)
      for (int h = 0; h < 2; h++)
	if(!fwrite(&Oldpos[g][h],sizeof(int),1,fichier)){
	  SDEBUG(W4,"Erreur d'écriture");
	  return false;
	}

    for (int bo = 0; bo < 2; bo++)
      if(!fwrite(&Dest[bo],sizeof(int),1,fichier)){
	SDEBUG(W4,"Erreur d'écriture");
	return false;
      }

    //Variables booléennes
    BYTE octet = bUnder();
    octet |= bParle() << 1;
    octet |= bPorteObj() << 2;
    octet |= bTunnel() << 3;
    octet |= bDesti() << 4;
    octet |= bCiterne() << 5;
    octet |= bStop() <<6;
    octet |= bDefense() <<7;
    if(!fwrite(&octet,sizeof(BYTE),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }


    //Signal de fin de fourmi
    BYTE endFourmi='f';
    if(!fwrite(&endFourmi,sizeof(BYTE),1,fichier)){
      SDEBUG(W4,"Erreur d'écriture");
      return false;
    }

    return true;
  }

  else	//Chargement
  {
  	if(fichier!=NULL){//Lecture du fichier, pas de mise à jour des pointeurs
		if(!fread(&i,sizeof(int),1,fichier)){	//MISE A JOUR DE LA table
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		SDEBUG(W2,"APPEL de SET (fourmi) avec index="<<i<<" valeur "<<this);
		table.set(i, (void *)this);
		/*Lecture des numéros de pointeurs et stockage dans les variables statiques*/
		if(!fread(&this->Room,sizeof(int),1,fichier)){ //room, clan, pCiterne, prev, next ->i,j,k,l,m
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&this->Clan,sizeof(int),1,fichier)){ //room, clan, pCiterne, prev, next ->i,j,k,l,m
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&this->PCiterne,sizeof(int),1,fichier)){ //room, clan, pCiterne, prev, next ->i,j,k,l,m
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&this->prev,sizeof(int),1,fichier)){ //room, clan, pCiterne, prev, next ->i,j,k,l,m
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&this->next,sizeof(int),1,fichier)){ //room, clan, pCiterne, prev, next ->i,j,k,l,m
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		/*Variables simples*/
		if(!fread(&Esp, sizeof(enum espece),1,fichier)){
			SDEBUG(W4,"Erreur d'écriture");
			return false;
		}
		if(!fread(&statut, sizeof(enum type),1,fichier)){
			 SDEBUG(W4,"Erreur d'écriture");
			return false;
		}
		if(!fread(&Numero,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Xc,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Yc,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Vie,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Nourriture,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Nourriture_dos,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Taille_mandibule,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Depot,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Excitation,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&LastPonte,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Taille,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&loquace,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Age,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(!fread(&Wait,sizeof(int),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		//Variables sauvées par boucles
		for (int g = 0; g < 5; g++)
		for (int h = 0; h < 2; h++)
			if(!fread(&Oldpos[g][h],sizeof(int),1,fichier)){
				SDEBUG(W4,"Erreur de lecture");
				return false;
			}
		for (int bo = 0; bo < 2; bo++)
			if(!fread(&Dest[bo],sizeof(int),1,fichier)){
				SDEBUG(W4,"Erreur de lecture");
				return false;
			}

		//Variables booléennes
		BYTE octet = 0;
		if(!fread(&octet,sizeof(BYTE),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		setbUnder(octet&1);
		setbParle((octet&2) >> 1);
		setbPorteObj((octet&4) >> 2);
		setbTunnel((octet&8) >> 3);
		setbDesti((octet&16) >> 4);
		setbCiterne((octet&32) >> 5);
		setbStop((octet&64) >> 6);
		setbDefense((octet&128) >> 7);

		//Vérification de fin de Fourmi
		BYTE endFourmi;
		if(!fread(&endFourmi, sizeof(BYTE),1,fichier)){
			SDEBUG(W4,"Erreur de lecture");
			return false;
		}
		if(endFourmi!='f'){
			SDEBUG(W4,"Fin de fourmi invalide");
			return false;
		}
	}else{//Mise à jour des pointeurs
		this->Room = (CRoom *)table.get(reinterpret_cast<int64_t>(this->Room));
		this->Clan = (CClan *)table.get(reinterpret_cast<int64_t>(this->Clan));
		this->PCiterne = (Fourmi *)table.get(reinterpret_cast<int64_t>(this->PCiterne));
		this->prev = (CAnimal *)table.get(reinterpret_cast<int64_t>(this->prev));
		this->next = (CAnimal *)table.get(reinterpret_cast<int64_t>(this->next));
	}
  }
  return true;
}
