/***************************************************************************
 *
 *	Super Fourmi ou la vie artificielle
 *
 *  T&T Copyrights 17.02.2000.
 *
 ***************************************************************************/

#define NAME "SFourmi"
#define TITLE "Les Fourmis en action !"
#define PACKAGE "SFourmi"
#define VERSION __DATE__

#include "SFourmis.h"
#include "Global.h"

#include "Init.h"

#include "CommonEnd.h"

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"
#include "IOsf.h"

#define TMPPATH "/tmp/FourmiLog.txt"

// Global variables
DataMap MData;
User ZSF;


/**
 * Empty the log if the counter reaches tr
 */
int GarbageLog(int tr)
{
  if ((ZSF.Counter() % tr) == 0)
  {
    Log.close();
    Log.open(TMPPATH);
  }
  return 0;
}

/**
 * Return index of the max value in a 4-bytes array
 */
BYTE Max(BYTE c[4])
{
  int res = 0;

  if (c[1] > c[0]) res = 1;
  if (c[2] > c[res]) res = 2;
  if (c[3] > c[res]) res = 3;
  return res;
}

/**
 * Create a new map
 */
void NewMap ()
{
  std::cerr << "New Map created ..." << std::endl;
  SDEBUG(W2, "Initialisation normale");
  ZSF.Initialize(MData);
  ZSF.bPause = false;
  ZSF.setThis_room(terre);
}


int main (int argc, char *argv[])
{
  Log.open (TMPPATH);
  Log << "Version :" << VERSION << endl;
  Log << "Date de compilation:" << __DATE__<<endl;
  Log << "Heure de compilation:" << __TIME__<<endl;
  cerr << PACKAGE << " (" << VERSION << ") " << "by SF team" << endl;
  // Load config file
  MData.LoadData("sfourmi.ini");
  if (argc <= 1)
    std::cerr << "use: SFourmi [map]" << std::endl;
  else
  {
    MData.charger = true;
    MData.loadfile = argv[1];
  }

  ZSF.path = "./";

  SDEBUG(W0,"Initialisation " GRAPH);
  GraphX_Init();
  SDEBUG(W0,"SDL initialisé");

  // Load map or create a new one
  if (MData.charger)
  {
    if (!Charg_terrain(MData.loadfile))
    {
      std::cerr << "Error when loading map : " << MData.charger << std::endl;
      NewMap ();
    }
    else
      std::cerr << "Loading of " << MData.loadfile << " : OK" << std::endl;
  }
  else
    NewMap ();

  bActive = true;
    
  while (true)
  {

    if (SDL_PollEvent(evenement))
      AnalyseEvent(evenement);
    else if((bActive) && (!ZSF.bPause))
    {
      switch (MData.Screen())
      {
	case SFINTERFACE:
	  StartInterface();
	  break;
	case SFGAME:
	  Process_va();
	  ZSF.MoveRoom ();
	  updateFrame(MData.fps);
	  ZSF.IncCounter();
	  GarbageLog(1000);
	  break;
      }
    }
    else
    {
      SDL_WaitEvent(evenement); //Patiente 100 ms
      AnalyseEvent(evenement);
      ZSF.MoveRoom();
      updateFrame(1);
    }
  }

  return(0);
}
