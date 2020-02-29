////////////////////////////////////////////////////////////////////////////////////////////////
//
// Clans.cpp: implémentation de la classe CClan
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include "SFourmis.h"
////////////////////////////////////////////////////////////////////////////////////////////////

CClan::CClan()
{
  Gate[0].resize(1);
  Gate[1].resize(1);
  Gate[0].pop_back();
  Gate[1].pop_back();

}


////////////////////////////////////////////////////////////////////////////////////////////////

CClan::~CClan() {}

////////////////////////////////////////////////////////////////////////////////////////////////

class CTunnel *CClan::GetTunnel(int x, int y)
{
  for (unsigned int i = 0; i < terre->Pipe.size(); i++)
    if (x == terre->Gate[0][i] && y == terre->Gate[1][i])
      return terre->Pipe[i];
  SDEBUG(W7,"Pointeur Nul");
  return NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////

bool CClan::Serialize(FILE *sauveg, bool sauv, IOTablePointeur &table)
{
  int i;
  if (sauv && sauveg!=NULL) //SAUVEGARDE
  {
    //Le pointeur this
    i = table.FindOrAdd((void *)this);
    fwrite(&i,sizeof(int),1,sauveg);
    //Variables simples
    fwrite(&Citernes_under,sizeof(int),1,sauveg);	// Pas nécessaire mais accélère le chargement
    fwrite(&NeededCiternes,sizeof(int),1,sauveg);
    fwrite(&NBMAX_ROOM,sizeof(int),1,sauveg);
    fwrite(&DistAuto,sizeof(int),1,sauveg);			// Perimètre de défense
    fwrite(&POUVRIER,sizeof(int),1,sauveg);
    //fwrite(&NB_MAXIMUM,sizeof(int),1,sauveg);
    fwrite(&population,sizeof(int),1,sauveg);
    fwrite(&NbOuvrier,sizeof(int),1,sauveg);
    fwrite(&NbSentinelle,sizeof(int),1,sauveg);
    fwrite(&NbCiterne,sizeof(int),1,sauveg);
    // FOURMILLIERE //
    for (i = 0; i <= NBMAX_ROOM; i++)
      la_fourme[i]->Serialize(sauveg,sauv, table);

    // PIPE //
    fwrite("Pipe",4,1,sauveg);
    int nbpipe = Pipe.size();
    fwrite(&nbpipe,sizeof(int),1,sauveg);
    for (i = 0; i < nbpipe; i++)
      Pipe[i]->Serialize(sauveg,sauv, table);
    fwrite("Gate",4,1,sauveg);
    // GATES //
    nbpipe = Gate[0].size();
    fwrite(&nbpipe,sizeof(int),1,sauveg);
    for (i = 0; i < nbpipe; i++)
    {
      fwrite(&Gate[0][i],sizeof(int),1,sauveg); // Ecrit les 2 octets
      fwrite(&Gate[1][i],sizeof(int),1,sauveg);
    }

    //Sauvegarde des fourmis
    fwrite("Fourm",5,1,sauveg);

    for (int kb = 0; kb < MData.max_ants; kb++)
      switch (les_fourmis[kb]->Statut())
      {
	case NIHIL://on doit sauver pour des questions de pointeurs
	  fwrite("Nihil",5,1,sauveg);
	  break;
	case OUVRIER:
	  fwrite("Ouvri",5,1,sauveg);
	  les_fourmis[kb]->Serialize(sauveg,true,this,table);
	  break;
	case SENTINELLE:
	  fwrite("Senti",5,1,sauveg);
	  les_fourmis[kb]->Serialize(sauveg,true,this,table);
	  break;
	case NOURRICE:
	  fwrite("Nourr",5,1,sauveg);
	  les_fourmis[kb]->Serialize(sauveg,true,this,table);
	  break;
	case CITERNE:
	  fwrite("Citer",5,1,sauveg);
	  les_fourmis[kb]->Serialize(sauveg,true,this,table);
	  break;
	case BERGER:
	  fwrite("Berge",5,1,sauveg);
	  les_fourmis[kb]->Serialize(sauveg,true,this,table);
	  break;
	case LARVE:
	  fwrite("Larve",5,1,sauveg);
	  les_fourmis[kb]->Serialize(sauveg,true,this,table);
	  break;
	case REINE:
	  fwrite("Reine",5,1,sauveg);
	  les_fourmis[kb]->Serialize(sauveg,true,this,table);
	  break;
	default :
	  fwrite("Unkno",5,1,sauveg);
	  les_fourmis[kb]->Serialize(sauveg,true,this,table);
	  break;
      }
    i = table.FindOrAdd((void *)ReineClan);
    fwrite(&i,sizeof(int),1,sauveg);

    return(true);
  }
  else // Chargement
  {
    if(sauveg!=NULL){
      //THIS
      fread(&i,sizeof(int),1,sauveg);
      table.set(i, (void *)this);
      //Variables simples
      fread(&Citernes_under,sizeof(int),1,sauveg);
      fread(&NeededCiternes,sizeof(int),1,sauveg);
      fread(&NBMAX_ROOM,sizeof(int),1,sauveg);
      fread(&DistAuto,sizeof(int),1,sauveg);			// Périmètre de défense
      fread(&POUVRIER,sizeof(int),1,sauveg);
      //fread(&NB_MAXIMUM,sizeof(int),1,sauveg);
      fread(&population,sizeof(int),1,sauveg);
      fread(&NbOuvrier,sizeof(int),1,sauveg);
      fread(&NbSentinelle,sizeof(int),1,sauveg);
      fread(&NbCiterne,sizeof(int),1,sauveg);
      // FOURMILLIERE //
      SDEBUG(W0,"Debut chargement fourmillière");
      char *buff = new char[15];
      la_fourme = new CRoom*[NBMAX_ROOM+1];
      for (int kh = 0; kh <= NBMAX_ROOM; kh++)
      {
	SDEBUG(W0, kh);
	la_fourme[kh] = new CRoom(/*F_LARGEUR, F_HAUTEUR,SOUSSOL_R*/);
	la_fourme[kh]->Serialize(sauveg,sauv, table);
      }
      SDEBUG(W0,"Fin chargement fourmillière");

      // PIPE //
      fread(buff, 4,1, sauveg);
      if (strncmp(buff,"Pipe",4)!=0) {
	SDEBUG(W2, "Pipe absent !");
	return false;
      }
      int nbpipe;
      fread(&nbpipe,sizeof(int),1,sauveg);
      Pipe.resize(nbpipe,0);
      for (i = 0; i < nbpipe; i++){
	Pipe[i] = new CTunnel();
	Pipe[i]->Serialize(sauveg, sauv, table);
      }
      fread(buff, 4,1, sauveg);
      if (strncmp(buff,"Gate",4)!=0) {
	SDEBUG(W2, "Gate absent !");
	return false;
      }
      // GATES //
      fread(&nbpipe,sizeof(int),1,sauveg);
      Gate[0].resize(nbpipe,0);
      Gate[1].resize(nbpipe,1);

      for (i = 0; i < nbpipe; i++)
      {
	fread(&Gate[0][i],sizeof(int),1,sauveg);
	fread(&Gate[1][i],sizeof(int),1,sauveg);
      }


      // FOURMIS //
      NbCiterne = 0;
      NbSentinelle = 0;
      NbOuvrier = 0;
      char test3[6] = "00000";
      fread(test3,5,1,sauveg);
      if (strncmp(test3,"Fourm",5) != 0)
      {
	SDEBUG(W2,"Pas de signe Fourm");
	return false;
      }else
	SDEBUG(W2, "Signe Fourm ok, chargement des fourmis");
      //FIXME : UTILE OU PAS ?
      les_fourmis.resize (MData.max_ants);
      for (i = 0; i < MData.max_ants; i++)
      {
	SDEBUG(W2,"numero fourmi " << i);
	fread(test3,5,1,sauveg);
	if (strncmp(test3,"Nihil",5) == 0)
	{
	  les_fourmis[i] = new Ouvrier();
	  les_fourmis[i]->setStatut(NIHIL);
	}
	else if (strncmp(test3,"Ouvri",5) == 0)
	{
	  les_fourmis[i] = new Ouvrier();
	  les_fourmis[i]->Serialize(sauveg,false,this,table);
	  NbOuvrier++;
	}
	else if (strncmp(test3,"Senti",5) == 0)
	{
	  les_fourmis[i] = new Sentinelle();
	  les_fourmis[i]->Serialize(sauveg,false,this,table);
	  NbSentinelle++;
	}
	else if (strncmp(test3,"Nourr",5) == 0)
	{
	  les_fourmis[i] = new Ouvrier();
	  les_fourmis[i]->Serialize(sauveg,false,this,table);
	}
	else if (strncmp(test3,"Citer",5) == 0)
	{
	  les_fourmis[i] = new Citerne();
	  les_fourmis[i]->Serialize(sauveg,false,this,table);
	  NbCiterne++;
	}
	else if (strncmp(test3,"Berge",5) == 0)
	{
	  les_fourmis[i] = new Ouvrier();
	  les_fourmis[i]->Serialize(sauveg,false,this,table);
	}
	else if (strncmp(test3,"Larve",5) == 0)
	{
	  les_fourmis[i] = new Larve();
	  les_fourmis[i]->Serialize(sauveg,false,this,table);
	}
	else if (strncmp(test3,"Reine",5) == 0)
	{
	  les_fourmis[i] = new Reine();
	  les_fourmis[i]->Serialize(sauveg,false,this,table);
	  //numero_reine[num]=i;
	}
	else if (strncmp(test3,"Unkno",5) == 0)
	{
	  les_fourmis[i] = new Ouvrier();
	  les_fourmis[i]->Serialize(sauveg,false,this,table);
	}
	else
	{
	  SDEBUG(W4,"Invalide fourmi");
	  return false;
	}
	/*//On place les fourmis sur le terrain
	  if (les_fourmis[i]->Statut != NIHIL)
	//if (les_fourmis[i]->bUnder)			// Si sous terre
	//{
	les_fourmis[i]->AjouteAnimalXY(les_fourmis[i]->Xc,les_fourmis[i]->Yc);
	//}
	//else
	//les_fourmis[i]->AjouteAnimalXY(les_fourmis[i]->Xc,les_fourmis[i]->Yc);*/
      }
      //Pointeur Reine
      fread(&ReineClan,sizeof(int),1,sauveg);
      /*//Mise à jour des pointeurs PCiterne
	for (i = 0; i < NB_MAXIMUM; i++)
	{
	if(les_fourmis[i]->Statut!=NIHIL)
	if(les_fourmis[i]->PCiterne)
	les_fourmis[i]->PCiterne = les_fourmis[(int)(les_fourmis[i]->PCiterne)];
	}
	Population = NbCiterne+NbSentinelle+NbOuvrier;*/
      return true;
    }else{ //Fichier NULL : on met à jour les pointeurs.

      ReineClan = (Fourmi *) table.get(reinterpret_cast<int64_t>(ReineClan));
      //Fourmillière
      for (int kh = 0; kh <= NBMAX_ROOM; kh++)
	la_fourme[kh]->Serialize(NULL,sauv, table);
      //Pipe
      for (size_t p = 0; p < Pipe.size(); p++)
	Pipe[p]->Serialize(NULL, sauv, table);
      //Fourmis
      for (i = 0; i < MData.max_ants; i++)
	les_fourmis[i]->Serialize(NULL,false,this,table);

      return true;
    }
  }
}
