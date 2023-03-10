//////////////////////////////////////////////////////////////////////
//
// Citerne.h: interface pour la classe Citerne
//
//////////////////////////////////////////////////////////////////////

#ifndef CITERNE_H
#define CITERNE_H

class Citerne : public Fourmi
{
public:
	Citerne() {}
	Citerne(int x,
			int y,
			int num,
			Fourmi *reine,
			CRoom *room);
	virtual ~Citerne();

	virtual enum comm Repondre();
	virtual enum ordre Action();
	virtual int Prend_Nourriture() { return 0; }
};

#endif
