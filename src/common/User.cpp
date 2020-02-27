// Class User contains all the informations related to the user.
// It initializes the user-dependent flags and sets up the map
// according to the DataMap infos.

#include <iostream>
#include "SFourmis.h"

#ifdef GTK_Linux
# include <gtk/gtk.h>
# include <gdk/gdk.h>
# include <gdk-pixbuf/gdk-pixbuf.h>
# include "GTKInterface.h"
#endif

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "Tunnel.h"
#include "Room.h"
#include "DataMap.h"
#include "User.h"

#define INITIAL_QUEENS_MEAT	30000

User::User()
{
  bInfo = true,
  bInfo_enn = false;
  bPause = true;
  ButtonMeat = false;
  bDown = false;
  bMove = false;
  bSmall_map = true;		// F4 mini carte
  bStats = true;		// 'S' Statistiques
  bScreen = true;		// 'T' hyperespace
  bViolent = true;		// 'V' Violence
  bGoLeft = false;
  bGoRight = false;
  bGoUp = false;
  bGoDown = false;
  column = 0;
  line = 0;
  counter = 0;
  this_clan=NULL;
}

User::~User()
{}

void
User::Initialize(class DataMap &MData)
{
  ConstructEnvironment(MData);
}

void
User::ConstructEnvironment(class DataMap &MData)
{
  srand(time(NULL));

  /// TERRE ///
  SDEBUG(W0,"terre");
  terre = new CRoom(MData.length, MData.height, NORMAL_R);
  CheckWall(MData);

  /// CLANS ///
  SDEBUG(W0,"->clans<-");
  les_clans = new CClan*[MData.nb_clan];
  Init_Clan(MData);
  //Une valeur dans le pointeur
  this->setThis_clan(les_clans[0]);
  Init_Ants(MData);
  setThis_animal(les_clans[0]->les_fourmis[0]);

  terre->map[0][0].firstAnimal = NULL;

  Init_Ennemies(MData);
  Init_Landscape(MData);
}

void
User::CheckWall(class DataMap &MData)
{
  int		ci = 1;
  int		sum = 0;
  
  for (int i = 0; i < MData.length; i++)
    for (int j = 0; j < MData.height; j++)
      if (terre->map[i][j].decor->genre == MUR)
      {
	ci = 1;
	sum = 0;
	for (int p = -1; p <= 1; p++)
	  for (int o = -1; o <= 1; o++)
	    if (((p+o-1)%2==0))			  //si p+o-1 div par 2
	    {
	      if (CASE_VALIDE(i+p,j+o, terre))
		if (terre->map[i+p][j+o].decor->genre==MUR) 
		  sum+=ci;
	      ci *= 2;
	    }
	switch (sum)
	{
	  case (1+2):terre->map[i][j].terrain = 18;
		break;
	  case (1+4):terre->map[i][j].terrain = 12;
		break;
	  case (2+8):terre->map[i][j].terrain = 16;
		break;
	  case (4+8):terre->map[i][j].terrain = 10;
		break;
	  case (1+2+4):terre->map[i][j].terrain = 15;
		break;
	  case (2+4+8):terre->map[i][j].terrain = 13;
		break;
	  case (1+2+8):terre->map[i][j].terrain = 17;
		break;
	  case (1+4+8):terre->map[i][j].terrain = 11;
		break;
	  case (1+2+4+8):terre->map[i][j].terrain = 14;
		break;
	}
      }
}

void
User::Init_Clan(class DataMap &MData)
{
  CRoom		*tmpRoom;
  CTunnel	*firsttunnel;
  int		i, j;

  for (int k = 0; k < MData.nb_clan; ++k)
  {
    les_clans[k] = new CClan;
    les_clans[k]->la_fourme = new CRoom*[1];	  //1 salle d'abord
    les_clans[k]->la_fourme[0] = new CRoom(F_LARGEUR, F_HAUTEUR, SOUSSOL_R);
    tmpRoom = les_clans[k]->la_fourme[0];
    les_clans[k]->NBMAX_ROOM = 0;	  // 1 seul niveau au début
    for(i = 0, j = 0;;)
    {
      i = rand() % MData.length;
      j = rand() % MData.height;

      if(!CASE_VALIDE(i, j, terre))
	continue;
      break;
    }
    firsttunnel = new CTunnel(tmpRoom, 10, 10);
    firsttunnel->ConnectTo(terre, i, j, les_clans[k]);
    SDEBUG(W3, firsttunnel);
    tmpRoom->SetNiveau(0);

    les_clans[k]->NeededCiternes = 3;
    les_clans[k]->Citernes_under = MData.nb_tanks;
    les_clans[k]->NbCiterne = MData.nb_tanks;
    les_clans[k]->NbOuvrier = MData.nb_workers;
    les_clans[k]->NbSentinelle = MData.nb_guards;
    les_clans[k]->Population = MData.nb_workers +
      MData.nb_tanks + MData.nb_guards;
    les_clans[k]->DistAuto = les_clans[k]->Population / 2;
    les_clans[k]->les_fourmis.resize (MData.max_ants);
  }
}

void
User::Init_Queens(class DataMap &MData)
{
  int	i, j;
  
  for(int k = 0; k < MData.nb_clan; ++k)
  {
    i = F_LARGEUR / 2;
    j = F_HAUTEUR / 2;
    les_clans[k]->les_fourmis[0] = new Reine(i, j, 0,
					les_clans[k],
					les_clans[k]->la_fourme[0]);
    les_clans[k]->les_fourmis[0]->Nourriture = INITIAL_QUEENS_MEAT;
  }
}

void
User::Init_Workers(class DataMap &MData)
{
  int	i, j;

  for (int clan = 0; clan < MData.nb_clan; ++clan)
  {
    for (int k = 1; k < MData.nb_workers + 1; ++k)
    {
      i = rand() % MData.length;
      j = rand() % MData.height;

      if (!CASE_VIDE(i, j, terre))
      	{ 
	  k--;
	  continue;
	}
      les_clans[clan]->les_fourmis[k] =
	new Ouvrier(i, j, k, les_clans[clan]->ReineClan, terre);
    }
  }
}

void
User::Init_Guards(class DataMap &MData)
{
  int	i, j;
  
  for (int clan = 0; clan < MData.nb_clan; ++clan)
  {
    for (int k = 1 + MData.nb_workers; k < 1 + MData.nb_workers +
	MData.nb_guards; k++)
    {
      i = rand() % MData.length;
      j = rand() % MData.height;

      if (!CASE_VIDE(i, j, terre))
      {
	k--;
	continue;
      }
      les_clans[clan]->les_fourmis[k] =
	new Sentinelle(i, j, k, les_clans[clan]->ReineClan, terre);
    }
  }
}

void
User::Init_Tanks(class DataMap &MData)
{
  CClan		*tmpcl;
  int		i, j;
  
  for (int clan = 0; clan < MData.nb_clan; ++clan)
  {
    tmpcl = les_clans[clan];
    if (MData.nb_tanks > 0)
    {
      for (int k = 1 + MData.nb_workers + MData.nb_guards;
	  k < 1 + MData.nb_workers + MData.nb_guards + tmpcl->Citernes_under;
	  ++k)
      {
	i = rand() % F_LARGEUR; j = rand() % F_HAUTEUR;

	if (!CASE_VIDE(i, j, tmpcl->la_fourme[0]))
	{
	  k--;
	  continue;
	}
	tmpcl->les_fourmis[k] =
	  new Citerne(i, j, k, tmpcl->ReineClan, tmpcl->la_fourme[0]);
	tmpcl->les_fourmis[k]->setbUnder(true);
	tmpcl->les_fourmis[k]->Room = tmpcl->la_fourme[0];
      }
    }

    //On complète tous les emplacements qui ne sont inutilisés
    for (int k = 1 + les_clans[clan]->Population; k < MData.max_ants; ++k)
    {
      les_clans[clan]->les_fourmis[k] =
	new Ouvrier(0, 0, 0, tmpcl->ReineClan,terre);
      les_clans[clan]->les_fourmis[k]->setStatut(NIHIL);

    }
  }
}



void
User::Serialize(FILE * fichier, bool sauv, class IOTablePointeur &table){
	//static int i, j, k; //static pour conserver le numéro du pointeur à charger
	int i;
	unsigned short c=0;
	if(sauv){//sauvegarde
		if(fichier==NULL) //Dans ce cas, remplir la table de pointeurs
			return; //rien à faire ici : pas d'objet spécifique à User
		else{
			c =bInfo;
			c |=bInfo_enn<<1;
			c |=bPause<<2;
			c |=ButtonMeat<<3;
			c |=bDown<<4;
			c |=bMove<<5;
			c |=bSmall_map<<6;
			c |=bStats<<7;
			c |=bViolent<<8;
			c|=bScreen<<9;
			fwrite(&c,sizeof(unsigned short),1,fichier);
			fwrite(&column,sizeof(int),1,fichier);
			fwrite(&line,sizeof(int),1,fichier);
			fwrite(&counter,sizeof(counter),1,fichier);
			/*Pointeurs*/
			if(this_animal==NULL)
				SDEBUG(W2,"Sauv : le pointeur sur l'animal courant est NULL !");
			i=table.FindOrAdd((void *) this_animal);
			SDEBUG(W2,"Animal courant : "<<i<<" adresse "<<this_animal);
			fwrite(&i,sizeof(int),1,fichier);
			i=table.FindOrAdd((void *)this_room);
			SDEBUG(W2,"Room courante : "<<i<<" adresse "<<this_room);
			fwrite(&i,sizeof(int),1,fichier);
			i=table.FindOrAdd((void *) this_clan);
			SDEBUG(W2,"Clan courant : "<<i<<" adresse "<<this_clan);
			fwrite(&i,sizeof(int),1,fichier);
		}
	}
	else{	//Chargement
		if(fichier==NULL){ //Dans ce cas, lire la table de pointeurs
/* 2020: 64 bits pointers now... Cannot cast to int TODO 2020 FIXME loading should not be split between this function reading the index and another one replacing it with the pointer of the newly created object
			if((this_animal=(CAnimal *)table.get((int)this_animal))==NULL)
				SDEBUG(W2,"Chargement : le pointeur sur l'animal courant est NULL !");
			if((this_room=(CRoom *)table.get((int)this_room))==NULL)
				SDEBUG(W2,"Chargement : le pointeur sur la Room courante est NULL !");
			if((this_clan =(CClan *)table.get((int)this_clan))==NULL)
				SDEBUG(W2,"Chargement : le pointeur sur le Clan courant est NULL !");*/
			
		}
		else{//Chargement première étape : les pointeurs restent vides.
			fread(&c,sizeof(unsigned short),1,fichier);
			bInfo=(c&1);
			bInfo_enn=((c&2) >> 1);
			bPause=((c&4) >> 2);
			ButtonMeat=((c&8) >> 3);
			bDown=((c&16) >> 4);
			bMove=((c&32) >> 5);
			bSmall_map=((c&64) >> 6);
			bStats=((c&128) >> 7);
			bViolent=((c&256) >> 8);
			bScreen=((c&512) >> 9);
			fread(&column,sizeof(int),1,fichier);
			fread(&line,sizeof(int),1,fichier);
			fread(&counter,sizeof(counter),1,fichier);
			//Numéros des pointeurs dans la table !!! -> TODO 2020 : pointers are 64 bits now, cannot just read 32 bits...
			fread(&this_animal,sizeof(this_animal),1,fichier);
			fread(&this_room,sizeof(this_room),1,fichier);
			fread(&this_clan,sizeof(this_clan),1,fichier);
		}
	}
}

void
User::Init_Ants(class DataMap &MData)
{
  /// Reine ///
  SDEBUG(W0,"->Reines<-");
  Init_Queens(MData);

  /// Ouvriers ///
  SDEBUG(W0,"->Ouvriers<-");
  Init_Workers(MData);
  
  /// Sentinelles ///
  SDEBUG(W0,"->Sentinelles<-");
  Init_Guards(MData);

  /// Citernes ///
  SDEBUG(W0,"->Citernes<-");
  Init_Tanks(MData);
}

void
User::Init_Bugs(class DataMap &MData)
{
  int	i, j;
  
  for (int k = 0; k < MData.max_ennemies; ++k)
  {
    i = rand() % MData.length;
    j = rand() % MData.height;

    if (!CASE_VIDE(i, j, terre)) { k--; continue; }
    les_ennemis[k] = new Cafard(i, j, CAFARD, k, terre);
  }
}

void
User::Init_Ennemies(class DataMap &MData)
{
  /// Ennemis ///
  SDEBUG(W0,"->Ennemis<-");
  les_ennemis = new CAnimal*[MData.max_ennemies];
  Init_Bugs(MData);
}
void
User::Init_Landscape(class DataMap &MData)


{
  int	i, j;
  
  SDEBUG(W0,"->Nourriture<-");
  for (int k = 0;
      k < MData.meat_density * MData.length * MData.height / 100;
      ++k)
  {
    i = 1 + rand() % (MData.length - 2);
    j = 1 + rand() % (MData.height - 2);
    if (terre->map[i][j].decor->genre == ERIEN)
    {
      terre->map[i][j].decor->genre = VIANDE;
      terre->map[i][j].decor->etat = MData.max_meat;
    }
    else k--;
  }
}



//Ok
		
