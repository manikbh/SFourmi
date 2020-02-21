/***************************************************************************
 *
 *	Super Fourmi ou la vie artificielle
 *
 *  T&T Copyrights 17.02.2000.
 *
 ***************************************************************************/

#define NAME "SFourmi"
#define TITLE "Les Fourmis en action !"

#include "SFourmis.h"
#include "config.h"
#include "Global.h"

#include "Init.h"

#include "CommonEnd.h"

#ifdef WIN32
# include "WinInterface.h"
#endif

#ifdef GTK_Linux
# include <gdk/gdk.h>
# include <gdk-pixbuf/gdk-pixbuf.h>
# include "GTKInterface.h"
#endif

//Les includes de SDL sont deja inclus dans SFourmis.h

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"
#include "IOsf.h"

#ifdef WIN32
#define TMPPATH "c:\\FourmiLog.txt"
#else
#define TMPPATH "/tmp/FourmiLog.txt"
#endif
class DataMap	MData;
class User	ZSF;

int GarbageLog(int tr)
{
  if ((ZSF.Counter() % tr) == 0)
  {
    Log.close();
    Log.open(TMPPATH);
  }
  return 0;
}

BYTE Max(BYTE c[4])
{
  int res = 0;

  if (c[1] > c[0]) res = 1;
  if (c[2] > c[res]) res = 2;
  if (c[3] > c[res]) res = 3;
  return res;
}
void
NewMap ()
{
  std::cerr << "New Map created ..." << std::endl;
  SDEBUG(W2, "Initialisation normale");
  ZSF.Initialize(MData);
  ZSF.bPause = false;
  ZSF.setThis_room(terre);
}
#ifdef WIN32
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main (int argc, char *argv[])
#endif
{
  Log.open (TMPPATH);
  Log << "Version :" << VERSION << endl;
  Log << "Date de compilation:" << __DATE__<<endl;
  Log << "Heure de compilation:" << __TIME__<<endl;
  cerr << PACKAGE << " (" << VERSION << ") " << "by SF team" << endl;  
  MData.LoadData("sfourmi.ini");
  if (argc <= 1)
    std::cerr << "use: SFourmi [map]" << std::endl;
  else
  {
    MData.charger = true;
    MData.loadfile = argv[1];
  }

  ZSF.path = SFOURMI_DATADIR;
#ifdef WIN32
  MSG msg;
  hPrevInstance = hPrevInstance;
  if (!doInit(hInstance, nCmdShow)) return FALSE;
  FontFactory();
#endif
//  gdk_init(&argc,&argv);
  SDEBUG(W0,"Initialisation " GRAPH);
  GraphX_Init();
  s_ini(1,"-> " GRAPH " Initialisé<-");

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

#ifndef WIN32
  bActive = true;
#endif
  SFDrawMode (DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
  while (ZSF.Counter() < 2^32)
  {
#ifdef WIN32
    if( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
    {
      if(!GetMessage(&msg, NULL, 0, 0))
	return msg.wParam;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
#endif
#ifdef GTK_Linux
    if (gdk_events_pending())
    {
      evenement = gdk_event_get();
      if (evenement)
      {
	AnalyseEvent(evenement);
	gdk_event_free(evenement);
      }
    }
#endif
#ifdef SF_SDL
    if (SDL_PollEvent(evenement))
      AnalyseEvent(evenement);
#endif
    else if((bActive) && (!ZSF.bPause))
    {
      switch (MData.Screen())
      {
	case SFINTERFACE:
	  StartInterface();
	  //ZSF.bPause=true;
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
#ifdef WIN32
    else
      WaitMessage();
#endif
#ifdef SF_SDL
    else
    {
      SDL_WaitEvent(evenement); //Patiente 100 ms
      AnalyseEvent(evenement);
      ZSF.MoveRoom();
      updateFrame(1);
    }
#endif
  }
#ifdef GTK_Linux
  GDKDestroy();
#endif
  return(0);
}
