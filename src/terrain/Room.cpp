//
// Room.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Mon Feb  4 00:27:12 2002 Ghost in the Shell
// Last update Sat Oct 19 00:12:40 2002 Ghost in the Shell
//

#include "SFourmis.h"

CRoom::CRoom(int largeur, int hauteur, typeRoom tR)
{
  Gate[0].resize(1);
  Gate[1].resize(1);
  Gate[0].pop_back();
  Gate[1].pop_back();
  Init(largeur, hauteur, tR);
}

CRoom::CRoom(){}

CRoom::~CRoom()
{
  /*	int i, j;

	for (i = 0; i < size.largeur; i++)
	for (j = 0; j < size.hauteur; j++)
	delete[] underground[i][j];
	for (i = 0; i < size.largeur; i++) delete[] underground[i];*/
}

void
CRoom::Init(int largeur, int hauteur, typeRoom tR)
  // Paramètres
  // Largeur	:largeur de la Room
  // Longeur	:longueur
  // tR		:permet de générer une map cohérente selon le type de room (extérieur, intérieur, forêt, désert ...
{
  int i, j ,k ;
  size.largeur = largeur;
  size.hauteur = hauteur;

  map = new CCase*[largeur];
  for (i = 0; i < largeur; i++)
  {
    map[i] = new CCase[hauteur];
    for (j = 0; j < hauteur; j++)
    {
      map[i][j].decor = new CDecor;
      map[i][j].terrain = 0;
      map[i][j].decor->genre = ERIEN;
      map[i][j].firstAnimal = NULL;

      for( k = 0; k<  PHERO_SIZE ; k++)
	map[i][j].phero[k] = new CPheromone( NUL , 0, 0, 0);
    }
  }
  switch (tR)
  {
    case NORMAL_R:							  //Génère des montagnes
      for (i = 0; i < largeur; i++)
	for (j = 0; j < hauteur; j++)
	{
	  map[i][j].terrain = rand()%6;
	}
      for(k=0; k< largeur / 10;k++)
      {
	i= rand()%largeur;
	j= rand()%hauteur;
	int u=rand()%10+2;int v=rand()%10+2;

	for(int h=-u;h<u;h++)
	  for(int l=-v;l<v;l++)
	    if((CASE_VALIDE(i+h,j+l,this)))
	      map[i+h][j+l].decor->genre= MUR;
      }
      break;
    case SOUSSOL_R:
      for (i = 0; i < largeur; i++)
	for (j = 0; j < hauteur; j++)
	{
	  map[i][j].terrain = 0;			  //MAJ
	}
      break;
    case VIDE_R:
	break;
    default:
	break;
  }

}

class CTunnel*
CRoom::GetTunnel(int x, int y) const
// Paramètres
// x		:coordonnée x dont on veut savoir le numéro de tunnel (dans la room this)
// y		:idem pour y
//
// Valeurs de sortie
// NULL		:erreur pas de tunnel à cet endroit
// CTunnel*	:pointeur de tunnel directement exploitable dans CClan Pipeline
{
  SDEBUG(W3,Pipe.size());
  for (unsigned int i = 0; i < Pipe.size(); i++)
    if (x == Gate[0][i] && y == Gate[1][i])
    {
      SDEBUG(W3, Pipe[i]);
      return Pipe[i];
    }
  return NULL;
}


void
CRoom::SetEntrees(int x, int y, int num)
  // Paramètres
  // x		:Entrée x
  // y		:Entrée y
  // num	:numéro de l'entrée à modifier
{
  Gate[0][num] = x;
  Gate[1][num] = y;
}

void
CRoom::AjouteEntree(int x, int y, CClan *clan)
{

  if (this == terre)
  {

    clan->Gate[0].push_back(x);
    clan->Gate[1].push_back(y);
    SDEBUG(W3,x);SDEBUG(W3,y);
    SDEBUG(W3,clan->Gate[1].size());
  }
  Gate[0].push_back(x);
  Gate[1].push_back(y);
}


bool
CRoom::Serialize(FILE *sauveg,bool sauv, IOTablePointeur &table)
{
	int i, j, k2;
	unsigned k;
	if (sauv && sauveg!=NULL)// Sauvegarde
	{
		//THIS
		k=table.FindOrAdd((void*) this);
		fwrite(&k, sizeof(int),1,sauveg);

		//Variables simples
		fwrite(&niveau,sizeof(int),1,sauveg);
		fwrite(&nb_entree,sizeof(int),1,sauveg);
		fwrite(&nb_sortie,sizeof(int),1,sauveg);
		fwrite(&FreeCells,sizeof(int),1,sauveg);
		fwrite(&resistance,sizeof(int),1,sauveg);
		fwrite(&size.largeur,sizeof(int),1,sauveg);
		fwrite(&size.hauteur,sizeof(int),1,sauveg);
		fwrite("DebEntr",7,1,sauveg);

		//Variables vecteur
		k = Pipe.size();
		fwrite(&k,sizeof(int),1,sauveg);
		for (k = 0; k < Pipe.size(); k++){
			k2=table.FindOrAdd((void*)Pipe[k]);
			fwrite(&k2,sizeof(int),1,sauveg);
			if(this==Pipe[k]->In){
				fwrite("i",sizeof(char),1,sauveg);//Ici
				Pipe[k]->Serialize(sauveg, sauv, table);
			}else
				fwrite("a",sizeof(char),1,sauveg);//Ailleurs
		}
		k = Gate[0].size();
		fwrite(&k,sizeof(int),1,sauveg);
		for (k = 0; k < Gate[0].size(); k++){
			fwrite(&Gate[0][k],sizeof(int),1,sauveg);
			fwrite(&Gate[1][k],sizeof(int),1,sauveg);
		}
		fwrite("Map",3,1,sauveg);
		CCase ca; //Objet temporaire pour sauver
		CDecor *cd;
		for (k = 0; k < size.largeur; k++) //On parcourt le terrain
		for (k2 = 0; k2 < size.hauteur; k2++){
			ca=map[k][k2];
			//SDEBUG(W2,"en "<<k<<","<<k2<<" ca, map "<<&ca<<", "<<&map[k][k2]);
			fflush(sauveg);
			if((i=table.Find((void *)(cd = ca.decor)))==-1){ //Si le décor n'est pas encore sauvé
				ca.decor = (CDecor *) table.Add((void *)ca.decor);
				fwrite("i",sizeof(char),1,sauveg);//A sauver "I"ci
				fwrite(cd, sizeof(CDecor),1, sauveg);//On sauve le décor
			}else{
				ca.decor =(CDecor *) i;
				fwrite("a",1,1,sauveg);//Ok, déjà sauvé "A"illeurs
			}
			ca.firstAnimal = (CAnimal *)table.FindOrAdd((void *)ca.firstAnimal);
			//Sauvegarde de la Case
			fwrite(&ca.terrain, sizeof(char),1,sauveg); //Ne pas sauver les pointeurs phero ni le pointeur CDecor
			fwrite(&ca.offsetX, sizeof(char),1,sauveg);
			fwrite(&ca.offsetY, sizeof(char),1,sauveg);
			fwrite(&ca.firstAnimal, sizeof(int),1,sauveg);
			fwrite(&ca.decor, sizeof(int),1,sauveg);
			fwrite(&PHERO_SIZE, sizeof(PHERO_SIZE),1,sauveg);
			SDEBUG(W2," ca : "<<(int)ca.terrain<<", "<<(int)ca.offsetX<<", "<<(int)ca.offsetY<<", "<<ca.firstAnimal<<", "<<ca.decor<<" PheroSize = "<<(int)PHERO_SIZE);
			//Sauvegarde des phéromones
			for(i=0; i<PHERO_SIZE; i++)
				ca.phero[i]->Serialize(sauveg, sauv, table);
			//fwrite("o",1,1, sauveg);//DEBUG
		}

	}else if(sauveg!=NULL){		// Chargement du fichier
		//THIS
		fread(&k,sizeof(int),1,sauveg);
		table.set(k, (void *)this);
		//Variables simples
		fread(&niveau,sizeof(int),1,sauveg);
		fread(&nb_entree,sizeof(int),1,sauveg);
		fread(&nb_sortie,sizeof(int),1,sauveg);
		fread(&FreeCells,sizeof(int),1,sauveg);
		fread(&resistance,sizeof(int),1,sauveg);
		fread(&size.largeur,sizeof(int),1,sauveg);
		fread(&size.hauteur,sizeof(int),1,sauveg);
		//Test d'intégrité
		char *buff = new char [15];
		if (buff == NULL) return false;
		fread(buff,7,1,sauveg);
		if (strncmp(buff,"DebEntr",7) != 0)
			SDEBUG(W2, "Gros Problème de chargement de terrain !");

		//Variables vecteur
		fread(&i,sizeof(int),1,sauveg);
		Pipe.resize(i);
		for (k = 0; k < i; k++)
		{
			fread(&Pipe[k],sizeof(int),1,sauveg);
			fread(buff, 1, 1, sauveg);
			if(buff[0]=='i'){
				CTunnel *newTunnel= new CTunnel();
				newTunnel->Serialize(sauveg, sauv, table);
			}else if(buff[0]=='a'){
				SDEBUG(W2, "Erreur de chargement de tunnel : il est ailleurs");
			}else{
				SDEBUG(W2, "Erreur de chargement de tunnel : marqueur trouvé :"<<buff[0]);
			}
		}

		fread(&i,sizeof(int),1,sauveg);
		Gate[0].resize(i);
		Gate[1].resize(i);
		for (k = 0; k < i; k++)
		{
			fread(&Gate[0][k],sizeof(int),1,sauveg);
			fread(&Gate[1][k],sizeof(int),1,sauveg);
		}
		//Test d'intégrité
		fread(buff,3,1,sauveg);
		if (strncmp(buff,"Map",3) != 0)
			SDEBUG(W2, "Carte introuvable !");
		//Carte
		Init(size.largeur, size.hauteur, VIDE_R); //CREE LES PHEROS et CDECOR : ne pas les recréer
		for (k = 0; k < size.largeur; k++)
		for (k2 = 0; k2 <size.hauteur; k2++)
		{
			fread(buff,sizeof(char),1,sauveg);
			if(buff[0]=='i')
				fread(map[k][k2].decor, sizeof(CDecor),1, sauveg);
			else if(buff[0]=='a'){ //NON FONCTIONNEL, NON UTILISE
				fread(&map[k][k2].decor, sizeof(int),1, sauveg);
				SDEBUG(W2, "C'est bizarre... un décor non présent...");
			}else{
				SDEBUG(W2, "Erreur de chargement de décor !");
			}
			fread(&map[k][k2].terrain, sizeof(char),1,sauveg); 
			fread(&map[k][k2].offsetX, sizeof(char),1,sauveg);
			fread(&map[k][k2].offsetY, sizeof(char),1,sauveg);
			fread(&map[k][k2].firstAnimal, sizeof(int),1,sauveg);//TODO 2020: reading just 32 bits will give random values in a 64bits pointer type
			fread(&i, sizeof(int),1,sauveg);//Numero du pointeur vers décor dans la table
			SDEBUG(W2, "map : "<<(int)map[k][k2].terrain<<", "<<(int)map[k][k2].offsetX<<", "<<(int)map[k][k2].offsetY<<", "<<map[k][k2].firstAnimal<<", "<<(int)i);
			table.set(i, (void*)map[k][k2].decor);
			map[k][k2].decor=(CDecor *)i;
			j=0;
			fread(&j, sizeof(PHERO_SIZE),1,sauveg);//NON UTILISE !
			SDEBUG(W2, "Taille de phéro (3 en principe) lue :" << j);
			for(i=0; i<j; i++)//Chargement des phéromones
				map[k][k2].phero[i]->Serialize(sauveg, sauv, table);


		}
	}else{//Mise à jour des pointeurs
		for (k = 0; k < size.largeur; k++)
		for (k2 = 0; k2 <size.hauteur; k2++){
			map[k][k2].firstAnimal=(CAnimal *)table.get(reinterpret_cast<int64_t>(map[k][k2].firstAnimal));
			map[k][k2].decor=(CDecor *)table.get(reinterpret_cast<int64_t>(map[k][k2].decor));
		}
		for (k = 0; k < Pipe.size(); k++){
			Pipe[k] = (CTunnel *) table.get(reinterpret_cast<int64_t>(Pipe[k]));
			if (table.get(reinterpret_cast<int64_t>(Pipe[k]->In)) == this){
				Pipe[k]->Serialize(NULL, sauv, table);
			}
		}
	}
	return true;
}
