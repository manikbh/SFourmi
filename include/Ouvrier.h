//////////////////////////////////////////////////////////////////////
//
// Ouvrier.h: interface pour la classe Ouvrier
//
//////////////////////////////////////////////////////////////////////

#ifndef OUVRIER_H
#define OUVRIER_H

class Ouvrier : public Fourmi
{
public:
	Ouvrier() {}
	Ouvrier(int x,
			int y,
			int num,
			class Fourmi *reine,
			class CRoom *room);
	~Ouvrier();

	virtual enum ordre Action();
	virtual int Prend_Nourriture();
	virtual enum comm Repondre();
	//	virtual int Prend_Nourriture_under();
};

#endif
