SUBDIRS = include common terrain animaux

bin_PROGRAMS = SFourmi

#LDFLAGS  = -lSDL -lSDL_ttf -lSDL_image
SFourmi_SOURCES =  SFourmi.cpp


SFourmi_LDADD = \
common/libcommon.a\
animaux/fourmis/libfourmis.a\
../lib/Os/libGraphX.a	\
../lib/Os/opengl/libopengl.a	\
terrain/libterrain.a	\
animaux/libanimaux.a	\
animaux/autres/libautres.a
