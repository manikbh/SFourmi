#ifndef MACROS_H
# define MACROS_H
# define CASE_VALIDE(X, Y, ROOM) \
	(X >= 0 && X < ROOM->size.largeur && Y >= 0 && Y < ROOM->size.hauteur)
# define CASE_VIDE(X, Y, ROOM) \
  	(CASE_VALIDE(X, Y, ROOM) && \
	 	ROOM->map[X][Y].firstAnimal == NULL && \
		ROOM->map[X][Y].decor->genre == ERIEN)
# define TEST_ANIMAL(X, Y, ROOM) \
	(ROOM->map[X][Y].firstAnimal)
# define TEST_UNDER(ROOM) \
  	(ROOM == terre ? false : true)
# define GETSIGN_I(A)\
  	(A == 0 ? 0 : (A >= 0 ? 1 : -1))

# ifdef GTK_Linux
#  define SINT	int
#  define UINT	unsigned int
#  define SBYTE	char
#  define BYTE	unsigned char
# endif

#ifdef SF_SDL
#  define SINT  int
#  define UINT  unsigned int
#  define SBYTE char
#  define BYTE  unsigned char
#endif

# define 		DF_X	800
# define		DF_Y	600
# define		F_LARGEUR 20
# define		F_HAUTEUR 20

# define MAX_I(A, B)\
	(A < B ? B : A)
# define MIN_I(A, B)\
	(A > B ? B : A)

#endif
