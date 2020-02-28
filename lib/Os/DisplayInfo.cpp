//
// DisplayInfo.cc
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Mon Feb  4 13:41:27 2002 Ghost in the Shell
// Last update Tue Oct 22 22:15:25 2002 Ghost in the Shell
//


#include "SFourmis.h"

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"

#include <cstdint>
#include <string>

extern CRoom*		terre;
extern CClan**		les_clans;
extern CAnimal**	les_ennemis;

static void
DisplayIcon(class DataMap& MData, class User& ZSF)
{
  class SFRect		rc;

  if (ZSF.bInfo)	  //les infos ne portent que sur les fourmis
  {
    switch (static_cast<Fourmi *> (ZSF.This_animal())->Statut())
    {
      case REINE:
	rc.setSFRect(0,8*32+600,32,8*32+632);
	break;
      case OUVRIER:
	rc.setSFRect(32,8*32+600,64,8*32+632);
	break;
      case SENTINELLE:
	rc.setSFRect(96,8*32+600,128,8*32+632);
	break;
      case CITERNE:
	rc.setSFRect(64,8*32+600,96,8*32+632);
	break;
      case LARVE:
	rc.setSFRect(64,4*32+600,96,4*32+632);
	break;
      case NIHIL:
	rc.setSFRect(0, 0, 0, 0);
	ZSF.bInfo = false;
	break;
      default:
	break;
    }
    SFDrawSurface(22, 180, rc);
  }
  else
    if (ZSF.bInfo_enn)
    {
      switch (ZSF.This_animal()->Esp)
      {
        case CAFARD:
	  rc.setSFRect(4*32,8*32+600,4*32+32,8*32+632);
	  break;
        default :
	  rc.setSFRect(0,0,0,0);
	  break;
      }
      SFDrawSurface(22, 180, rc);
    }
}

static void
DisplayGeneralInfo(class DataMap& MData, class User& ZSF)
{
  class SFRect	rc;
  char		buffer[5];
  class SFColor	colors;

  SFTextColor(255, 255, 255);

  sprintf(buffer,"%" PRIu64, ZSF.Counter());
  SFDisplay(24, 128, "Tours:");
  SFDisplay(80, 128,buffer);

  sprintf(buffer,"%d", ZSF.This_clan()->Population);
  SFDisplay(518, 582, buffer);				  // Population

#ifndef RELEASE
  sprintf(buffer,"%d", MData.max_ants);
  SFDisplay(620, 576, buffer);
  sprintf(buffer,"%d", ZSF.This_clan()->Citernes_under);
  SFDisplay(0, 576, buffer);
#endif
  
  /* FIXME : numéro du clan ?
  sprintf(buffer,"%d", ZSF.This_clan()+1);			  //Clan
  SFDisplay(510, 580, "Clan");
  SFDisplay(545, 580, buffer);*/
  colors.setSFColor(255 * (reinterpret_cast<int64_t>(ZSF.This_clan()) % 2) * 32,
      		    255 * (reinterpret_cast<int64_t>(ZSF.This_clan()) % 3) * 32,
		    255 * (reinterpret_cast<int64_t>(ZSF.This_clan()) % 4) * 32);

  rc.setSFRect(483, 578, 499, 593);
  SFDrawDCRectangle(rc, colors);
}

static void
DisplayEnnemiesInfo(class DataMap& MData, class User& ZSF)
{
  std::string szStatut = "";
  class SFRect	rc;
  class SFColor	colors;
  char		buffer[9];

  rc.setSFRect(22+30,
      	       180+30-ZSF.This_animal()->Vie*32/200,
	       22+32,
	       180+30);
  colors.setSFColor(0, 204, 0);
  if (ZSF.This_animal()->Vie > 50)
    SFTextColor(250,250,250);			  //blanc
  else
    SFTextColor(248,11,4);				  //rouge
  sprintf(buffer,"%d", static_cast<Cafard *>(ZSF.This_animal())->Bouclier);
  SFDisplay(28, 214, buffer);
  SFTextColor(248,11, 4);
  SFDisplay(28, 228, "agressivité");

  sprintf(buffer,"%d", static_cast<Cafard *>(ZSF.This_animal())->Agressif);
  SFTextColor(248,11,4);
  SFDisplay(78, 228,buffer);

  SFTextColor(248,11,4);
  SFDisplay(28, 228+14, "mandibule");

  sprintf(buffer,"%d", static_cast<Cafard *>(ZSF.This_animal())->Mandibule);
  SFDisplay(78, 228+14, buffer);

  sprintf(buffer,"%d",ZSF.This_animal()->Numero);
  SFDisplay(78, 228+28, buffer);

  if (static_cast<Cafard *>(ZSF.this_animal)->bDefense == true)
    SFDisplay(78, 228+28, "Défense");

  switch(ZSF.This_animal()->Esp)
  {
    case CAFARD:
      szStatut = "Cafard";break;
    default:
      szStatut = "Inconnu";break;
  }
  SFTextColor(248, 236, 24);
  SFDisplay(56, 184, szStatut);
}

static void
DisplayAntsInfo(class DataMap& MData, class User& ZSF)
{
  class SFRect	rc;
  int		nourri;
  class SFColor	colors;
  std::string	szStatut;
  char		buffer[9];

  colors.setSFColor(0, 204, 0);
  rc.setSFRect(22+30,180+30-ZSF.This_animal()->Vie*32/200
      ,22+32,180+30);
  SFDrawDCRectangle(rc, colors);
  nourri = ZSF.This_animal()->Nourriture;
  sprintf(buffer,"%d",nourri);
  if(nourri > 100)
    SFTextColor(250,250,250);			  //blanc
  else
    SFTextColor(248,11,4);				  //rouge
  SFDisplay(28, 214,buffer);
  SFTextColor(248,11,4);
  SFDisplay(28, 228,"loquacité");

  sprintf(buffer,"%d",static_cast<Fourmi *>(ZSF.This_animal())->Loquace());
  SFTextColor(248,11,4);
  SFDisplay(78, 228,buffer);

  SFTextColor(248,11,4);
  SFDisplay(28, 228+14, "mandibule");

  sprintf(buffer,"%d",static_cast<Fourmi *>(ZSF.This_animal())->Taille_mandibule);
  SFDisplay(78, 228+14, buffer);

  sprintf(buffer,"%d",ZSF.This_animal()->Numero);	  //Numero
  SFDisplay(70, 200, buffer);

  SFDisplay(28, 228+42, "Clan");			  //Clan
  for(int cll=0; cll< MData.nb_clan ;cll++)
  {
    if (ZSF.This_animal()->Clan==les_clans[cll])
    {
      sprintf(buffer,"%d",cll);
      SFDisplay(78, 228+42, buffer);
      break;
    }
  }

#ifndef RELEASE
  SFDisplay(28, 228+56, "wait");			  //wait
  sprintf(buffer,"%d",static_cast<Fourmi *>(ZSF.This_animal())->Wait);
  SFDisplay(78, 228+56, buffer);
#endif

  SFDisplay(28, 228+70, "Age");			  //wait
  sprintf(buffer,"%d",ZSF.This_animal()->Age);
  SFDisplay(78, 228+70, buffer);

#ifndef RELEASE
  if (static_cast<Fourmi *>(ZSF.This_animal())->bUnder() == true)
    SFDisplay(78, 228+28, "dessous");
  if (static_cast<Fourmi *>(ZSF.This_animal())->bUnder() == false)
    SFDisplay(78, 228+28, "dessus");
#endif
  
  switch (static_cast<Fourmi *>(ZSF.This_animal())->Statut())
  {
    case OUVRIER:
      szStatut = "Ouvrier";break;
    case SENTINELLE:
      szStatut = "Sentinelle";break;
    case REINE:
      szStatut = "Reine";break;
    case CITERNE:
      szStatut = "Citerne";break;
    case LARVE:
      szStatut = "Larve";
      SFDisplay(28,228+56,"Gestation");
      sprintf(buffer,"%d", static_cast<Fourmi *>(ZSF.This_animal())->LastPonte);
      SFDisplay(78,228+56,buffer);
      break;
    default:
      break;
  }
  SFTextColor(248,236,24);
  SFDisplay(56, 184,szStatut);
}

static void
DisplayIndividualInfo(class DataMap& MData, class User& ZSF)
{
  if (ZSF.bInfo)
    DisplayAntsInfo(MData, ZSF);
  else
    if (ZSF.bInfo_enn)
      DisplayEnnemiesInfo(MData, ZSF);
}

void
DisplayInfo(class DataMap &MData, class User &ZSF)
{
  SFRect 	rc;
  char		buffer[5];

  rc.setSFRect(0, 0, 160, 600);
  SFDrawSurface(0,0, rc);
  rc.setSFRect(160, 576, 800, 600);
  SFDrawSurface(160, 600-24, rc);

  DisplayIcon(MData, ZSF);
  DisplayGeneralInfo(MData, ZSF);
  DisplayIndividualInfo(MData, ZSF);
  if(ZSF.bStats)
  {
    SFTextColor(0,236,254);
    SFDisplay(710,500, "Ouvrières");
    SFDisplay(710,514, "Sentinelles");
    SFDisplay(710,528, "Citernes");
    //	SFDisplay(730,542, "Cafards");

    sprintf(buffer,"%d",ZSF.This_clan()->NbOuvrier);
    SFDisplay(780, 500, buffer);
    sprintf(buffer,"%d",ZSF.This_clan()->NbSentinelle);
    SFDisplay(780, 514, buffer);
    sprintf(buffer,"%d",ZSF.This_clan()->NbCiterne);
    SFDisplay(780, 528, buffer);
    //	sprintf(NbCafard,buffer2,10);
    //	SFDisplay 780, 542, buffer2);

  }
}
