//////////////////////////////////////////////////////////////////////
//
// Larve.h: Interface pour la classe Larve
//
//////////////////////////////////////////////////////////////////////

#ifndef LARVE_H
#define LARVE_H

class Larve : public Fourmi
{
public:
	Larve() {}
	Larve(int x,
		  int Y,
		  int nreine,
		  int num,
		  class Fourmi *reine,
		  class CRoom *room);
	virtual ~Larve();

	virtual enum ordre Action();
	virtual int Prend_Nourriture() { return 0; }
	virtual int Prend_Nourriture_under() { return 0; }
	virtual enum comm Repondre() { return NICHTS; }
};

#endif
