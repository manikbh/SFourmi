//////////////////////////////////////////////////////////////////////
//
// Wonders.h: déclarations des types
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
// Lorsque une chose a fait son action, elle est détruite si elle renvoie DEAD 

enum comm {NICHTS, DEMANDE, OFFRE, HELP, VAZY};
// Type de communication : demande de bouffe, offre de bouffe, 
// demande à être suivie, donne une destination

enum nature {ERIEN, VIANDE, CADAVRE, MUR, CAILLOU};
// Type de décor déplaçable ou bouffable

enum typeRoom {NORMAL_R, SOUSSOL_R, DESERT_R, FORET_R,VIDE_R};

struct CDecor
{
	nature genre;					// Bouffe, caillou, cadavre...
	int	 etat;						// Dépend du genre (quantité pour la bouffe par ex)
};

struct CCase
{
  	int	z;				// Hauteur, altitude
	class CAnimal *firstAnimal;		// Liste chaînée des animaux
	class CPheromone *phero[3];		// Faire liste chaînée pour mettre plus de phéromones
	CDecor *decor;					// Décor déplaçable ou bouffable, peut prendre plusieurs cases
	char terrain;					// Image de fond
	char offsetX, offsetY;			// Dans le cas où il prend plusieurs cases, coordonnées de l'image à afficher
};

#endif
