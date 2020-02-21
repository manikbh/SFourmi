/***************************************************************************
 *
 *	Super Fourmi ou la vie artificielle
 *
 *  T&T Copyrights 17.02.2000.
 *
 ***************************************************************************/
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jpeglib.h>
#include <jerror.h>
}
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

//Les includes de SDL sont deja inclus dans SFourmis.h

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"
#include "IOsf.h"

#define TMPPATH "/tmp/FourmiLog.txt"

class DataMap	MData;
class User	ZSF;

unsigned char jpgimage[256][256]; /* l'image du terrain */

extern "C" {
void loadJpegImage(char *filename)
{
  FILE *file;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  unsigned char *im=(unsigned char *)jpgimage,*ligne;
  
  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_decompress(&cinfo);

  /* On met en place une image par defaut si filename=NULL*/
  if (filename==NULL){
    filename =  (char*) malloc(128);
    strcpy(filename,"terrain.jpg");
  }


  if (!(file=fopen(filename,"rb"))) {
    fprintf(stderr,"Erreur : impossible d'ouvrir %s\n",filename);
    exit(1);
  }

  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);
  if ((cinfo.image_width!=256) || (cinfo.image_height!=256)) {
    fprintf(stderr,"Erreur : l'image doit etre de taille 256x256\n");
    exit(1);
  }

  if (cinfo.out_color_space!=JCS_GRAYSCALE){
    fprintf(stderr,"Error : l'image doit etre en niveaux de gris\n");
    exit(1);
  }
  jpeg_start_decompress(&cinfo);

  while (cinfo.output_scanline<256){

    ligne=im+256*cinfo.output_scanline;
    jpeg_read_scanlines(&cinfo,&ligne,1);
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
}
}

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

void NewMap ()
{
  std::cerr << "New Map created ..." << std::endl;
  SDEBUG(W2, "Initialisation normale");

  SDEBUG(W0, "Chargement carte en cours");
  loadJpegImage("/usr/local/share/SFourmi/images/terrain.jpg");
  SDEBUG(W0, "Chargement carte fait");
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
  MData.LoadData("sfourmi.ini");
  if (argc <= 1)
    std::cerr << "use: SFourmi [map]" << std::endl;
  else
  {
    MData.charger = true;
    MData.loadfile = argv[1];
  }

  ZSF.path = SFOURMI_DATADIR;
  SDEBUG(W0,"Chargement carte");
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
  SDEBUG(W0,"Initialisation " GRAPH);
  glutInit(&argc, argv);
  GraphX_Init();
  s_ini(1,"-> " GRAPH " Initialisé<-");


  SFDrawMode (DDBLTFAST_NOCOLORKEY|DDBLTFAST_WAIT);
  while (ZSF.Counter() < 2^32)
  {
    if (SDL_PollEvent(evenement))
      process_events(evenement);
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
	  UpdateFrame(MData.fps);
	  ZSF.IncCounter();
	  GarbageLog(100);
	  break;
      }
    }
    else
    {
      SDL_WaitEvent(evenement); //Patiente 100 ms
      process_events(evenement);
      ZSF.MoveRoom();
      UpdateFrame(1);
    }
  }
  return(0);
}
