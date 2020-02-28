//
// DataMap.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Wed Jul 10 21:57:31 2002 Ghost in the Shell
// Last update Tue Oct 22 21:42:00 2002 Ghost in the Shell
//

// General Class used as a global environment variable
// DataMap includes the initialized paramaters and broadly speaking
// everything related to the entire scope.
// It contains the Screen var that indicates in which section the window is
// But it does not include the user's handle, flags and action which are
// located in the Class User.

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cassert>
#include <filesystem>
#include "Wonders.h"
#include "SFourmis.h"
#include "DataMap.h"
#define MAX_DATA	13

char	*mydata[MAX_DATA] = {"Length", "Height", "nWorkers", "nGuards", "nTanks",
    "Meat_density", "Fps", "Max_meat", "nClan", "Max_ennemies", "Ants_reborn",
    "Ennemies_reborn","LoadFile"};

DataMap::DataMap()
{
  // Defaults values

  setScreen(SFINTERFACE);
  length = 200;
  height = 200;
  nb_workers = 10;
  nb_guards = 10;
  nb_tanks = 10;
  max_ants = MAX_I(400, nb_workers + nb_guards + nb_tanks);
  meat_density = 3;
  fps = 1;
  max_meat = 4000;
  nb_clan = 3;
  max_ennemies = 25;
  ants_reborn = 500;
  ennemies_reborn = 1000;
  nb_ennemies = 20;
  charger=false;
}

DataMap::~DataMap()
{
}

void
DataMap::CheckMapParameters(int var, char *cmdline)
{
  switch (var)
  {
    case 0:
      length = atoi(cmdline);
      break;
    case 1:
      height = atoi(cmdline);
      break;
    case 2:
      nb_workers = atoi(cmdline);
      break;
    case 3:
      nb_guards = atoi(cmdline);
      break;
    case 4:
      nb_tanks = atoi(cmdline);
      break;
    case 5:
      meat_density = atoi(cmdline);
      break;
    case 6:
      fps = atoi(cmdline);
      break;
    case 7:
      max_meat = atoi(cmdline);
      break;
    case 8:
      nb_clan = atoi(cmdline);
      break;
    case 9:
      max_ennemies = atoi(cmdline);
      break;
    case 10:
      ants_reborn = atoi(cmdline);
      break;
    case 11:
      ennemies_reborn = atoi(cmdline);
      break;
    case 12:
      SDEBUG (W0, "Loading Map detected...");
      charger = strncmp ("none", cmdline, 4) == 0 ? false : true;
      loadfile = new char[strlen (cmdline)];
      strncpy (loadfile, cmdline + 1, strlen (cmdline) - 1);
      loadfile[strlen (cmdline) - 1] = '\0';
      break;
    default:
      std::cerr << "Parse error in initialization file" << std::endl;
      break;
  }
}

void
DataMap::LoadData(std::string file)
{
  std::filesystem::path inifile;
  ifstream	sf_ini;
  char		cmdline[80];

  char          *HOME;
  assert((HOME = getenv("HOME")) != 0);
  if(!HOME){
    std::cerr << "Could not get HOME environment variable -> cannot find config file !\n";
  }
  else
  {
    inifile = HOME;
    inifile /= file;
    sf_ini.open (inifile);
    SDEBUG (W0, inifile);
  }

  if (sf_ini.is_open() == 0)
  {
    SDEBUG (W0, "Could not open the file sfourmi.ini");
    std::cerr << "File " << inifile << " is missing. Please read the INSTALL section in /usr/doc/SFourmi/README." << std::endl;
    std::cerr << "Default settings used instead ..." << std::endl;
  }
  for (int i = 0; sf_ini.get(cmdline[i]) && (cmdline[i] != EOF); ++i)
    if (cmdline[i] == '\n')
    {
      cmdline[i] = '\0';
      i = -1;
      if ((cmdline[0] != '#') && (isspace(cmdline[0]) != true) && (cmdline[0] != 0))
      {
	bool	found = false;

	for (int j = 0; j < MAX_DATA; ++j)
	  if (strncmp(mydata[j], cmdline, strlen(mydata[j])) == 0)
	  {
	    found = true;
	    CheckMapParameters(j, cmdline + strlen(mydata[j]));
	  }
	  SDEBUG (W0, cmdline);
	if (found == false)
	{
	  cerr << "This is not a valid option " << cmdline << endl;
	  SDEBUG (W0, "Invalid option:");
	}
      }
    }
  max_ants = MAX_I(400, nb_workers + nb_guards + nb_tanks);
}

void DataMap::Serialize(FILE *f, bool sauv)
{
  if (sauv)		// Sauvegarde
  {
    fwrite(&length,sizeof(int),1,f);
    fwrite(&height,sizeof(int),1,f);
    fwrite(&nb_workers,sizeof(int),1,f);
    fwrite(&nb_guards,sizeof(int),1,f);
    fwrite(&nb_tanks,sizeof(int),1,f);
    fwrite(&meat_density,sizeof(int),1,f);
    fwrite(&fps,sizeof(int),1,f);
    fwrite(&max_meat,sizeof(int),1,f);
    fwrite(&nb_clan,sizeof(int),1,f);
    fwrite(&ants_reborn,sizeof(int),1,f);
    fwrite(&max_ants,sizeof(int),1,f);
    fwrite(&max_ennemies,sizeof(int),1,f);
    /*Comptage des ennemis vivants*/
    int kh=0;
    for(int kk=0;kk<MData.max_ennemies;kk++)
      if(les_ennemis[kk]!=NULL)
	kh++;
    nb_ennemies=kh;
    fwrite(&nb_ennemies,sizeof(int),1,f);
    fwrite(&ennemies_reborn,sizeof(int),1,f);
    fwrite(&screen,sizeof(enum e_screen),1,f);
  }else{
    fread(&length,sizeof(int),1,f);
    fread(&height,sizeof(int),1,f);
    fread(&nb_workers,sizeof(int),1,f);
    fread(&nb_guards,sizeof(int),1,f);
    fread(&nb_tanks,sizeof(int),1,f);
    fread(&meat_density,sizeof(int),1,f);
    fread(&fps,sizeof(int),1,f);
    fread(&max_meat,sizeof(int),1,f);
    fread(&nb_clan,sizeof(int),1,f);
    fread(&ants_reborn,sizeof(int),1,f);
    fread(&max_ants,sizeof(int),1,f);
    fread(&max_ennemies,sizeof(int),1,f);
    fread(&nb_ennemies,sizeof(int),1,f);
    fread(&ennemies_reborn,sizeof(int),1,f);
    fread(&screen,sizeof(enum e_screen),1,f);
  }
}
