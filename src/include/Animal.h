//
// Animal.h
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Mon Feb  4 13:46:12 2002 Ghost in the Shell
// Last update Thu Nov 27 00:56:47 2003 Ghost in the Shell
//

#ifndef ANIMAL_H
# define ANIMAL_H

class CAnimal
{
public:
	CAnimal(){}
	CAnimal(int x, int y, espece espec, int num, CRoom* room);
virtual ~CAnimal();
virtual ordre Action() = 0;
virtual void Paf(int choc) = 0;
	void		AjouteAnimalXY(int x, int y);
	void		SetPos(int x, int y);
	inline void	Aller_sur(int x, int y)
	{
	  Room->map[Xc][Yc].firstAnimal = 0;
	  SetPos(x, y);
	}
	void		Meurt();
	int		Avancer_vers(int lx, int ly, int dist);
	int		Avancer_hasard();
	class CAnimal*	RegardeEnnemi(int Peri);
//virtual bool Serialize(FILE *fichier, bool sauv)=0;
	inline int	Distance(int x, int y) const
		{ return((int)sqrt((float)(Xc-x)*(Xc-x)+(Yc-y)*(Yc-y)));}
	inline int	DistancePtr(CAnimal* ani) const
		{ return((int)sqrt((float)(Xc - ani->Xc) * (Xc - ani->Xc)
		      + (Yc - ani->Yc) * (Yc - ani->Yc)));}
	inline void	SetDest(int x, int y)
		{ Dest[0] = x; Dest[1] = y;}
	inline void	SetDestPtr(const CAnimal* ani)
		{ Dest[0] = ani->Xc; Dest[1] = ani->Yc;}
	int	GetPositionIndex() const;
	static signed char const     Deplacements[16];
// Propriétés réelles

	enum espece	Esp;
	int		Vie;
	int		Age;
	int		Nourriture;
	int		Taille;
// Propriétés informatiques
	class CRoom*	Room;
	class CClan*	Clan;
	int		Numero;
	int		Xc, Yc; 
	int		Oldpos[5][2];
	int		Dest[2];
	class CAnimal*	prev;
	class CAnimal*	next;	// Liste doublement chaînée
};

#endif
