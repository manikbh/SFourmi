////////////////////////////////////////////////////////////////////////////////
//
//	IOsf.h: En-tête de IOsf.cpp
//
////////////////////////////////////////////////////////////////////////////////

#ifndef IOSF_H
# define IOSF_H

extern bool	Charg_terrain(char *file);
extern void	Sauv_terrain(void);
class IOTablePointeur{
	private:
		//std::vector<void *> table;
		void ** table;
		int taille;
	public:
		IOTablePointeur(int size, bool realSize);
		~IOTablePointeur();
		int FindOrAdd(void * pointeur);
		int Find (void * pointeur);
		int Add (void * pointeur);
		void *get(int i);
		void set(int a, void * pointeur);
		int size(){return taille;}
};


#endif
