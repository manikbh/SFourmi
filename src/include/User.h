#ifndef USER_H
# define USER_H
#include <string>
#include "SDLInterface.h"


class IOTablePointeur;
class	CRoom;
class	DataMap;
class	CAnimal;
class 	CClan;

class	User
{
  public:
	User();
	~User();
	void Serialize(FILE * fichier, bool sauv, IOTablePointeur &table);
	void		Initialize(class DataMap &MData);
	void    	ConstructEnvironment(class DataMap &MData);


	// !!!!!!!!!! ACCELERER AVEC TABLE SIN ET COS !!!!!!!!!!!!!!
	void MoveRoom();
	inline void	setThis_clan(CClan *c) {this_clan = c;}
	inline CClan *This_clan() const {return this_clan;}
	inline void	setThis_room(class CRoom *r) {this_room = r;}
	inline CRoom	*This_room() const {return this_room;}
	inline int	Counter() const {return counter;}
	inline void	IncCounter() {counter++;}
	inline CAnimal	*This_animal() const {return this_animal;}
	inline void	setThis_animal(CAnimal *canibal) {this_animal = canibal;}
	std::string	path;
	bool 		bInfo;
	bool 		bInfo_enn;
	bool 		bPause;
	bool 		ButtonMeat;
	bool 		bDown;			// armé si le bouton gauche activé
	bool 		bMove;			
	bool 		bSmall_map;		// F4 mini carte
	bool 		bStats;		// 'S' Statistiques

	bool 		bScreen;		// 'T' hyperespace
	bool 		bViolent;		// 'V' Violence
	bool 		bGoLeft;
	bool 		bGoRight;
	bool 		bGoUp;
	bool 		bGoDown;
	int		column;
	int		line;
	class CAnimal	*this_animal;

  private:
    	void      	CheckWall(class DataMap &MData);
	void		Init_Clan(class DataMap &MData);
	void		Init_Queens(class DataMap &MData);
	void		Init_Workers(class DataMap &MData);
	void		Init_Guards(class DataMap &MData);
	void		Init_Tanks(class DataMap &MData);
	void		Init_Ants(class DataMap &MData);
	void		Init_Bugs(class DataMap &MData);
	void   		Init_Ennemies(class DataMap &MData);
	void 		Init_Landscape(class DataMap &MData);
	int		counter;
    	class CClan * this_clan;
	class CRoom	*this_room;
};

#endif
