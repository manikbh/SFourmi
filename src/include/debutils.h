#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SFourmis.h"

#ifndef _DEBUTIL_
#define _DEBUTIL_

#ifdef RELEASE
# define LEVEL W0
#else
# define LEVEL WALL
#endif

enum WLEVEL {W0,W1,W2,W3,W4,W5,W6,W7,W8,W9,WALL};
/*
 *WNONE : ce qui est forc�ment en sortie, pour une vitesse optimale
 *W1	: D�roulement normal pour tester, d�boguer (surveillance de 1er niveau) 
 *W2	: Surveillance de niveau 2 ( 2 niveaux de paranth�ses )
 *W3	: Un peu plus de surveillance aux endroits moins fr�quent�s (constructeurs, initialisations)
 *W4	: Anormalit�s pr�visibles qui ne causent pas de plantages directs et peu fr�quentes.
 *W5	: ... Apr�s W5, sniffers aux endroits sensibles...
 *W6	: 
 *W7	: Pointeurs nuls
 *W8	: 
 *W9	: Erreurs dangers, ici risque de plantages garantis
 *WALL	: Active tout
 *
 */
#define SDEBUG(lev,msg) \
{ \
if(lev < LEVEL) \
    { \
    Log << lev <<" #"<< __FILE__ << " @ " << __LINE__ << " : " << msg << endl; Log.flush(); \
    } \
};

#endif

