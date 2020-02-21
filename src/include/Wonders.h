//////////////////////////////////////////////////////////////////////
//
// Wonders.h: d�clarations des types
//
//////////////////////////////////////////////////////////////////////

#ifndef WONDERS_H
# define WONDERS_H

enum e_screen {
  SFINTERFACE,
  SFGAME
};

enum espece {
  TOUT,
  FOURMI,
  PUNAISE,
  CAFARD
};

enum type {
  NIHIL,
  OUVRIER,
  SENTINELLE,
  NOURRICE,
  CITERNE,
  BERGER,
  LARVE,
  REINE
}; 


enum categorie {
  NOURRITURE,
  PCITERNE,
  DANGER,
  PISTE,
  NUL,
  ON
};
// ON veut dire qu'on est sur une case pheromone

enum ordre {DEAD, OK};
// Lorsque une chose a fait son action, elle est d�truite si elle renvoie DEAD 

enum comm {NICHTS, DEMANDE, OFFRE, HELP, VAZY};
// Type de communication : demande de bouffe, offre de bouffe, 
// demande � �tre suivie, donne une destination

enum nature {ERIEN, VIANDE, CADAVRE, MUR, CAILLOU};
// Type de d�cor d�pla�able ou bouffable

enum typeRoom {NORMAL_R, SOUSSOL_R, DESERT_R, FORET_R,VIDE_R};

struct CDecor
{
	nature genre;					// Bouffe, caillou, cadavre...
	int	 etat;						// D�pend du genre (quantit� pour la bouffe par ex)
};

struct CCase
{
  	int	z;				// Hauteur, altitude
	class CAnimal *firstAnimal;		// Liste cha�n�e des animaux
	class CPheromone *phero[3];		// Faire liste cha�n�e pour mettre plus de ph�romones
	CDecor *decor;					// D�cor d�pla�able ou bouffable, peut prendre plusieurs cases
	char terrain;					// Image de fond
	char offsetX, offsetY;			// Dans le cas o� il prend plusieurs cases, coordonn�es de l'image � afficher
};

#endif
