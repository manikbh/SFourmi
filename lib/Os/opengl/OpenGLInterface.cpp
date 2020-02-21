//
// OpenGLInterface.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Wed Nov 26 03:08:21 2003 Ghost in the Shell
// Last update Thu Nov 27 13:25:40 2003 Ghost in the Shell
//



#include <string.h>
#include "Wonders.h"
#include "SFourmis.h"

#ifdef SF_SDL

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"
#include "IOsf.h"

#include "SDLInterface.h"
/* Variables globales */

unsigned char afficheRepere=TRUE; /* Affichage du repere */
unsigned char faceArriere=FALSE; /* Affichage des faces arrieres de polygones */
unsigned char areteTransv=FALSE; /* Affichage de l'arete transversale */
unsigned char afficheNormales=FALSE; /* Affichage des normales */
unsigned char afficheLampes=TRUE; /* Affichage des lampes */
unsigned char modePlein=TRUE; /* Affichage en mode plein ou fil de fer */
int nbSubdiv = NB_SUBDIV_INIT; /* Nombre de subdivisions du maillage */
float echelleVert=ECHELLE_VERT_INIT; /* echelle verticale du relief */
char b_gauche = 0,b_droit = 0; /* bouton de souris presse ? */
int theta=0,phi=330; /* Position de l'observateur */
int xprec,yprec; /* sauvegarde de la position de la souris */
float zview = 5;//DISTANCE_INIT; /* distance de l'observateur a l'origine */

float xcam = 0, ycam = 0;

/* Parametres d'éclairage */

// L0 is DIFFUSE and SPECULAR lamp
GLfloat L0pos[]={ 0.0,2.0,1.0};
GLfloat L0dif[]={ 1.0,1.0,0.0};
// L1 is DIFFUSE and SPECULAR lamp
GLfloat L1pos[]={ 2.0,2.0,5.0};
GLfloat L1dif[]={ 1.0,1.0,1.0};
GLfloat Mspec[]={0.5,0.5,0.5};

GLfloat Mspec_grass[]={0.0, 1.0, 0.0};
GLfloat Mspec_sand[]={1.0, 1.0, 0.25};
GLfloat Mspec_water[]={0.0, 0.0, 1.0};
/* GLfloat Mspec[]={0.0, 1.0, 0.0}; */
GLfloat Mshiny=50;


int gp = 1; // G Pressed? ( New )
GLuint filter; // Which Filter To Use
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR }; // Storage For Three Types Of Fog
GLuint fogfilter= 0; // Which Fog To Use
GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f}; // Fog Color



static GLboolean should_rotate = GL_TRUE;

int StartInterface()
{
  MData.setScreen(SFGAME);
  return(0);
}
static void handle_mouse_button_up(SDL_MouseButtonEvent* mbd)
{
  if (mbd->button == SDL_BUTTON_LEFT)
    b_gauche = 0;
  if (mbd->button == SDL_BUTTON_RIGHT)
    b_droit = 0;
}

static void handle_mouse_button_down(SDL_MouseButtonEvent* mbd)
{
  static int X, Y;
  X = mbd->x;
  Y = mbd->y;
  switch(MData.Screen())
  {
    case SFGAME:
      if(!((X < 160)||(Y > 600-24)))
      {
	ZSF.bMove = true;
	MouseXYD[0] = X;
	MouseXYD[1] = Y;
	if(ZSF.bPause)
	  UpdateFrame(1);
      }
      break;
    case SFINTERFACE:
      //      if((X>195)&&(Y>76)&&(X<418)&&(Y<114))
      //	tb=1;
      //      if((X>195)&&(Y>206)&&(X<418)&&(Y<242))
      //	tb=3;
      break;
  }
  if (mbd->button == SDL_BUTTON_LEFT)
  {
    b_gauche = 1;
    xprec = X;
    yprec = Y;
  }
  if (mbd->button == SDL_BUTTON_RIGHT)
  {
    b_droit = 1;
    yprec = Y;
  }
}

static void handle_mouse_motion(SDL_MouseMotionEvent* mev)
{
  /* si le bouton gauche est presse */
  if (b_gauche) {
    theta += mev->x - xprec;
    if (theta>=360)
      while (theta>=360)
	theta-=360;
    phi += mev->y - yprec;
    if (phi<0)
      while (phi<0)
	phi+=360;
    xprec = mev->x;
    yprec = mev->y;
    UpdateFrame(1);
  }

  /* si le bouton gauche est presse */
  if (b_droit) {
    zview +=((float)(mev->y - yprec))/10.0;
    if (zview < 0.1)
      zview = 0.1;
    if (zview > DISTANCE_MAX)
      zview = DISTANCE_MAX;
    UpdateFrame(1);
    yprec = mev->y;
  }
}

static void handle_key_down(SDL_keysym* keysym)
{
  switch(keysym->sym){
    case SDLK_RIGHT:
      ZSF.bGoRight=true;
      break;
    case SDLK_LEFT:
      ZSF.bGoLeft = true;
      break;
    case SDLK_DOWN:
      ZSF.bGoDown = true;
      break;
    case SDLK_UP:
      ZSF.bGoUp = true;
      break;
    default: break;
  }
}

static void handle_key_up( SDL_keysym* keysym )
{

  /* 
   *      * We're only interested if 'Esc' has
   *           * been presssed.
   *                *
   *                     * EXERCISE: 
   *                          * Handle the arrow keys and have that change the
   *                               * viewing position/angle.
   *                                    */
  switch( keysym->sym ) {
    case SDLK_ESCAPE:
      if(ZSF.bPause)
	ZSF.bPause =false;
      else
	exit(1);
      break;
    case SDLK_SPACE:
      should_rotate = !should_rotate;
      break;
    case SDLK_DOLLAR :
      nbSubdiv++;
      if (nbSubdiv>NB_SUBDIV_MAX)
	nbSubdiv=NB_SUBDIV_MAX;
      creeTerrain();
      UpdateFrame(1);
      break;
    case SDLK_ASTERISK : /* diminution du nombre de subdivisions*/
      nbSubdiv--;
      if (nbSubdiv<1)
	nbSubdiv=1;
      creeTerrain();
      UpdateFrame(1);
      break;
/*    case SDLK_p : //augmentation de l'echelle verticale 
      echelleVert+=0.02;
      if (echelleVert>ECHELLE_VERT_MAX)
	echelleVert=ECHELLE_VERT_MAX;
      creeTerrain();
      UpdateFrame(1)();
      break;
*/
    case SDLK_o : /* diminution de l'echelle verticale */
      echelleVert-=0.02;
      if (echelleVert<-ECHELLE_VERT_MAX)
	echelleVert=-ECHELLE_VERT_MAX;
      creeTerrain();
      UpdateFrame(1);
      break;
    case SDLK_r : /* UpdateFrame(1) du repere ON/OFF */
      afficheRepere=1-afficheRepere;
      UpdateFrame(1);
      break;
    case SDLK_c : /* UpdateFrame(1) des faces arrieres ON/OFF */
      faceArriere=1-faceArriere;
      if (faceArriere)
	glDisable(GL_CULL_FACE);
      else
	glEnable(GL_CULL_FACE);
      UpdateFrame(1);
      break;
/*    case SDLK_t: // UpdateFrame(1) des aretes transversales
      areteTransv=1-areteTransv;
      creeTerrain();
      UpdateFrame(1)();
      break;
*/
    case SDLK_n: /* UpdateFrame(1) des normales ON/OFF */
      afficheNormales=1-afficheNormales;
      UpdateFrame(1);
      break;
    case SDLK_l : /* UpdateFrame(1) des lampes ON/OFF */
      afficheLampes=1-afficheLampes;
      UpdateFrame(1);
      break;
    case SDLK_g :
      gp= 1 - gp; // gp Is Set To TRUE
      fogfilter+=1; // Increase fogfilter By One
      if (fogfilter>2) // Is fogfilter Greater Than 2?
	fogfilter=0; // If So, Set fogfilter To Zero
      glFogi (GL_FOG_MODE, fogMode[fogfilter]); // Fog Mode
      UpdateFrame(1);
      break;
    case SDLK_RIGHT:
      ZSF.bGoRight = false;
      break;
    case SDLK_LEFT:
      ZSF.bGoLeft = false;
      break;
    case SDLK_DOWN:
      ZSF.bGoDown = false;
      break;
    case SDLK_UP:
      ZSF.bGoUp = false;
      break;
    case SDLK_F1:
      cout << "touche F1\n";
      break;

    case SDLK_F2:
      ZSF.setThis_room((ZSF.This_room() == terre ?
	    ZSF.This_clan()->la_fourme[0] : terre));
      ZSF.column = 0;
      ZSF.line = 0;
      break;

    case SDLK_F3:				  //CENTRER sur la fourmilière
      ZSF.column = max(0,ZSF.This_clan()->Gate[0][0]-10);
      ZSF.line = max(0,ZSF.This_clan()->Gate[1][0]-9);
      ZSF.column = min(ZSF.column, ZSF.This_room()->size.largeur-20);
      ZSF.line = min( ZSF.line, ZSF.This_room()->size.hauteur-20);
      break;

    case SDLK_F4:				  //Mini carte
      ZSF.bSmall_map = !ZSF.bSmall_map;
      break;

    case SDLK_F5:				  //Centre sur la fourmi sélectionnée
      if(ZSF.bInfo)
      {
	ZSF.column = max(0,ZSF.This_animal()->Xc-10);
	ZSF.line = max(0,ZSF.This_animal()->Yc-9);
	ZSF.column = min(ZSF.column, ZSF.This_room()->size.largeur-20);
	ZSF.line = min( ZSF.line, ZSF.This_room()->size.hauteur-20);
      }
      else
      {
	if(ZSF.bInfo_enn)
	{
	  ZSF.column = max(0,ZSF.This_animal()->Xc-10);
	  ZSF.line = max(0,ZSF.This_animal()->Yc-9);
	  ZSF.column = min(ZSF.column, MData.length-20);
	  ZSF.line = min( ZSF.line, MData.height-20);
	}
      }
      break;

    case SDLK_F9:			  //Sauvegarde
      Sauv_terrain();
      break;

    case SDLK_F11:			  //Change le clan en cours (sens -)
      if(ZSF.This_clan()==les_clans[0])
	ZSF.setThis_clan(les_clans[MData.nb_clan-1]);
      else{
	for(int indice=0; indice<MData.nb_clan; indice++) //Parcours du tableau
	  if(ZSF.This_clan()==les_clans[indice]){
	    ZSF.setThis_clan(les_clans[indice-1]);
	    return;
	  }
	SDEBUG(W4,"Le clan courant est invalide !");//On ne devrait jamais arriver ici
      }
      break;

    case SDLK_F12:
      if(ZSF.This_clan()==les_clans[MData.nb_clan-1])
	ZSF.setThis_clan(les_clans[0]);
      else{
	for(int indice=0; indice<MData.nb_clan; indice++) //Parcours du tableau
	  if(ZSF.This_clan()==les_clans[indice]){
	    ZSF.setThis_clan(les_clans[indice+ 1]);
	    return;
	  }
	SDEBUG(W4,"Le clan courant est invalide !");//On ne devrait jamais arriver ici
      }
      break;

    case SDLK_p:				  //ZSF.bPause
      ZSF.bPause = !ZSF.bPause;
      break;
    case SDLK_s:				  //STATISTIQUES
      ZSF.bStats = !ZSF.bStats;
      break;
    case SDLK_t:				  //Active ou désactive l'UpdateFrame(1) principal
      ZSF.bScreen = !ZSF.bScreen;
      break;
      //case SDLK_PLUS:			  //Changement d'étage en sous-sol
      //cave_selec=min(les_clans[ZSF.This_clan()]->NBMAX_ROOM,cave_selec++);
      //break;
      //case SDLK_MINUS:
      //cave_selec=max(0,cave_selec--);
      //break;
    case SDLK_v:				  //Violent mode
      ZSF.bViolent = !ZSF.bViolent;
      break;
    case SDLK_i:
      ZSF.bInfo=!ZSF.bInfo;
      break;
    case SDLK_f:
      SDL_WM_ToggleFullScreen(screen);
      break;
    default:
      cout<<"Touche inconnue\n";
      break;
  }
}


void process_events(SDL_Event *evenement)
{
  switch(evenement->type)
  {

    case SDL_MOUSEMOTION:					  //Mouvement de la souris
      handle_mouse_motion(&(evenement->motion));
      break;

    case SDL_KEYUP :
      handle_key_up(&(evenement->key.keysym));
      break;
    case SDL_KEYDOWN :
      handle_key_down(&(evenement->key.keysym));
      break;     

    case SDL_QUIT://Message de  fermeture de la fenêtre
      exit(0); break;

    case SDL_MOUSEBUTTONDOWN :
      /* if(evenement->button.button==SDL_BUTTON_LEFT) */
	/* LButtonDown(evenement->button.x,evenement->button.y); */
      /* else if(evenement->button.button==SDL_BUTTON_RIGHT) */
	/* RButtonDown(evenement->button.x,evenement->button.y); */
      /* else */
	/* bActive = !bActive; */
      handle_mouse_button_down(&evenement->button);
      break;
    case SDL_MOUSEBUTTONUP:
      /* if(evenement->button.button==SDL_BUTTON_LEFT) */
	/* LButtonUp(evenement->button.x,evenement->button.y); */
      handle_mouse_button_up(&evenement->button);
      break;
    default:;
  }
}

void LButtonDown(int X ,int Y)
{
  if(!((X < 160)||(Y > 600-24)))
  {
    ZSF.bDown = true;
    MouseXY[0] = X;
    MouseXY[1] = Y;
  }
}


void LButtonUp(int X ,int Y)
{
  unsigned	rc1Y, rc1X;
  char BufX[5], BufY[5];
  int PosX, PosY, PosXD, PosYD;

  bool Selectbcp = ((ZSF.bDown)&&(ZSF.bMove));
  ZSF.bDown =false;

  ZSF.bMove = false;
  switch(MData.Screen())
  {
    case SFGAME:

      if((X < 160)||(Y > 600-24))
      {
	if((X>71)&&(X<105)&&(Y>504)&&(Y<535))
	  ZSF.ButtonMeat = true;
	else if(((X>XSMAP)&&(X<XSMAP+LSMAP)&&(Y>YSMAP)&&(Y<YSMAP+HSMAP))&&ZSF.bSmall_map)
	{
	  PosX = ZSF.This_room()->size.largeur*(X - 15)/LSMAP;
	  PosY = ZSF.This_room()->size.hauteur*(Y -386)/HSMAP;
	  ZSF.column = max(0,PosX-10);
	  ZSF.line = max(0,PosY-9);
	  ZSF.column = min(ZSF.column, ZSF.This_room()->size.largeur-20);
	  ZSF.line = min( ZSF.line, ZSF.This_room()->size.hauteur-18);
	}
      }
      else
      {
	rc1Y = 180;
	rc1X= 40;
	{
	  PosX = (X - 160)/32+ZSF.column;
	  PosY = (Y )/32+ZSF.line;

	  if(ZSF.ButtonMeat)
	  {
	    if((ZSF.This_room()->map[PosX][PosY].decor->genre == ERIEN))
	    {
	      ZSF.This_room()->map[PosX][PosY].decor->genre = VIANDE;
	      ZSF.This_room()->map[PosX][PosY].decor->etat = MData.max_meat;
	      //ZSF.ButtonMeat = FALSE;
	      UpdateFrame(1);
	    }
	    return;
	  }
	  if (Selectbcp)
	  {
	    PosXD = (MouseXY[0] - 160)/32+ZSF.column;
	    PosYD = (MouseXY[1] )/32+ZSF.line;
	  }
	  else
	  {
	    PosXD = PosX;
	    PosYD = PosY;
	  }
	  sprintf(BufX,"%d",PosX);
	  sprintf(BufY,"%d",PosY);
	  ZSF.bInfo = false;ZSF.bInfo_enn=false;
	  for (int i=min(PosX,PosXD);i<=max(PosX,PosXD);i++)
	  {
	    for (int j=min(PosY,PosYD);j<=max(PosY,PosYD);j++)
	    {

	      if (TEST_ANIMAL(i, j, ZSF.This_room()) != NULL)
	      {
		if (ZSF.This_room()->map[i][j].firstAnimal->Esp == FOURMI)
		{
		  ZSF.bInfo = true;
		  ZSF.setThis_animal(ZSF.This_room()->map[i][j].firstAnimal);
		  UpdateFrame(1);
		  goto plusloineheh;

		}
		//INFOS ENNEMIS
		if(ZSF.This_room()->map[i][j].firstAnimal->Esp !=FOURMI)
		{
		  ZSF.bInfo_enn=true;
		  ZSF.setThis_animal(ZSF.This_room()->map[i][j].firstAnimal);
		  UpdateFrame(1);
		  goto plusloineheh;
		}
	      }

	    }
	  }
plusloineheh:;

	}
      }
      break;
    case SFINTERFACE:
      if ((X>195)&&(Y>76)&&(X<418)&&(Y<114))
	MData.setScreen (SFGAME);
      if ((X>195)&&(Y>206)&&(X<418)&&(Y<242));

      break;
  }
}


void RButtonDown(int X ,int Y)
{
  if(ZSF.ButtonMeat) 
  {
    ZSF.ButtonMeat = false;
    return;
  }
}


#endif
