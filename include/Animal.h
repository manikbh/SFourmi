//
// Animal.h
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
//
// Started on  Mon Feb  4 13:46:12 2002 Ghost in the Shell
// Last update Mon Oct 21 23:47:56 2002 Ghost in the Shell
//

#ifndef ANIMAL_H
#define ANIMAL_H

class CAnimal
{
public:
	CAnimal() {}
	CAnimal(int x, int y, espece espec, int num, CRoom *room);
	virtual ~CAnimal();
	virtual ordre Action() = 0;
	virtual void Paf(int choc) = 0;
	void AjouteAnimalXY(int x, int y);
	void SetPos(int x, int y);
	inline void Aller_sur(int x, int y)
	{
		Room->map[Xc][Yc].firstAnimal = 0;
		SetPos(x, y);
	}
	void Meurt();
	int Avancer_vers(int lx, int ly, int dist);
	int Avancer_hasard();
	class CAnimal *RegardeEnnemi(int Peri);
	// virtual bool Serialize(FILE *fichier, bool sauv)=0;
	inline int Distance(int x, int y) const
	{
		return ((int)sqrt((float)(Xc - x) * (Xc - x) + (Yc - y) * (Yc - y)));
	}
	inline int DistancePtr(CAnimal *ani) const
	{
		return ((int)sqrt((float)(Xc - ani->Xc) * (Xc - ani->Xc) + (Yc - ani->Yc) * (Yc - ani->Yc)));
	}
	inline void SetDest(int x, int y)
	{
		Dest[0] = x;
		Dest[1] = y;
	}
	inline void SetDestPtr(const CAnimal *ani)
	{
		Dest[0] = ani->Xc;
		Dest[1] = ani->Yc;
	}
	inline int GetPositionIndex() const
	{
		int X = this->Xc - this->Oldpos[0][0];
		int Y = this->Yc - this->Oldpos[0][1];

		if (((X == 0) && (Y == 0)) || ((X == 0) && (Y == -1)))
			return 0;
		// vers HAUT DROITE
		else if ((X == 1) && (Y == -1))
			return 32;
		// vers DROITE
		else if ((X == 1) && (Y == 0))
			return 64;
		// vers BAS DROITE
		else if ((X == 1) && (Y == 1))
			return 96;
		// vers BAS
		else if ((X == 0) && (Y == 1))
			return 128;
		// vers BAS GAUCHE
		else if ((X == -1) && (Y == 1))
			return 160;
		// vers GAUCHE
		else if ((X == -1) && (Y == 0))
			return 192;
		// vers HAUT GAUCHE
		else if ((X == -1) && (Y == -1))
			return 224;
		return 0;
	}
	static constexpr signed char const Deplacements[16] = {-1, -1,
														   0, -1,
														   1, -1,
														   1, 0,
														   1, 1,
														   0, 1,
														   -1, 1,
														   -1, 0};
	// Propriétés réelles

	enum espece Esp;
	int Vie;
	int Age;
	int Nourriture;
	int Taille;
	// Propriétés informatiques
	class CRoom *Room;
	class CClan *Clan;
	int Numero;
	int Xc = 0, Yc = 0;
	int Oldpos[5][2];
	int Dest[2];
	class CAnimal *prev;
	class CAnimal *next; // Liste doublement chaénée
};

#endif
