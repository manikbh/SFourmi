//////////////////////////////////////////////////////////////////////
//
// Fourmi.h: interface pour la classe Fourmi
//
//////////////////////////////////////////////////////////////////////

#ifndef FOURMI_H
# define FOURMI_H

class IOTablePointeur;

class Fourmi : public CAnimal
{
public:
	Fourmi(){}
	Fourmi(	int 		x,
	    	int 		y,
		enum type	social,
		int 		num,
		class	Fourmi*	reine,
		class 	CRoom	*room);
virtual ~Fourmi();

virtual	enum ordre	Action() = 0;
virtual	int		Prend_Nourriture() = 0;
virtual	enum comm	Repondre() = 0;
virtual void		Paf(int choc);
	void		Attaque(class CAnimal *ani, int force) const;
	int		ChangeRoom(class CRoom *rm);
	int		GoNearestCiterne();
	int		EnterTunnel(const class CTunnel* Tunnel);
	int		Faire_Salle(int x, int y);
	int		Lacher_Pheromone(enum categorie Type);
	int		RegardeAnimalAutour(int Peri,
	    				bool cl = true,
					enum espece esp = FOURMI);
	int		RegardeNourritureAutour(int Peri) const;
	class Fourmi*	Plus_Proche(enum type defourmi) const;
	enum categorie	DoPhero(enum categorie Categ);
	enum categorie	Phero_autour(enum categorie Categ);

	inline void	setbUnder(bool b) {bunder = b;}
	inline bool	bUnder() const {return bunder;}
	inline void	setbParle(bool b) {bparle = b;}
	inline bool	bParle() const {return bparle;}
	inline void	setbPorteObj(bool b) {bporteObj = b;}
	inline bool	bPorteObj() const {return bporteObj;}
	inline void	setbTunnel(bool b) {btunnel = b;}
	inline bool	bTunnel() const {return btunnel;}
	inline void	setbDesti(bool b) {bdesti = b;}
	inline bool	bDesti() const {return bdesti;}
	inline void	setbCiterne(bool b) {bciterne = b;}
	inline bool	bCiterne() const {return bciterne;}
	inline void	setbStop(bool b) {bstop = b;}
	inline bool	bStop() const {return bstop;}
	inline void	setbDefense(bool b) {bdefense = b;}
	inline bool	bDefense() const {return bdefense;}
	inline void	setStatut(enum type e) {statut = e;}
	inline type	Statut() const {return statut;}
	inline void	setLoquace(int lev) {loquace = lev;}
	inline int	Loquace() const {return loquace;}

	void		Evolve();
	bool		Serialize(FILE *fichier, bool sauv, class CClan *clan, IOTablePointeur &table);

	// VRAIES CARACTERISTIQUES //
	int		LastPonte;
	int		Nourriture_dos;
	int		Taille_mandibule;
	int		Depot;
	int		Excitation;
	
	int		Wait;
	class Fourmi*	PCiterne;
	// CARACTERISTIQUES VIRTUELLES //
private:
	enum type	statut;
	int		loquace;
	unsigned int bunder:1;
	unsigned int bparle:1;		// Si elle parle
	unsigned int bporteObj:1;	// Si elle porte un objet
	unsigned int btunnel:1;		// Dans un tunnel ?
	unsigned int bdesti:1;		// Une destination ?
	unsigned int bciterne:1;		// On a trouvé une citerne et on verse du phéromone
	unsigned int bstop:1;			// Sert à savoir si besoin est de s'arrêter
	unsigned int bdefense:1;		// Si elle est attaquée
};

#endif
