////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tunnel.cpp: implémentation de la classe CTunnel
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include "SFourmis.h"
#include "Clan.h"

////////////////////////////////////////////////////////////////////////////////////////////////

CTunnel::CTunnel(CRoom *from,int x, int y, CClan *clan)
// Paramètres
// from		:tunnel de provenance
// x		:abscisse d'entrée
// y		:ordonnée de l'entrée
// clan		:A mettre seulement lorsque l'on relit la terre
{
	In = from;
	In->Pipe.push_back(this);
	In->AjouteEntree(x,y,clan);
	Out = NULL;
	Longueur = 20;
	Resistance = 10;
	NbFourmi = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////
CTunnel::CTunnel(){
	In=Out=NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////

CTunnel::~CTunnel() {}

////////////////////////////////////////////////////////////////////////////////////////////////

void CTunnel::ConnectTo(CRoom *to,int x, int y,CClan *clan)
// Paramètres
// to		:tunnel de destination
// x		:abscisse d'entrée
// y		:ordonnée de l'entrée
// clan		:A mettre seulement lorsque l'on relit la terre
{
	//	if(to == In) return; //DANS CE CAS ? BIZARRE
	Out = to;
	Out->Pipe.push_back(this);
	Out->AjouteEntree(x,y,clan);
	SDEBUG(W3,Out);
	SDEBUG(W3,In);
	SDEBUG(W3,terre);
	SDEBUG(W3,Out->Pipe.size());
}


////////////////////////////////////////////////////////////////////////////////////////////////

void CTunnel::Serialize(FILE * fichier, bool sauv, IOTablePointeur &table)
{
	int in, out;
	if (sauv)
	{
		//fwrite("Tunnel",6,1,fichier);
		//Sauvons this !
		in = table.FindOrAdd((void *)this);
		fwrite(&in,sizeof(int),1,fichier);

		//Le reste
		fwrite(&Longueur,sizeof(int),1,fichier);
		fwrite(&Resistance,sizeof(BYTE),1,fichier);
		fwrite(&NbFourmi,sizeof(BYTE),1,fichier);
		in = table.FindOrAdd((void *)this->In);
		out = table.FindOrAdd((void *)this->Out);
		fwrite(&in,sizeof(int),1,fichier);
		fwrite(&out,sizeof(int),1,fichier);
	}
	else{

		if(fichier !=NULL){
			fread(&in,sizeof(int),1,fichier);
			SDEBUG(W2, "Tunnel : je suis "<<this<<" en "<<(int)in);
			table.set(in, (void *)this);
			fread(&Longueur,sizeof(int),1,fichier);
			fread(&Resistance,sizeof(BYTE),1,fichier);
			fread(&NbFourmi,sizeof(BYTE),1,fichier);
			fread(&this->In,sizeof(int),1,fichier);
			fread(&this->Out,sizeof(int),1,fichier);
			//SDEBUG(W2, "\tje vais (adresses dans la table) de "<<(int)in<<" à "<<(int)out);
			//cout<<"Passage1 in, out :"<<in<<", "<<out<<"\n";
		}else{
			//cout<<"Passage2 in, out :"<<in<<", "<<out<<"\n";
			this->In = (CRoom *) table.get(reinterpret_cast<int64_t>(this->In)); 
			this->Out = (CRoom *) table.get(reinterpret_cast<int64_t>(this->Out));
			//SDEBUG(W2, "Tunnel " << (int)this<<": je charge in :"<<(int)in<<" d'adresse réelle "<<(int)this->In<<" et out :"<<(int)out<<" d'adresse réelle "<<(int)this->Out);
		}
	}
}
