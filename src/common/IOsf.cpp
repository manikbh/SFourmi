//Fichier à maj

#include "../include/SFourmis.h"

struct head_sauv
{
  int version;
  int tablePointeurSize;
  /*int hauteur,largeur;
  int tour;
  int     nb_max_ennemi,nb_ennemi_alive;
  int quantite;
  BYTE nb_clan;*/
/*  int Volutabily_meat;
  int Volutabily_danger; Ce sont des const, pas utile.*/
  int	tot;
};

IOTablePointeur::IOTablePointeur(int size, bool realSize){
	//table.resize(size, NULL);
	table = new void*[size];
	if(realSize)
		taille = size;
	else
		taille=0;
	//for (int a=0; a<(this->table.size()); a++)
	//	table[a]=NULL;

}

IOTablePointeur::~IOTablePointeur(){
	delete[] table;
}
int IOTablePointeur::Find(void *pointeur){
	static int a;//Static pour ne pas recréer la var à chaque appel.
	for (a=0; a<taille; a++)
		if (table[a]==pointeur)
			return a;
	return -1; //Si introuvable
}

int IOTablePointeur::FindOrAdd(void *pointeur){
	int a;
	//cout<<(int)pointeur<<"\n";
	if( (a=this->Find(pointeur))!=-1){//Find
		SDEBUG(W2,"Pointeur trouvé "<<pointeur<<" en "<<a);
		return a;
	}
	SDEBUG(W2,"Ajout pointeur "<<pointeur<<" en "<<taille);
	/*if (taille>=this->size())//Add
		table.push_back(pointeur);
	else*/{
		table[taille]=pointeur;
		//cout<<"Dépassement !";
	}
	taille ++;
	return taille-1;
}

int IOTablePointeur::Add(void *pointeur){
	table[taille]=pointeur;
	taille ++;
	return taille-1;
}

void * IOTablePointeur::get(int a){
	if(a>0 &&a<taille&&this->table[a])
		SDEBUG(W2,"Pointeur NULL !");
	SDEBUG(W2, "Get "<<a);
	return (a<taille?this->table[a]:NULL);
}

void IOTablePointeur::set(int a, void * pointeur){
	SDEBUG(W2,"SET "<<pointeur<<" en "<<a);
	if(a>taille){
		SDEBUG(W2, "Le numero de pointeur est invalide !");
		return;
	}
	table[a]=pointeur;
}
//////////////////////SAUVEGARDE///////////////////
void Sauv_terrain()
{
	int kh;
	cout<<"\nDébut de sauvegarde...";

	/*Table des pointeurs*/
	IOTablePointeur table(MData.nb_clan * (MData.max_ants+50) + MData.max_ennemies+MData.length*MData.height*10 ,false);
	SDEBUG(W2, "Taille de la table de pointeurs :"<<MData.nb_clan * (MData.max_ants+50) + MData.max_ennemies+MData.length*MData.height*10);
	if(table.FindOrAdd((void *)NULL)!=0)
		SDEBUG(W2, "Le pointeur Null n'est pas à l'emplacement 0 !") ;
	if(table.FindOrAdd((void *)&MData)!=1)
		SDEBUG(W2, "Le pointeur &MData n'est pas à l'emplacement 1 !") ;
	/*Header*/
	head_sauv header; //SAUVER LES STATIC DE PHEROMONE
	header.version=3;//Inclus les ennemis !(version 2) CClan:version3
	//header.tot = CPheromone::tot;//FIXME Ca sert à quoi ? Et pourquoi le link ne marche pas avec ?
  //On ne sauve pas le header ici, mais à la fin après un rewind(sauveg)
  // : c'est nécessaire pour connaître la taille de la table des pointeurs

	FILE *sauveg;
	//Pour adapter les étapes de la barre de sauvegarde
	//NUMPHASE=MDate.nb_clan+6;//FIXME : rétablir le numphase global ?

  if (!(sauveg=fopen("Sfourmi.sf","w"))){
    cout << "Impossible de créer la sauvegarde !\n" ;
    return; //Si ça marche pas tant pis !
  }
	cout<<"\tFichier créé";
  //Sauvegarde des structures globales
  MData.Serialize(sauveg, true);
  fwrite("ZSF",3,1,sauveg);
  ZSF.Serialize(sauveg, true, table);
	cout<<"\tGlobal sauvé";
  //Sauvegarde du terrain:
	terre->Serialize(sauveg, true, table);
	cout<<"\tTerrain sauvé";
  //Sauvegarde des clans
  for(kh=0;kh<MData.nb_clan;kh++)
    {
      fwrite("BegClan",7,1,sauveg);
      les_clans[kh]->Serialize(sauveg,true,table);
	cout<<"\tClan "<<kh<<" sauvé";
    }

  //Ennemis:
  fwrite("Ennem",5,1,sauveg);
  for(kh=0; kh<MData.max_ennemies; kh++)
    if(les_ennemis[kh]!=NULL)
      ((Cafard *)les_ennemis[kh])->Serialize(sauveg,true, table);
  cout<<"\tEnnemis sauvés";

  //Header
   header.tablePointeurSize = table.size();
	SDEBUG(W2,"Taille de la table IO :");
	SDEBUG(W2,header.tablePointeurSize);
      //rewind(sauveg); //Retour au début du fichier
   fwrite(&header, sizeof(head_sauv), 1, sauveg);
   cout<<"\tSauvegarde achevée !\n";

   fclose(sauveg);
}
////////////////////////CHARGEMENT///////////////////////////////////////////////

bool Charg_terrain(std::string file)// A Besoin de MData et de ZSF, et l'existence des pointeurs les_clans les_ennemis et le pointeur vers le CRoom principal.
{
/*FIXME : ATTENTION : il faut créer la table des pointeurs et l'initialiser
à NULL à partir de sa taille (sauvée dans le header)
table.resize(size_t taille, (void *) NULL);
*/

	/****************************PREMIERE PARTIE : LECTURE***************************/
  head_sauv head;
  FILE *fichier;
  char buffer[10];
  int kh;
  SDEBUG (W2, "Chargement : lecture");
  if (!(fichier=fopen(file.c_str(),"r")))
    {
      SDEBUG (W2,"Echec de Open\n");
      std::cerr << "Cannot open file " << file << std::endl;
      return false;
    }
  //Lecture du header
  fseek(fichier, -sizeof(head_sauv),SEEK_END);
  fread(&head,sizeof(head_sauv),1,fichier);
  if (head.version!=3)
    {
      SDEBUG(W2,"Erreur version\n");
      return false;
    }
  rewind(fichier);

  IOTablePointeur table(head.tablePointeurSize, true);
    //Chargement des structures globales
  MData.Serialize(fichier, false);
	fread(buffer,3,1,fichier);
      if(strncmp("ZSF",buffer,3)!=0){
      		SDEBUG(W2,"ERREUR");
		return false;
	}
  ZSF.Serialize(fichier, false, table);

    //Chargement du terrain:
	terre = new CRoom();
	terre->Serialize(fichier, false, table);
  //Chargement des clans
  les_clans = new CClan*[MData.nb_clan];
  for(kh=0;kh<MData.nb_clan;kh++)
    {
      fread(buffer,7,1,fichier);
      if(strncmp("BegClan",buffer,7)!=0){
      		SDEBUG(W2,"ERREUR");
		return false;
	}
	les_clans[kh]=new CClan();
      les_clans[kh]->Serialize(fichier,false,table);
    }

  //Ennemis:
  les_ennemis = (CAnimal **)new Cafard*[MData.max_ennemies];
	fread(buffer,5,1,fichier);
	if(strncmp("Ennem",buffer,5)!=0){
		SDEBUG(W2,"ERREUR");
		return false;
	}
	for(kh=0; kh<MData.nb_ennemies; kh++){
		les_ennemis[kh] =(CAnimal *) new Cafard();
		((Cafard *)les_ennemis[kh])->Serialize(fichier,false, table);
	}
	for(kh=MData.nb_ennemies; kh<MData.max_ennemies; kh++)
		les_ennemis[kh]=NULL;
  fclose(fichier);
   /*******************DEUXIEME PARTIE : MISE A JOUR DES POINTEURS************/
  SDEBUG(W2,"Lecture ok");
  //global
  ZSF.Serialize(NULL, false, table);
  //terrain
  terre->Serialize(NULL, false, table);
  //Clans
  for(kh=0;kh<MData.nb_clan;kh++)
	  les_clans[kh]->Serialize(NULL,false,table);
  //Ennemis
  for(kh=0; kh<MData.nb_ennemies; kh++)
	  ((Cafard *)les_ennemis[kh])->Serialize(NULL,false, table);
  return(true);

}


