////////////////////////////////////////////////////////////////////////////////
//
// Tunnel.h: interface pour la classe CTunnel
//
// Cette classe permet de relier les diff�rents salles
// Mais n'est pas repr�sent�e r��llement: elle est caract�ris�e par un integer
//  - d'entr�e vers CRoom
//  - de sortie vers CRoom
// et par une "distance", solidit�, "diam�tre" (pour le nbre max de fourmis), nbre de fourmis dedans.
// Les CEtage sont en d�finitive des "sortes" de "salles" qui sont connect�es entre elles par des
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
	CTunnel(CRoom *from,int x,int y, CClan *clan = NULL);
	CTunnel();
	~CTunnel();

	void ConnectTo(CRoom *to,int x,int y, CClan *clan = NULL);
	void Serialize(FILE * fichier, bool sauv, IOTablePointeur &table);

	CRoom *In;
	CRoom *Out;
	int Longueur;
	int Resistance;
	int NbFourmi;
//	int MaxFourmi;	// Pas une priorit�
};

#endif
