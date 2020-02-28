//////////////////////////////////////////////////////////////////////
//
// Pheromone.h: interface pour la classe CPheromone
//
//////////////////////////////////////////////////////////////////////

#ifndef PHEROMONE_H
# define PHEROMONE_H

class IOTablePointeur;

class CPheromone  
{
public:
	CPheromone(	enum categorie cat,
	    		int intense,
			int vectX, 
			int vectY);
	~CPheromone();
	enum ordre	Action();	// A chaque tour l'intensité baisse
	inline int	Interaction(int num); 
	inline int	GetPositionIndex() const
	  		{
			  int 	X = this->Vex;
			  int 	Y = this->Vey;

			  if (((X == 0) && (Y == 0)) || ((X == 0) && (Y == -1)))
			    return 0;
			    //vers HAUT DROITE
			  else if ((X == 1) && (Y == -1))
			    return 32;
			    //vers DROITE
			  else if ((X == 1) && (Y == 0))
			    return 64;
			    //vers BAS DROITE
			  else if ((X == 1) && (Y == 1))
			    return 96;
			    //vers BAS
			  else if ((X == 0) && (Y == 1))
			    return 128;
			    //vers BAS GAUCHE
			  else if ((X == -1) && (Y == 1))
			    return 160;
			    //vers GAUCHE
			  else if ((X == -1) && (Y == 0))
			    return 192;
			    //vers HAUT GAUCHE
			  else if ((X == -1) && (Y == -1))
			    return 224;
			  return 0;
			}
static	int const	Volutabily_meat;
static	int const	Volutabily_danger;
static	int	tot;
	void		Serialize(FILE * fichier, bool sauv, IOTablePointeur &table);

	short		Vex, Vey;	// Vecteur directionnel unitaire
	short		Intensite, NbTour;
	enum categorie	Genre;
};
#endif

