////////////////////////////////////////////////////////////////////////////////
//
//	Systeme.h: D�finition de constantes d�pendantes du syst�me
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SYSTEM_H
# define SYSTEM_H

// Release define
//# define RELEASE

//#define WIN32 "WIN32"
#define	SF_SDL "SDL"
//#define GTK_Linux "GTK"

#ifdef SF_SDL
# define GRAPH "SDL"
#endif
#ifdef WIN32
# define GRAPH "DirectX"
#endif
#ifdef GTK_Linux
# define GRAPH "GTK"
#endif
#endif
