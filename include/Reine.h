//////////////////////////////////////////////////////////////////////
//
// Reine.h: interface pour la classe Reine
//
//////////////////////////////////////////////////////////////////////

#ifndef REINE_H
#define REINE_H

class Reine : public Fourmi
{
public:
	Reine() {}
	Reine(int x,
		  int y,
		  int num,
		  class CClan *clan,
		  class CRoom *room);
	~Reine();

	virtual ordre Action();
	virtual int Prend_Nourriture() { return 0; }
	virtual comm Repondre() { return NICHTS; }
	virtual int Prend_Nourriture_under() { return 0; }
};

#endif
