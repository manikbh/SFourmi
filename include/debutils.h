#ifndef _DEBUTIL_
#define _DEBUTIL_

#include "SFourmis.h"

#ifdef RELEASE
# define LEVEL W0
#else
# define LEVEL WALL
#endif

enum WLEVEL {W0,W1,W2,W3,W4,W5,W6,W7,W8,W9,WALL};
/*
 *WNONE : ce qui est forcément en sortie, pour une vitesse optimale
 *W1	: Déroulement normal pour tester, déboguer (surveillance de 1er niveau) 
 *W2	: Surveillance de niveau 2 ( 2 niveaux de paranthèses )
 *W3	: Un peu plus de surveillance aux endroits moins fréquentés (constructeurs, initialisations)
 *W4	: Anormalités prévisibles qui ne causent pas de plantages directs et peu fréquentes.
 *W5	: ... Après W5, sniffers aux endroits sensibles...
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
    Log << lev <<" #"<< __FILE__ << " @ " << __LINE__ << " : " << msg << std::endl; Log.flush(); \
    } \
};

#endif

