#include <string.h>
#include "Wonders.h"
#include "SFourmis.h"

#ifdef GTK_Linux
# include <gtk/gtk.h>
# include <gdk/gdk.h>
# include <gdk-pixbuf/gdk-pixbuf.h>
# include "GTKInterface.h"


#include "GraphXstruct.h"
#include "GraphXproc.h"
#include "GraphXtools.h"
#include "IOsf.h"

//bool Dialogue (char *titre, //Voir page 247 GTK
inline void Display(GdkDrawable *surface,GdkGC* gc, int X, int Y, char *chaine)
{
  gdk_draw_string(surface, police1, gc, X, Y, chaine);
}


int StartInterface(void)
{
  MData.setScreen(SFGAME);
  return(0);
}


void AnalyseTouche(GdkEventKey *ev)				  //Up Return F1 a b....
{
  char *touche=gdk_keyval_name(ev->keyval);
  if(strcmp(touche,"F1")==0)					  //Surface ou fourmilière
    cout << "touche F1\n";
  else 
    if(strcmp(touche,"F2") == 0)

    {
      ZSF.setThis_room((ZSF.This_room() == terre ?
	    ZSF.This_clan()->la_fourme[0] : terre));
      ZSF.column = 0;
      ZSF.line = 0;
    }
    else
      if(strcmp(touche,"F3")==0)				  //CENTRER sur la fourmilière
    {
      ZSF.column = max(0,ZSF.This_clan()->Gate[0][0]-10);
      ZSF.line = max(0,ZSF.This_clan()->Gate[1][0]-9);
      ZSF.column = min(ZSF.column, ZSF.This_room()->size.largeur-20);
      ZSF.line = min( ZSF.line, ZSF.This_room()->size.hauteur-20);
    }
    else
      if(strcmp(touche,"F4")==0)				  //Mini carte
      ZSF.bSmall_map = !ZSF.bSmall_map;
    else
      if(strcmp(touche,"F5")==0)				  //Centre sur la fourmi sélectionnée
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
    else
      if(strcmp(touche,"F9")==0)				  //Sauvegarde
    {
      Sauv_terrain();
    }
    else
      if(strcmp(touche,"F11")==0){			  //Change le clan en cours (sens -)
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
      }
      else if(strcmp(touche,"F12")==0)			  //Change le clan en cours (sens +)
      {
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
      }
      else if(strcmp(touche,"p")==0)				  //ZSF.bPause
	ZSF.bPause = !ZSF.bPause;
      else if(strcmp(touche,"s")==0)				  //STATISTIQUES
	ZSF.bStats = !ZSF.bStats;
      else if(strcmp(touche,"t")==0)				  //Active ou désactive l'affichage principal
	ZSF.bScreen = !ZSF.bScreen;
  //else if(strcmp(touche,"+")==0)				  //Changement d'étage en sous-sol
  //cave_selec=min(les_clans[ZSF.This_clan()]->NBMAX_ROOM,cave_selec++);
  //else if(strcmp(touche,"-")==0)
  //cave_selec=max(0,cave_selec--);
      else if(strcmp(touche,"v")==0)				  //Violent mode
	ZSF.bViolent = !ZSF.bViolent;
      else if(strcmp(touche,"v")==0)
	ZSF.bInfo=!ZSF.bInfo;
      else if(strcmp(touche,"Return")==0)
      {}
      else if(strcmp(touche,"Escape")==0)
	if(ZSF.bPause)
	  ZSF.bPause =FALSE;
	else										  //gdk_event_put(GdkEvent* type GDK_DELETE;
	exit(0);
      else if(strcmp(touche,"Right")==0)
      {
	ZSF.column = min(ZSF.column+1,ZSF.This_room()->size.largeur-10*2);
	updateFrame(1);
      }
      else if(strcmp(touche,"Left")==0)
      {
	ZSF.column = max(ZSF.column-1,0);
	updateFrame(1);
      }
      else if(strcmp(touche,"Down")==0)
      {
	ZSF.line = min(ZSF.line+1, ZSF.This_room()->size.hauteur-9*2);
	updateFrame(1);
      }
      else if(strcmp(touche,"Up")==0)
      {
	ZSF.line = max(ZSF.line-1,0);
	updateFrame(1);
      }

}


void AnalyseEvent(GdkEvent * evenement)
{
  int x,y;
  GdkModifierType mod;
  switch(evenement->type)
  {
    case GDK_KEY_PRESS :
      AnalyseTouche((GdkEventKey *)evenement);
      break;
    case GDK_DESTROY:						  //Selon le WindowManager, peut arriver
    case GDK_DELETE:						  //Message de  fermeture de la fenêtre
      exit(0); break;
    case GDK_MOTION_NOTIFY:					  //Mouvement de la souris
      gdk_window_get_pointer(((GdkEventMotion *)evenement)->window,&x,&y,&mod);
      MouseMove(x,y);
      break;
    case GDK_BUTTON_PRESS :
      //	  MessageBox("Bouton");
      bActive = !bActive;
      if(((GdkEventButton *)evenement)->button==1)
	LButtonDown(static_cast<int>(((GdkEventButton *)evenement)->x)
	    ,static_cast<int>(((GdkEventButton *)evenement)->y)
	    );
      else								  //Bouton milieu ou droit
	RButtonDown(static_cast<int>(((GdkEventButton *)evenement)->x)
	    ,static_cast<int>(((GdkEventButton *)evenement)->y)
	    );
      break;
    case GDK_3BUTTON_PRESS:					  //Considere le triple clic comme un double.
    case GDK_2BUTTON_PRESS:					  //Double clic
      break;
    case GDK_BUTTON_RELEASE:
      if(((GdkEventButton *)evenement)->button==1)
	LButtonUp(static_cast<int>(((GdkEventButton *)evenement)->x)
	    ,static_cast<int>(((GdkEventButton *)evenement)->y)
	    );
      //Pas de fonction Bouton droit
      break;
    case GDK_KEY_RELEASE:
      break;
    default:;
  }
}


void MouseMove(int X ,int Y)
{
  switch(MData.Screen())
  {
    case SFGAME:
      if(!((X < 160)||(Y > 600-24)))
      {
	ZSF.bMove = TRUE;
	MouseXYD[0] = X;
	MouseXYD[1] = Y;
	if(ZSF.bPause) updateFrame(1);
      }
      break;
    case SFINTERFACE:
      //      if((X>195)&&(Y>76)&&(X<418)&&(Y<114))
      //	tb=1;
      //      if((X>195)&&(Y>206)&&(X<418)&&(Y<242))
      //	tb=3;
      break;
  }
}


void LButtonDown(int X ,int Y)
{
  if(!((X < 160)||(Y > 600-24)))
  {
    ZSF.bDown = TRUE;
    MouseXY[0] = X;
    MouseXY[1] = Y;
  }
}


void LButtonUp(int X ,int Y)
{
  /*	HDC hdc;
	RECT rc1;*/
  int rc1X,rc1Y,rc1W,rc1H;
  char BufX[5],BufY[5];
  char *szMiss = "Fourmi !";
  int PosX,PosY,PosXD,PosYD;

  bool Selectbcp = ((ZSF.bDown)&&(ZSF.bMove));
  ZSF.bDown =FALSE;

  ZSF.bMove = FALSE;
  switch(MData.Screen())
  {
    case SFGAME:

      if((X < 160)||(Y > 600-24))
      {
	if((X>71)&&(X<105)&&(Y>504)&&(Y<535))
	{
	  ZSF.ButtonMeat = TRUE;
	}
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
	      updateFrame(1);

	    }
	    return;
	  }
	  if(Selectbcp)
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
	  /*lpDDSPrimary->GetDC(&hdc);
	    SelectObject(hdc, hFont2);
	    SetTextColor(hdc, RGB(255,255,255));
	    SetBkMode(hdc, TRANSPARENT);*/
	  if(ZSF.This_room()->map[PosX][PosY].decor->genre==VIANDE)
	  {
	    ZSF.bInfo =FALSE;
	    char buffer[8];
	    sprintf(buffer,"%d",ZSF.This_room()->map[PosX][PosY].decor->etat);
	    Display(FlipPM, jauneGC, rc1X, rc1Y, BufX);
	    rc1X+=38;
	    Display(FlipPM,jauneGC, rc1X, rc1Y, BufY);
	    rc1Y+=18;rc1X-=38;
	    Display(FlipPM,jauneGC, rc1X, rc1Y, buffer);
	  }
	  ZSF.bInfo = FALSE;ZSF.bInfo_enn=FALSE;
	  for(int i=min(PosX,PosXD);i<=max(PosX,PosXD);i++)
	  {
	    for(int j=min(PosY,PosYD);j<=max(PosY,PosYD);j++)
	    {

	      if(TEST_ANIMAL(i, j, ZSF.This_room()) != NULL)
	      {
		if(ZSF.This_room()->map[i][j].firstAnimal->Esp == FOURMI)
		{
		  ZSF.bInfo = TRUE;
		  ZSF.setThis_animal(ZSF.This_room()->map[i][j].firstAnimal);
		  updateFrame(1);
		  goto plusloineheh;

		}
		//INFOS ENNEMIS
		if(ZSF.This_room()->map[i][j].firstAnimal->Esp !=FOURMI)
		{
		  ZSF.bInfo_enn=TRUE;
		  ZSF.setThis_animal(ZSF.This_room()->map[i][j].firstAnimal);
		  updateFrame(1);
		  goto plusloineheh;
		}
	      }

	    }
	  }
plusloineheh:;

	}
      }
      //lpDDSPrimary->ReleaseDC(hdc);
      break;
    case SFINTERFACE:
      if((X>195)&&(Y>76)&&(X<418)&&(Y<114))
	MData.setScreen(SFGAME);
      if((X>195)&&(Y>206)&&(X<418)&&(Y<242));
      //PostQuitMessage(0);

      break;
  }
}


void RButtonDown(int X ,int Y)
{
  if(ZSF.ButtonMeat) {ZSF.ButtonMeat = FALSE;return;}

#if OUIDEBUG == OUI
  /*HDC hdc;
    RECT rc1;
    char BufX[3],BufY[3];
    int PosX,PosY;

    if((X > 160)&&(Y < 600-24))
    {
    if(!Sous_Terre)
    {
    PosX = (X - 160)/32+ZSF.column;
    PosY = (Y )/32+ZSF.line;
    rc1.top = 180;
    rc1.left = 40;
    _itoa(traces[PosX][PosY][0]->Vex,BufX,10);
    _itoa(traces[PosX][PosY][0]->Vey,BufY,10);
    lpDDSPrimary->GetDC(&hdc);
    SelectObject(hdc, hFont2);
    SetTextColor(hdc, RGB(255,255,255));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, rc1.left, rc1.top, BufX, lstrlen(BufX));
    rc1.left+=38;
    TextOut(hdc, rc1.left, rc1.top, BufY, lstrlen(BufY));
    ZSF.bPause = TRUE;
    lpDDSPrimary->ReleaseDC(hdc);

    }
    }*/
#endif
}

#endif //FIN GTK_Linux
/*long FAR PASCAL WindowProc( HWND hWnd, UINT message,
  WPARAM wParam, LPARAM lParam )
  {
  BYTE ceclan=0;//GAFFE CLAN : remplacer par une var de clan
  switch( message )
  {
  case WM_RBUTTONDOWN:
  RButtonDown(wParam,LOWORD(lParam),HIWORD(lParam));
  break;
  case WM_LBUTTONUP:
  LButtonUp(wParam,LOWORD(lParam),HIWORD(lParam));
  break;
  case WM_LBUTTONDOWN:
  LButtonDown(wParam,LOWORD(lParam),HIWORD(lParam));
  break;

  case WM_ACTIVATEAPP:
  bActive = wParam;
  break;
  case WM_MOUSEMOVE:
  if((ZSF.bDown)||(screennb==0))
  MouseMove(wParam,LOWORD(lParam),HIWORD(lParam));
  case WM_SETCURSOR:
  if(!ZSF.ButtonMeat)
  SetCursor(hCursor);
  else
  SetCursor(hCbouffe);
  return TRUE;

  case WM_CREATE:
  break;

  case WM_KEYDOWN:
  switch( wParam )
  {
  case VK_F2: //dessus dessous !

  Sous_Terre = !Sous_Terre;
  break;
  case VK_F3:	//Centre sur la fourmilière
  if(!Sous_Terre)
  {
  ZSF.column = max(0,les_clans[ZSF.This_clan()]->la_fourme[0]->entree_fourmi[0][0]-10);
  ZSF.line = max(0,les_clans[ZSF.This_clan()]->la_fourme[0]->entree_fourmi[1][0]-9);
  ZSF.column = min(ZSF.column, MData.length-20);
  ZSF.line = min( ZSF.line, MData.height-20);
//ZSF.This_clan()++;
//ZSF.This_clan()=ZSF.This_clan()%MData.nb_clan;
}
break;
case VK_F4: //mini carte !
ZSF.bSmall_map = !ZSF.bSmall_map;
break;

case VK_F5: //Centre sur la fourmi sélectionnée
if(ZSF.bInfo)
{
if(les_clans[clan_selec]->les_fourmis[ZSF.This_animal()]->under!=Sous_Terre)
Sous_Terre= !Sous_Terre;
if(Sous_Terre)
{
ZSF.columnU = max(0,les_clans[clan_selec]->les_fourmis[ZSF.This_animal()]->Xc-10);
ZSF.lineU = max(0,les_clans[clan_selec]->les_fourmis[ZSF.This_animal()]->Yc-9);
ZSF.columnU = min(ZSF.columnU, F_LARGEUR-20);
ZSF.lineU = min( ZSF.lineU, F_HAUTEUR-20);
}
else
{
ZSF.column = max(0,les_clans[clan_selec]->les_fourmis[ZSF.This_animal()]->Xc-10);
ZSF.line = max(0,les_clans[clan_selec]->les_fourmis[ZSF.This_animal()]->Yc-9);
ZSF.column = min(ZSF.column, MData.length-20);
ZSF.line = min( ZSF.line, MData.height-20);
}
}
else if(ZSF.bInfo_enn)
{
  ZSF.column = max(0,les_ennemis[ZSF.This_animal()]->Xc-10);
  ZSF.line = max(0,les_ennemis[ZSF.This_animal()]->Yc-9);
  ZSF.column = min(ZSF.column, MData.length-20);
  ZSF.line = min( ZSF.line, MData.height-20);
}
break;
case 0x53: //mini Stats ! 'S'
ZSF.bStats = !ZSF.bStats;
break;
case 'T':
ZSF.bScreen = !ZSF.bScreen;
break;
case '+':
cave_selec=min(les_clans[ZSF.This_clan()]->NBMAX_ETAGE,cave_selec++);
break;
case '-':
cave_selec=max(0,cave_selec--);
break;
case VK_F9:
Sauv_terrain();
break;
case 'V': //Violent mode
ZSF.bViolent = !ZSF.bViolent;
break;

case VK_RETURN:
ErrMsg (hWnd, "Version : %s \ncol %d ZSF.line %d \nreine %d  %d n° %d \npopulation %d",
    VERSION,
    ZSF.column,ZSF.line,
    les_clans[clan_selec]->les_fourmis[numero_reine[clan_selec]]->Xc, les_clans[clan_selec]->les_fourmis[numero_reine[clan_selec]]->Yc,
    map[les_clans[clan_selec]->les_fourmis[numero_reine[clan_selec]]->Xc][les_clans[clan_selec]->les_fourmis[numero_reine[clan_selec]]->Yc]->genre,
    les_clans[clan_selec]->population);
break;
case VK_ESCAPE:
if(ZSF.bPause){ZSF.bPause =FALSE;break;}
PostMessage(hWnd, WM_CLOSE, 0, 0);
break;
case VK_F11://Change le clan en cours

if(ZSF.This_clan()==0) ZSF.This_clan()=MData.nb_clan-1;
else
ZSF.This_clan()--;
break;
case VK_F12://Change le clan en cours

ZSF.This_clan()++;
if(ZSF.This_clan()==MData.nb_clan)
{
  ZSF.This_clan()=0;
}
break;
  case VK_RIGHT:
if(!Sous_Terre)
{
  ZSF.column = min(ZSF.column++,MData.length-10*2);
  updateFrame(1);
}
else
{
  ZSF.columnU = min(ZSF.columnU++,F_LARGEUR-10*2);
  updateFrame_under();
}
break;
  case VK_LEFT:
if(!Sous_Terre)
{
  ZSF.column = max(ZSF.column--,0);
  updateFrame(1);
}
else
{
  ZSF.columnU = max(ZSF.columnU--,0);
  updateFrame_under();
}
break;
  case VK_DOWN:
if(!Sous_Terre)
{
  ZSF.line = min(ZSF.line++,MData.height-9*2);
  updateFrame(1);
}
else
{
  ZSF.lineU = min(ZSF.lineU++,F_HAUTEUR-9*2);
  updateFrame_under();
}
break;
  case VK_UP:
if(!Sous_Terre)
{
  ZSF.line = max(ZSF.line--,0);

  updateFrame(1);
}
else
{
  ZSF.lineU = max(ZSF.lineU--,0);
  updateFrame_under();
}
break;
case VK_F1:
HDC hdc;
RECT rcWindow;

rcWindow.top = 5;
rcWindow.left = 5;
lpDDSPrimary->GetDC(&hdc);

char *szMsg = "Surprise !!!";
SetTextColor(hdc, RGB(20,60,20));
SetBkMode(hdc, TRANSPARENT);

TextOut(hdc, rcWindow.left, rcWindow.top, lpCmdLine1, lstrlen(lpCmdLine1));
lpDDSPrimary->ReleaseDC(hdc);

ZSF.bPause=TRUE;
if(!strcmp(lpCmdLine1,"sfelkheg") )
{
  info_plus();
}
else 	splash();

break;
}
break;
case WM_DESTROY:

finiObjects();
DeleteObjets();
//ErrMsg (hWnd, "Version : %s",VERSION);

PostQuitMessage( 0 );
break;
}

return DefWindowProc(hWnd, message, wParam, lParam);

}

int StartInterface( void )
{
  Init_screen();
  return screennb;
}     */
