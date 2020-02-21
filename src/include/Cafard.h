//////////////////////////////////////////////////////////////////////
//
// Cafard.h: interface pour la classe Cafard
//
//////////////////////////////////////////////////////////////////////

#ifndef CAFARD_H
# define CAFARD_H

class IOTablePointeur;

class Cafard : public CAnimal
{
public:
	Cafard(){}
	Cafard( int x,
	    	int y,
		enum espece kelgenre,
		int num, CRoom* room);
	~Cafard();
	enum ordre	Action();
	void		Cherche_Copain();
	void		Fuite();
	void		Attaque(CAnimal *ani);
virtual void		Paf(int choc);
	bool		Serialize(FILE *fichier, bool sauv, IOTablePointeur &table);

	int	Bouclier;
	int	Mandibule;
	int	Agressif;			// Réaction face à une attaque
	int	Paf_actuel;
	bool	bDefense;			// S'il doit réagir à un Paf
};
#endif
