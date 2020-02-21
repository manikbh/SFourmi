//////////////////////////////////////////////////////////////////////
//
// Room.h: interface pour la classe CRoom
//
// Faire évoluer cette classe et y mettre
// - Solidité
// - Fonction ( liste dans un enum )
// - "Pourrissure"
// - T
//
// GiTs mars 2001
//
//////////////////////////////////////////////////////////////////////

#ifndef ROOM_H
# define ROOM_H

//# ifdef WIN32
#  include <vector>
//# endif
class IOTablePointeur;

using namespace std;

class CRoom
{

public:
	CRoom(int largeur, int hauteur, enum typeRoom tR, unsigned char altitudes[256][256]);
	CRoom(int largeur, int hauteur, typeRoom tR);
	CRoom();
	~CRoom();

	inline void	SetNiveau(int niv) { niveau = niv; };
	void		Init(int largeur, int hauteur, enum typeRoom tR) {Init(largeur, hauteur, tR, 0);}
	void		Init(int largeur, int hauteur, enum typeRoom tR, unsigned char altitudes[256][256]);
	void		SetEntrees(int x, int y, int num);
	class CTunnel*	GetTunnel(int x, int y) const;
	bool		Serialize(FILE *sauveg, bool sauv, IOTablePointeur &table);
	void		AjouteEntree(int x, int y, class CClan *clan);
	struct
	{
	  int largeur, hauteur;
	} size;

	struct CCase	**map;	// Tableau des différents éléments du sous-sol
	int niveau;		// Etage
	int		resistance;
	int		nb_entree, nb_sortie;
	int		FreeCells;// Nbre de cases vides (permet de déterminer le nbre moyen de citernes)

	std::vector<class CTunnel*> Pipe;// Tableau de tunnels répertoriés dans CClan
		// Liste de n° des pipes (tunnels) répertoriés dans vector<CTunnel> pipeline de CClan
	std::vector<int> Gate[2];
	

};

#endif

