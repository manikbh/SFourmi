#include "SFourmis.h"

#ifdef WIN32
# include "WinInterface.h"
#endif

#ifdef GTK_Linux
# include <gdk/gdk.h>
# include <gdk-pixbuf/gdk-pixbuf.h>
# include "GTKInterface.h"
#endif

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"
#include "IOsf.h"

/////////////////////////////////////////////////////////////////////////////////
//	PROCEDURES PRINCIPALES
/////////////////////////////////////////////////////////////////////////////////

void
Process_va()
{  
  // Gestion du tour des fourmis
  for (int ceclan = 0; ceclan < MData.nb_clan; ceclan++)
  {
    CClan *clan = les_clans[ceclan];
    for (int k = 0; k < MData.max_ants; k++)
    {
      Fourmi *fourmi = clan->les_fourmis[k];
      switch (fourmi->Statut())
      {
	case NIHIL:
	  break;
	case CITERNE:
	  SDEBUG(W1,"#C#");
	  if(fourmi->Action() == DEAD)
	  {
	    fourmi->Room->map[fourmi->Xc][fourmi->Yc].firstAnimal = NULL;
	    fourmi->Room->map[fourmi->Xc][fourmi->Yc].decor->genre = CADAVRE;
	    clan->Citernes_under--;
	    fourmi->setStatut(NIHIL);
	    clan->Population--;
	    clan->NbCiterne--;

	  }
	  break;
	case OUVRIER :
	  SDEBUG(W1,"#O#");
	  if(fourmi->Action() == DEAD)
	  {
	    fourmi->Meurt();
	    fourmi->setStatut(NIHIL);
	    clan->Population--;
	    clan->NbOuvrier--;
	  }
	  break;
	case SENTINELLE :
	  SDEBUG(W1,"#S#");
	  if(fourmi->Action()==DEAD)
	  {
	    fourmi->Meurt();
	    fourmi->setStatut(NIHIL);
	    clan->Population--;
	    clan->NbSentinelle--;
	  }
	  break;
	case REINE:
	  SDEBUG(W1,"#R#");
	  fourmi->Action();
	  break;
	case LARVE:
	  SDEBUG(W1,"#L#");
	  {
	    auto fStatus = fourmi->Action();
	    if(fStatus==EVOLVE)
	      delete fourmi; // It was already replaced in the clan's array by the new Fourmi...
	    else if(fStatus==DEAD)
	    {
	      fourmi->Meurt();
	      fourmi->setStatut(NIHIL);
	      clan->Population--;
	      clan->NbSentinelle--;
	    }
	  }
	  break;
	default:
	  break;
      }
    }
    clan->DistAuto = clan->Population / 2;
    //MAJ
    clan->NeededCiternes = clan->la_fourme[0]->FreeCells / 250 + 2;
    SDEBUG(W1,"#EF#");
  }  //fin de Clan
  //Ennemis
  SDEBUG(W1,"#E#");
  for (int k = 0; k < MData.max_ennemies; k++)
    if (les_ennemis[k] != NULL)
      if (les_ennemis[k]->Action() == DEAD)
      {
	if (ZSF.bInfo_enn && (les_ennemis[k] == ZSF.This_animal()))
	  ZSF.bInfo_enn=FALSE;
	les_ennemis[k]->Meurt();
	//	  delete les_ennemis[k];
	les_ennemis[k]=NULL;
      }

  //Ajoute de la nourriture;
  if (rand() % 100 <= 4)
  {
    int i = 1 + rand() % (MData.length - 2);
    int j = 1 + rand() % (MData.height - 2);
    if (terre->map[i][j].decor->genre == ERIEN)
    {
      terre->map[i][j].decor->genre = VIANDE;
      terre->map[i][j].decor->etat = MData.max_meat;
    }
  }
}



