///////////////////////////////////////////////////////////////////////////////
//
//	GTKInterface.h: Procédures d'interface de GTK
//
///////////////////////////////////////////////////////////////////////////////
#ifdef SF_SDL
#ifndef SDLInterface_H
# define SDLInterface_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string.h>
#include <jpeglib.h>
#include <jerror.h>


#define NB_SUBDIV_INIT 64
#define NB_SUBDIV_MAX 128
#define ECHELLE_VERT_INIT 0.9
#define ECHELLE_VERT_MAX 3.0
#define ECHELLE_NORMALES 0.1
#define DISTANCE_INIT 6.0
#define DISTANCE_MAX 15.0

#define WATER -0.7f
#define PROF WATER

#define OFFSET 0.02
/* Definition du type sommet */
typedef struct {
  float x;
  float y;
  float z;
  float nx;
  float ny;
  float nz;
  char nb;
} vertex;

/* Variables globales */

extern	unsigned char afficheRepere; /* Affichage du repere */
extern	unsigned char faceArriere; /* Affichage des faces arrieres de polygones */
extern	unsigned char areteTransv; /* Affichage de l'arete transversale */
extern	unsigned char afficheNormales; /* Affichage des normales */
extern	unsigned char afficheLampes; /* Affichage des lampes */
extern	unsigned char modePlein; /* Affichage en mode plein ou fil de fer */
extern	int repere,
  terrain,
  normales,
  circle,
  lampes,
  cactus,
  pheromon; /* Identifiants des listes d'affichage */
extern	int nbSubdiv; /* Nombre de subdivisions du maillage */
extern	float echelleVert; /* echelle verticale du relief */
extern	char b_gauche,b_droit; /* bouton de souris presse ? */
extern	int theta,phi; /* Position de l'observateur */
extern	int xprec,yprec; /* sauvegarde de la position de la souris */
extern	float zview; /* distance de l'observateur a l'origine */

extern	float xcam, ycam;
/* Parametres d'éclairage */
extern	GLfloat L0pos[];
extern	GLfloat L0dif[];
extern	GLfloat L1pos[];
extern	GLfloat L1dif[];
extern	GLfloat Mspec[];

extern	GLfloat Mspec_grass[];
extern	GLfloat Mspec_sand[];
extern	GLfloat Mspec_water[];
extern	GLfloat Mspec_cafard[];
extern	GLfloat Mspec_ouvrier[];
extern	GLfloat Mspec_soldat[];
extern	GLfloat Mspec_reine[];
extern	GLfloat Mspec_phero_rouge[];

extern	GLfloat Mshiny;

extern	int gp; // G Pressed? ( New )
extern	GLuint filter; // Which Filter To Use
extern	GLuint fogMode[]; // Storage For Three Types Of Fog
extern	GLuint fogfilter; // Which Fog To Use
extern	GLfloat fogColor[4]; // Fog Color


int StartInterface(void);
void process_events(SDL_Event *evenement);
void RButtonDown(int X ,int Y);

void LButtonUp(int X ,int Y);
void LButtonDown(int X ,int Y);
void creeRepere();
void creeTerrain();
float elevation(int i,int j);
void creeNormales(vertex *T);
void creeLampes();
void drawVertex(int i,int j,vertex *T);
void setup_opengl(int, int);
//Pour ignorer les constantes DirectX
#define DDBLTFAST_NOCOLORKEY 0
#define DDBLTFAST_WAIT	0
#define DDBLTFAST_SRCCOLORKEY 0

#endif
#endif
