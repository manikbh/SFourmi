//////////////////////////////////////////////////////////////////////
//
// Clan.h: interface pour la classe CClan
//
//////////////////////////////////////////////////////////////////////

#include <vector>

#ifndef CLAN_H
# define CLAN_H

class IOTablePointeur;

class CClan
{
public:
	CClan();
	~CClan();

	class CTunnel		*GetTunnel(int x, int y);
	bool			Serialize(FILE *sauveg, bool sauv, IOTablePointeur &table);
	
	vector<class Fourmi*>	les_fourmis;
	class CRoom		**la_fourme;

	// Sortie / entrée dans la fourmilière
	vector<int>		Gate[2];
	vector<class CTunnel*>	Pipe;

	int			Population;
	int			NbOuvrier,
				NbSentinelle,
				NbCiterne;
	int			NBMAX_ROOM;// Nbre de salles en cours
	int			Citernes_under;	// Citernes au sous-sol
	int			NeededCiternes;// Citernes requises au sous-sol (variera)
	int			DistAuto;// Perimètre de défense
	int			POUVRIER;
	class Fourmi		*ReineClan;
};
#endif

