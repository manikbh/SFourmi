////////////////////////////////////////////////////////////////////////////////////////////////
//
// Cafard.cpp: implémentation de la classe Cafard
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include "SFourmis.h"

////////////////////////////////////////////////////////////////////////////////////////////////

Cafard::Cafard(int x, int y, espece kelGenre, int num, CRoom* room) : CAnimal(x, y, CAFARD, num, room)
{
	Vie = 200+rand()%100;
	Agressif = 60+rand()%450;
	Bouclier = 15+rand()%335;
	Mandibule = 5+rand()%50;
	Taille = 10+rand()%150;
	bDefense = false;
}


////////////////////////////////////////////////////////////////////////////////////////////////

Cafard::~Cafard() {}

////////////////////////////////////////////////////////////////////////////////////////////////

void Cafard::Cherche_Copain()
{
}


//////////////////////////////////////////////////////////////////////////////////////////////

void Cafard::Attaque(CAnimal *ani)
{
	if (ani == NULL)    return;

	ani->Paf(Mandibule);
}


///////////////////////////////////////////////////////////////////////////////////////////////

void Cafard::Fuite()
{
	Avancer_hasard();
}


////////////////////////////////////////////////////////////////////////////////////////////////

void Cafard::Paf(int choc)
// Paramètres
// choc	: intensité du coup
{
	Vie -= choc;
	bDefense = true;
}


///////////////////////////////////////////////////////////////////////////////////////////////

ordre Cafard::Action()
{
	if (Vie < 0)
		return DEAD;
	if (!bDefense)								  // Rajouter Find_Ennemi et Attaque
		Avancer_hasard();
	else
	{
		bDefense = false;
		if (Agressif < 100)
			Fuite();
		else
			Attaque(RegardeEnnemi(1));
	}
	return OK;
}


///////////////////////////////////////////////////////////////////////////////////////////////

bool Cafard::Serialize(FILE *fichier, bool sauv, IOTablePointeur &table)
{

	int  i;
	if (sauv&&(fichier!=NULL))
	{
		i=table.FindOrAdd((void *)this); //Sauve son numero dans la table
		if(!fwrite(&i,sizeof(int),1,fichier)){
			 SDEBUG(W4,"Erreur d'écriture");
			return false;
		}
		fwrite(&Vie,1,sizeof(int),fichier);
		fwrite(&Numero,1,sizeof(int),fichier);
		fwrite(&Xc,1,sizeof(int),fichier);
		fwrite(&Yc,1,sizeof(int),fichier);
		fwrite(&Esp,1,sizeof(espece),fichier);
		fwrite(&Taille,1,sizeof(BYTE),fichier);
		fwrite(&Agressif,1,sizeof(int),fichier);
		fwrite(&Bouclier,1,sizeof(int),fichier);
		BYTE octet = bDefense;
		fwrite(&octet,1,sizeof(BYTE),fichier);
		fwrite(&Mandibule,1,sizeof(int),fichier);
		fwrite(&Paf_actuel,1,sizeof(int),fichier);
		fwrite(&Age,1,sizeof(int),fichier);
		fwrite(&Nourriture,1,sizeof(int),fichier);
		/*Les tableaux*/
		for (int g = 0; g < 5; g++)
		for (int h = 0; h < 2; h++)
			if(!fwrite(&Oldpos[g][h],sizeof(int),1,fichier)){
				SDEBUG(W4,"Erreur de lecture");
				return false;
			}
		for (int bo = 0; bo < 2; bo++)
			if(!fwrite(&Dest[bo],sizeof(int),1,fichier)){
				SDEBUG(W4,"Erreur de lecture");
				return false;
			}
	/*	Les pointeurs*/
		i=table.FindOrAdd((void *) Room);
		fwrite(&i,1,sizeof(int),fichier);
		i=table.FindOrAdd((void *) Clan);
		fwrite(&i,1,sizeof(int),fichier);
		i=table.FindOrAdd((void *) prev);
		fwrite(&i,1,sizeof(int),fichier);
		i=table.FindOrAdd((void *) next);
		fwrite(&i,1,sizeof(int),fichier);
	}
	else
	{
		if(fichier!=NULL){
			fread(&i,1,sizeof(int),fichier);
			table.set(i, (void *)this);
			fread(&Vie,1,sizeof(int),fichier);
			fread(&Numero,1,sizeof(int),fichier);
			fread(&Xc,1,sizeof(int),fichier);
			fread(&Yc,1,sizeof(int),fichier);
			fread(&Esp,1,sizeof(espece),fichier);
			fread(&Taille,1,sizeof(BYTE),fichier);
			fread(&Agressif,1,sizeof(int),fichier);
			fread(&Bouclier,1,sizeof(int),fichier);
			BYTE octet = bDefense;
			fread(&octet,1,sizeof(BYTE),fichier);
			fread(&Mandibule,1,sizeof(int),fichier);
			fread(&Paf_actuel,1,sizeof(int),fichier);
			fread(&Age,1,sizeof(int),fichier);
			fread(&Nourriture,1,sizeof(int),fichier);
			/*Les tableaux*/
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
			fread(&Room,1,sizeof(int),fichier);
			fread(&Clan,1,sizeof(int),fichier);
			fread(&prev,1,sizeof(int),fichier);
			fread(&next,1,sizeof(int),fichier);
		}else{//Reconstitution des pointeurs
			Room = (CRoom *) table.get(reinterpret_cast<int64_t>(Room));
			Clan = (CClan *) table.get(reinterpret_cast<int64_t>(Clan));
			prev = (CAnimal *)table.get(reinterpret_cast<int64_t>(prev));
			next = (CAnimal *)table.get(reinterpret_cast<int64_t>(next));
		}
	}
	return true;
}
