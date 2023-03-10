//////////////////////////////////////////////////////////////////////
//
// Sentinelle.h: interface pour la classe Sentinelle
//
//////////////////////////////////////////////////////////////////////

#ifndef _SENTINELLE_H_
#define _SENTINELLE_H_

#include "Fourmi.h"
#include "Wonders.h"

class Sentinelle : public Fourmi
{
public:
	Sentinelle() {}
	Sentinelle(int x, int y, int num, Fourmi *reine, CRoom *room);
	~Sentinelle();

	int Centre[2];
	int Peri;

	virtual ordre Action();
	virtual int Prend_Nourriture() { return 0; }
	virtual int Prend_Nourriture_under() { return 0; }
	virtual comm Repondre() { return NICHTS; }
	int CountEnnemi();
};

#endif
