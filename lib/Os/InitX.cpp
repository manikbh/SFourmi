//
// InitX.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Mon Feb  4 13:40:49 2002 Ghost in the Shell
// Last update Thu Nov 27 12:46:09 2003 Ghost in the Shell
//

#include "SFourmis.h"
#include "config.h"
#ifdef WIN32
# include "WinInterface.h"
#endif

#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"
#include "../../images/All.xpm"
#include "../../images/Icon1.xpm"

float Cos[360], Sin[360], Tan[360];
int repere = 0,
  terrain = 0,
  normales = 0,
  circle = 0,
  pheromon = 0,
  cactus = 0,
  lampes=0; /* Identifiants des listes d'affichage */

static void calcCosSinTable()
{
  int i;
  for (i=0;i<360;i++){
    Sin[i]=sinf(i/360.0*6.283185);
    Cos[i]=cosf(i/360.0*6.283185);
    if ((i%90))
      Tan[i]=tanf(i/360.0*6.283185);
  }
}


#ifdef SF_SDL
bool GraphX_Init(void)
{
  const SDL_VideoInfo* info = 0;
  
  if (SDL_Init(/*SDL_INIT_AUDIO|*/SDL_INIT_VIDEO)<0)
  {
    SDEBUG(W0, "Echec de l'initialisation de SDL !");
    SDEBUG(W0, SDL_GetError());
    exit(0);
  }
  //Nettoyer SDL a la fin de l'execution
  atexit(SDL_Quit);
  
  // Let's get some video information.
  info = SDL_GetVideoInfo( );

  if( !info ) {
    // This should probably never happen.
    fprintf( stderr, "Video query failed: %s\n",
	SDL_GetError( ) );
    SDL_Quit();
  }

  //idem pour la librairie des polices
  if (TTF_Init()<0){
    SDEBUG(W0, "Echec de l'initialisation de TTF !");
    exit(1);
  }	
  atexit(TTF_Quit);


  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  
  //S'attacher a l'ecran en 800,600, le bpp du systeme (0), acceleration hardware et double buffer
  if (!(screen = SDL_SetVideoMode(800,600,0,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_OPENGL))){
  /* if (!(screen = SDL_SetVideoMode(800,600,0,SDL_HWSURFACE|SDL_DOUBLEBUF))){ */
    SDEBUG(W0, "Echec de l'initialisation du mode video !");
    SDL_Quit();
  }

  SDEBUG(W0, "Opengl opening ...");
  setup_opengl(800, 600);
  SDEBUG(W0, "Opengl set !");
  SDEBUG(W0, "Lanscape, Lights  generator ...");
  //TODO 2020 Adding glutInit
  
  creeRepere();
  creeLampes();
  creeTerrain();
  calcCosSinTable();
  SDEBUG(W0, "PASSED" << std::endl);
  std::cerr << "PASSED" << std::endl;

  //  image = IMG_Load("/usr/local/share/SFourmi/images/All.bmp");
  image = IMG_ReadXPMFromArray (All_xpm);
  if(image == NULL){
    SDEBUG(W0, "Impossible de charger l'image !");
    if(!fopen("/usr/local/share/SFourmi/images/All.bmp","r"))
    {SDEBUG(W0, "Image inaccessible !");}
    else
    {SDEBUG(W0, "Image accessible !");}
    exit(1);
  }
  SDL_SetColorKey(image, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(image->format, 4, 4, 4));
  SFRect sfr(0,0,800,600);
  /* SFDrawSurface(0,0,sfr); */
  std::cerr << "PASSED2" << std::endl;

  if(! (SFfont=TTF_OpenFont("/usr/local/share/SFourmi/images/font.ttf", 10))){//On charge la police taille 10pt
    SDEBUG(W0, "Impossible de charger la police !");
    SDEBUG(W0, SFOURMI_DATADIR "/images/font.ttf");
    exit(1);
  }
  std::cerr << "PASSED" << std::endl;

  TTF_SetFontStyle(SFfont, TTF_STYLE_NORMAL);
  evenement = new SDL_Event;		
  return true;
#endif
}

#ifdef SF_SDL

void setup_opengl( int width, int height )
{
  float ratio = (float) width / (float) height;
  glShadeModel( GL_SMOOTH );
  gluPerspective( 45.0, ratio, 0.1, 20.0 );
  glClearColor(0.5, 0.5, 0.5, 1.0);

  glFogi(GL_FOG_MODE, fogMode[fogfilter]); // Fog Mode
  glFogfv(GL_FOG_COLOR, fogColor); // Set Fog Color
  glFogf(GL_FOG_DENSITY, 0.05f); // How Dense Will The Fog Be
  glHint(GL_FOG_HINT, GL_DONT_CARE); // Fog Hint Value
  glFogf(GL_FOG_START, 1.0f); // Fog Start Depth
  glFogf(GL_FOG_END, 0.0f); // Fog End Depth
  glEnable(GL_FOG); // Enables GL_FOG
  
  glClearDepth(6.0f); // Depth Buffer Setup
  /* glDisable(GL_DEPTH_TEST); // Disables Depth Testing */
  /* glEnable(GL_BLEND); // Enable Blending */
  /* glBlendFunc(GL_SRC_ALPHA,GL_ONE); // Type Of Blending To Perform */
  /* glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST); // Really Nice Perspective Calculations */
  /* glHint(GL_POINT_SMOOTH_HINT,GL_NICEST); // Really Nice Point Smoothing */
  

  glEnable(GL_DEPTH_TEST);
  modePlein = 1;
  if (modePlein)
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glCullFace(GL_BACK);
  if (faceArriere)
    glDisable(GL_CULL_FACE);
  else
    glEnable(GL_CULL_FACE);

  /* Mise en place de la perspective */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();  
  gluPerspective(45.0,1.0,0.1,20.0);
  glMatrixMode(GL_MODELVIEW); 

  /* Parametrage de l'éclairage */
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,L0dif);
  glLightfv(GL_LIGHT0,GL_SPECULAR,L0dif);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,L1dif);
  glLightfv(GL_LIGHT1,GL_SPECULAR,L1dif); 

}

/* Creation de la liste d'affichage pour le repere */
void creeRepere()
{
   repere=glGenLists(1);
  glNewList(repere,GL_COMPILE);
  glDisable(GL_LIGHTING);
  glLineWidth(2.0);
    glBegin(GL_LINES);
      glColor3f(1.0,0.0,0.0);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.3,0.0,0.0);
      glColor3f(0.0,1.0,0.0);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,0.3,0.0);
      glColor3f(0.0,0.0,1.0);
      glVertex3f(0.0,0.0,0.0);
      glVertex3f(0.0,0.0,0.3);
    glEnd();
    glEnable(GL_LIGHTING);
  glEndList();
} 

/* Creation de la liste d'afffichage pour le terrain */
void creeTerrain()
{
  int i,j;
  float pas=20.0/nbSubdiv;
  vertex *T;
  vertex *P1,*P2,*P3,*P4;
  vertex V1,V2,V3;
  float incx,incy,incz,norme;
  T=(vertex *)malloc((nbSubdiv+1)*(nbSubdiv+1)*sizeof(vertex));

  /* Initialisation des normales */
  for (i=0;i<=nbSubdiv;i++)
    for (j=0;j<=nbSubdiv;j++) {
      T[i*(nbSubdiv+1)+j].nx=0.0;
      T[i*(nbSubdiv+1)+j].ny=0.0;
      T[i*(nbSubdiv+1)+j].nz=0.0;
    }
  /* remplissage du tableau Tvertex */
  for (i=0;i<=nbSubdiv;i++)
    for (j=0;j<=nbSubdiv;j++) {
      T[i*(nbSubdiv+1)+j].x=-10.0+i*pas;
      T[i*(nbSubdiv+1)+j].y=-10.0+j*pas;
      T[i*(nbSubdiv+1)+j].z=elevation(i,j);
    }
  for (i=0;i<nbSubdiv;i++)
    for (j=0;j<nbSubdiv;j++) {
      P1=&T[i*(nbSubdiv+1)+j];
      P2=&T[(i+1)*(nbSubdiv+1)+j];
      P3=&T[(i+1)*(nbSubdiv+1)+j+1];
      P4=&T[i*(nbSubdiv+1)+j+1];
      
      V1.x=P2->x-P1->x; V1.y=P2->y-P1->y; V1.z=P2->z-P1->z;
      V2.x=P3->x-P1->x; V2.y=P3->y-P1->y; V2.z=P3->z-P1->z;
      V3.x=P4->x-P1->x; V3.y=P4->y-P1->y; V3.z=P4->z-P1->z;

      incx=V2.y*V1.z-V1.y*V2.z;
      incy=V2.z*V1.x-V1.z*V2.x;
      incz=V2.x*V1.y-V1.x*V2.y;
      norme=sqrt(incx*incx+incy*incy+incz*incz);
      incx/=norme; incy/=norme; incz/=norme;
      P1->nx-=incx; P1->ny-=incy; P1->nz-=incz;
      P2->nx-=incx; P2->ny-=incy; P2->nz-=incz;
      P3->nx-=incx; P3->ny-=incy; P3->nz-=incz;
      

      incx=V3.y*V2.z-V2.y*V3.z;
      incy=V3.z*V2.x-V2.z*V3.x;
      incz=V3.x-V2.y-V2.x*V3.y;
      P1->nx-=incx; P1->ny-=incy; P1->nz-=incz;
      P3->nx-=incx; P3->ny-=incy; P3->nz-=incz;
      P4->nx-=incx; P4->ny-=incy; P4->nz-=incz;
    }
  
  /* normalisation des normales */
  for (i=0;i<=nbSubdiv;i++)
    for (j=0;j<=nbSubdiv;j++) {
      P1=&T[i*(nbSubdiv+1)+j];
      norme=sqrt(P1->nx*P1->nx+P1->ny*P1->ny+P1->nz*P1->nz);
      P1->nx/=norme;
      P1->ny/=norme;
      P1->nz/=norme;
    }
  
  /* Liste pour l'objet terrain */
  if (glIsList(terrain))
    glDeleteLists(terrain,1);
  terrain=glGenLists(1);
  glNewList(terrain,GL_COMPILE);
  /* Paramétrage du matériau */
  /* glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Mspec); */
  /* glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Mshiny); */

  glColor3f(0.1,1.0,0.0);
  glLineWidth(1.0);
  for (i=0;i<nbSubdiv;i++)
    for (j=0;j<nbSubdiv;j++) {
      if (T[i*(nbSubdiv+1)+j].z > PROF + 0.2)
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_grass);
      else
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_sand);
	
      glBegin(GL_TRIANGLES);
      /* triangle 1 */
      glEdgeFlag(TRUE);
      drawVertex(i,j,T);
      drawVertex(i+1,j,T);
      if (!areteTransv)
	glEdgeFlag(FALSE);
      drawVertex(i+1,j+1,T);

      /*triangle 2 */
      drawVertex(i,j,T);
      if (!areteTransv)
	glEdgeFlag(TRUE);
      drawVertex(i+1,j+1,T);
      drawVertex(i,j+1,T);
      glEnd();
    }
  glEndList();

  /* Generation de la liste d'affichage des normales*/
  creeNormales(T);
  
  /* Liberation de Tvertex */
  free(T);
  
  if (glIsList(circle))
    glDeleteLists(circle,1);
  circle=glGenLists(1);
  glNewList(circle,GL_COMPILE);
  glutSolidSphere(0.1f, 5, 5);
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_phero_rouge);
  glTranslatef(-0.05f, 0.03, 0.08);
  glutSolidSphere(0.02f, 5, 5);
  glTranslatef(0.1f, 0.0f, 0.0f);
  glutSolidSphere(0.02f, 5, 5);
  glPopMatrix();
  glEndList();
  
  if (glIsList(cactus))
    glDeleteLists(cactus,1);
  cactus=glGenLists(1);
  glNewList(cactus,GL_COMPILE);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_grass);
  glutSolidSphere(0.1f, 5, 5);
  glPushMatrix();
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_sand);
  glTranslatef(-0.05f, 0.03, 0.08);
  glutSolidSphere(0.02f, 5, 5);
  glTranslatef(0.1f, 0.0f, 0.0f);
  glutSolidSphere(0.02f, 5, 5);
  glTranslatef(-0.05f, -0.08, -0.02);
  glutSolidSphere(0.02f, 5, 5);
  glPopMatrix();
  glEndList();
 
  if (glIsList(pheromon))
    glDeleteLists(pheromon,1);
  pheromon=glGenLists(1);
  glNewList(pheromon,GL_COMPILE);
  glutSolidSphere(0.01f, 5, 5);
  glEndList();
}


/* Cree la liste d'affichage pour les normales */
void creeNormales(vertex *T)
{
  int i,j;
  vertex *P;
  if (glIsList(normales))
    glDeleteLists(normales,1);
  normales=glGenLists(1);
  glNewList(normales,GL_COMPILE);
  glDisable(GL_LIGHTING);
  glLineWidth(1.0);
  glColor3f(1.0,1.0,1.0);
  glBegin(GL_LINES);
  for (i=0;i<=nbSubdiv;i++)
    for (j=0;j<=nbSubdiv;j++) {
      P=&T[i*(nbSubdiv+1)+j];
      glVertex3fv(&P->x);
      glVertex3f(P->x+ECHELLE_NORMALES*P->nx,
		 P->y+ECHELLE_NORMALES*P->ny,
		 P->z+ECHELLE_NORMALES*P->nz);
    }
  glEnd();
  glEnable(GL_LIGHTING);
  glEndList();
  
}

/* Creation de la Liste d'affichage pour les lampes */
void creeLampes()
{
  lampes=glGenLists(1);
  glNewList(lampes,GL_COMPILE);
  glDisable(GL_LIGHTING);
  glColor3f(1.0,1.0,1.0);
  glPointSize(6.0);
  glBegin(GL_POINTS);
  glVertex3fv(L0pos);
  glVertex3fv(L1pos);
  glEnd();
  glLineWidth(1.0);
  glBegin(GL_LINES);
  glVertex3fv(L0pos);
  glVertex3f(0.0,0.0,0.0);
  glVertex3fv(L1pos);
  glVertex3f(0.0,0.0,0.0);
  glEnd();
  glEnable(GL_LIGHTING);
  glEndList();
}

/* Affiche le sommet (i,j) du maillage */
void drawVertex(int i,int j,vertex *T)
{
  glNormal3fv(&(T[i*(nbSubdiv+1)+j].nx));
  glVertex3fv(&(T[i*(nbSubdiv+1)+j].x));
}

/* Calcul de la hauteur d'un point */
float elevation(int i,int j)
{
  /* int valeur=jpgimage[(int)((float)i/nbSubdiv*255)][(int)((float)j/nbSubdiv*255)]; */
  //int valeur=ZSF.This_room()->map[(int)(float(i)/float(nbSubdiv)*255.0)][(int)(float(j)/float(nbSubdiv)*255.0)].z;
  int valeur=ZSF.This_room()->map[i][j].z; // MANIK 2009 (car au-dessus avec i et j entre 0 et nbSubdiv, on arrive à 255 à chaque fois alors que c'est pas forcément aussi grand que la carte -> segfault
  return ((float)valeur/128.0-1.0)*echelleVert;
}
#endif

