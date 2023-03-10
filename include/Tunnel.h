////////////////////////////////////////////////////////////////////////////////
//
// Tunnel.h: interface pour la classe CTunnel
//
// Cette classe permet de relier les différents salles
// Mais n'est pas représentée réellement: elle est caractérisée par un integer
//  - d'entrée vers CRoom
//  - de sortie vers CRoom
// et par une "distance", solidité, "diamètre" (pour le nbre max de fourmis), nbre de fourmis dedans.
// Les CEtage sont en définitive des "sortes" de "salles" qui sont connectées entre elles par des
// tunnels. (des pipes comme sous Unix)
//
// GiTs mars 2001
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _TUNNEL_H_
#define _TUNNEL_H_

#include "Wonders.h"
#include "Room.h"

class IOTablePointeur;

class CTunnel
{
public:
	CTunnel(CRoom *from, int x, int y, CClan *clan = NULL);
	CTunnel();
	~CTunnel();

	void ConnectTo(CRoom *to, int x, int y, CClan *clan = NULL);
	void Serialize(FILE *fichier, bool sauv, IOTablePointeur &table);

	CRoom *In;
	CRoom *Out;
	int Longueur;
	int Resistance;
	int NbFourmi;
	//	int MaxFourmi;	// Pas une priorité
};

#endif
