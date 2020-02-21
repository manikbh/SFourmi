//
// UpdateFrame.cpp
//
// Made by (Ghost in the Shell)
// Login   <dodeskaden@Z>
// 
// Started on  Sat Feb  2 10:32:54 2002 Ghost in the Shell
// Last update Thu Nov 27 13:28:18 2003 Ghost in the Shell
//


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

#ifdef SF_SDL
//SDL : les includes sont deja dans SFourmis.h
GLfloat Mspec_cafard[]={0.0, 1.0, 0.0};
GLfloat Mspec_ouvrier[]={1.0, 1.0, 0.0};
GLfloat Mspec_soldat[]={0.50, 1.0, 1.0};
GLfloat Mspec_reine[]={1.0, 1.0, 1.0};
GLfloat Mspec_phero_rouge[]={1.0, 0.0, 0.0};
#endif

extern unsigned char jpgimage[256][256];

static void	DrawPheromon(int k, int j)
{
  static CCase	*tmpc;
  static int index;

  for(int m = 0; m < PHERO_SIZE; ++m)
  {
    tmpc = &ZSF.This_room()->map[k][j];
    if(tmpc->phero[m] != NULL){ //MANIK 2009 -> Ne devrait jamais arriver...
	    std::cerr << "Une phero nulle sur le terrain ("<<k<<","<< j<<")\n";
	    continue;
    }
    if ((tmpc->phero[m]->Genre) != NUL)
    {
      glPushMatrix();
      glTranslatef(k * 0.078 - 10.0, j * 0.078 - 10.0, ZSF.This_room()->map[k][j].z/128.0 - 1.0 * echelleVert);
      if (tmpc->phero[m]->Action() == OK)
      {
	if (tmpc->phero[m]->Genre == PCITERNE)
	    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_sand);
	else// if (tmpc->phero[m]->Genre == DANGER)
	    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_phero_rouge);
	glCallList(pheromon);
	/* glutSolidSphere(0.01f, 5, 5); */
	index = tmpc->phero[m]->GetPositionIndex();
	
      }
      glPopMatrix();
    }
  }
#if 0
  //Fin des phéromones
  //Entrées
  rc.setSFRect(0, 0, 0, 0);
  for (int cl = 0; cl < MData.nb_clan; ++cl)
    for (int hk = 0; hk < 1; hk++)
    {
      if (ZSF.This_room() != terre)
      {
	if ((k + ZSF.column == ZSF.This_room()->Gate[0][hk])
	    && (j + ZSF.line == ZSF.This_room()->Gate[1][hk]))
	{
	  rc.setSFRect(7*32,600,7*32+32,632);
	  SFDrawSurface(160+k*32,j*32, rc);
	}
      }
      else
	if ((k+ZSF.column == les_clans[cl]->Gate[0][hk])
	    && (j+ZSF.line == les_clans[cl]->Gate[1][hk]))
	{
	  rc.setSFRect(7*32,600,7*32+32,32+600);
	  SFDrawSurface(160+k*32,j*32,rc);
	}
    }							  //Entrées
#endif
}

static void	DrawMap(void)
{
  static class SFRect		rc;
  static int			i;

  for(int k = 0; k < 10 * 2; k++)
    for(int j = 0; j < 9 * 2; j++)
    {
      i = ZSF.This_room()->map[k+ZSF.column][j+ZSF.line].terrain;

      if (i < 10)
	rc.setSFRect(i*32, 0 + 600, i*32+ 32, 0 + 32 + 600);
      else
      {
	i -= 10;
	rc.setSFRect(9*32+i*32, 0 + 600, 9*32+i*32+ 32, 0 + 32 + 600);
      }
      SFDrawSurface(159+32*k, j * 32, rc);
    }
}

static void	DrawAnimal(int k, int j, int F)
{
  static int		Shift = 0;
  static class SFRect	rc;
  static CAnimal*	ani;
  static int		index = 0;

  if ((ani = TEST_ANIMAL(k, j, ZSF.This_room()))
      != NULL)
  {
    index = ani->GetPositionIndex();
    glPushMatrix();
    glTranslatef(k * 0.078 - 10.0, j * 0.078 - 10.0, ZSF.This_room()->map[k][j].z/128.0 - 1.0 * echelleVert);
    glRotatef(index, 0.0, 0.0, 1.0);
    switch (ani->Esp)
    {
      case FOURMI:
	/* if ((ZSF.This_animal() == ani) && (ZSF.bInfo)) */
	/* { */
	  /* rc.setSFRect(0, 7*32 + 600, 32, 7 * 32 + 632); */
	  /* SFDrawSurface(160 + k * 32, j * 32, rc); */
	/* } */
	switch (static_cast<Fourmi *>(ani)->Statut())
	{
	  case OUVRIER:
	    /* if ((static_cast<Fourmi *>(ani)->bParle()) ||  */
		/* (static_cast<Fourmi *>(ani)->bPorteObj())) */
	      /* Shift = 255; */
	    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_ouvrier);
	    break;
	  case SENTINELLE:
	    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_soldat);
	    break;

	  case CITERNE:
	    break;
	  case REINE:
	    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_reine);
	    break;
	  case LARVE:
	    break;
	  default:	  // PAR Defaut larve
	    break;
	} 
	break;
      case CAFARD:
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_cafard);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Mspec);
	//Tracer un cadre jaune
	/* if ((ZSF.This_animal() == ani) && (ZSF.bInfo_enn == true)) */
	/* { */
	  /* rc.setSFRect(0,7*32+600,32,7*32+632); */
	  /* SFDrawSurface(160+k*32,j*32, rc); */
	/* } */
	/* rc.setSFRect(index + 256,64+600, index + 256 + 32,64+632); */
	/* SFDrawSurface(160+32*k+GraphPos[F][0] */
	    /* ,32*j+GraphPos[F][1], rc); */
	break;
      default:
	break;
    }						  //fin de switch
    glCallList(circle);
    /* glutSolidSphere(0.1f, 5, 5); */
    glPopMatrix();
  }  //fin de if firstanimal != NULL
}

void	DrawOutset(int k, int j)
{

  /* if (ZSF.This_room()->map[k + ZSF.column][j + ZSF.line].decor->genre == VIANDE) */
  /* SDEBUG(W0, "DrawOutset"); */
  if (ZSF.This_room()->map[k][j].decor->genre == VIANDE)
  {
    glPushMatrix();
    glTranslatef(k * 0.078 - 10.0, j * 0.078 - 10.0, ZSF.This_room()->map[k][j].z/128.0 - 1.0 * echelleVert);
    /* glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_water); */
    glCallList(cactus);
    //glutSolidSphere(0.1f, 5, 5);
    glPopMatrix();
    /* if(ZSF.This_room()->map[k + ZSF.column][j + ZSF.line].decor->etat */
	/* < MData.max_meat / 3) */
      /* rc.setSFRect(0, 5 * 32 + 600, 32, 5 * 32 + 632); */
    /* else */
      /* if(ZSF.This_room()->map[k + ZSF.column][j + ZSF.line].decor->etat */
	  /* > 2 * MData.max_meat / 3) */
	/* rc.setSFRect(64,5*32+600,96,5*32+632); */
      /* else */
	/* rc.setSFRect(32,5*32+600,64,5*32+632); */
  }

#if 0
  if (ZSF.This_room()->map[k+ZSF.column][j+ZSF.line].decor->genre != ERIEN)
  {
    switch (ZSF.This_room()->map[k+ZSF.column][j+ZSF.line].decor->genre)
    {
      case VIANDE:
	SFDrawSurface(160+32*k,32*j, rc);
	break;
      case CADAVRE:
	rc.setSFRect(32, 7*32 + 600, 32 + 32, 7*32 + 32 + 600);
	/* SFDrawSurface(160+32*k,32*j, rc); */
	break;
      default:
	break;
    }
  }
#endif
}


void UpdateFrame(BYTE Frame)
{
  class SFRect		rc;

#ifdef SF_SDL
   static float c = 0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  /* gluLookAt(xcam,ycam,distance,xcam,ycam,0.0,0.0,1.0,0.0); */
  
  glTranslatef(-xcam, -ycam, -zview);
  glRotatef(phi,1.0,0.0,0.0);
  glRotatef(theta,0.0,0.0,1.0);
  glLightfv(GL_LIGHT0,GL_POSITION,L0pos);
  glLightfv(GL_LIGHT1,GL_POSITION,L1pos);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 1.0);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);

  glPushMatrix();
  glTranslated(5,c,5);
  c = (c + 0.1); if (c > 10.0) c=0;
  /* Paramétrage du matériau */
  /* glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec2); */
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Mspec);
  /* glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Mshiny); */

  glColor3f(1.0, 1.0, 0.0);
  glutSolidSphere(1,20,20);
  glPopMatrix();

  // Plan de l'eau
  glColor3f(0.0, 0.0, 1.0);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Mspec_water);
  glBegin(GL_QUADS);
  glVertex3f(-20.0,20.0, PROF);
  glVertex3f(-20.0,-20.0,PROF);
  glVertex3f(20.0,-20.0,PROF);
  glVertex3f(20.0,20.0,PROF);
  glEnd();
  int alpha = 90 - (phi - 248);
  int beta = 45 - alpha;
  int x = Sin[alpha] / (Cos[alpha] * Cos[alpha]) * 0.1f * Cos[22];
  int h = 1.0 / 2.0f * (Tan[beta] - zview * Tan[alpha]);
  int l = zview * Tan[alpha];
  
  if (ZSF.bScreen)
  {
    int ymin = (int)((float)(ycam + x) / 0.078) + 90;
    ymin = ymin < 0 ? 0 : ymin;
    int ymax =  (int)((float)(l + 2 * h) / 0.078) + ymin + 90; 
    ymax = ymax > 256 ? 256 : ymax;
    int xmin = (int)((float)(xcam + x) / 0.078) + 70;
    int xmax = xmin + 110;
    xmin = xmin < 0 ? 0 : xmin;
    xmax = xmax > 256 ? 256 : xmax;
  /* std::cerr << "alpha " << alpha << " x " << x << " h " << h << " zv " << zview<< std::endl; */
  /* std::cerr << "ypos " << ypos<< " boo " << boo<< std::endl; */
    /* for (int F = 0; F < Frame; ++F) */
    /* { */
      /* DrawMap(); */
      for (int k = xmin; k < xmax; ++k)
	for (int j = ymin; j < ymax; ++j)
	{
	  DrawPheromon(k, j);

	  //Debut des fourmis
	  DrawAnimal(k, j, 1);

	  //Décors, nourriture etc ...
	  DrawOutset(k, j);
	}
      /* if ((ZSF.bDown) && (ZSF.bMove)) */
		/* SelectMultiple(); */
      /* if (ZSF.bSmall_map) */
		/* SmallMap(); */
	
    /* } */
  }
  glPopMatrix();
  if (afficheLampes)
    glCallList(lampes);
  glCallList(terrain);
  if (afficheNormales)
    glCallList(normales);
  if (afficheRepere)
    glCallList(repere);
  SDL_GL_SwapBuffers( ); 
	/*if(SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);*/
  /* DisplayInfo(MData, ZSF); */
  /* SDL_UpdateRect(image, 0, 0, 800, 600); */
  /* Flips(); */
#endif

}
