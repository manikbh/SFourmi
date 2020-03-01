#ifndef DATAMAP_H
# define DATAMAP_H

# define XSMAP	16	
# define YSMAP	388
# define HSMAP	92
# define LSMAP	95

//using namespace std;
#include <string>

class	DataMap
{
  public:
    	DataMap();
	~DataMap();

	void		LoadData(std::string inifile);
	inline void	setScreen(enum e_screen s) {screen = s;}
	inline e_screen	Screen() const {return screen;}
	int		length;//TODO 2020 : provide default values and call the class SFourmiConfig or something
	int		height;
	int		nb_workers;
	int		nb_guards;
	int		nb_tanks;
	int		meat_density;
	int		fps;
	int		max_meat;
	int		nb_clan;
	int		ants_reborn;
	int		max_ants;
	int		max_ennemies;
	int		nb_ennemies;
	int		ennemies_reborn;
	std::string	loadfile;
	bool		charger;

	void Serialize(FILE *f, bool sauv);

	
  private:
	void		CheckMapParameters(int var, char *cmdline);
	enum e_screen	screen;

};

#endif
