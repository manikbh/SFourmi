## Makefile.am -- Process this file with automake to produce Makefile.in
##

EXTRA_DIST = $(SPECIFIC_MACROS) $(STANDARD_MACROS) $(SPECIFIC_TOOLS)

MAINTAINERCLEANFILES =				\
Makefile.in					\
$(STANDARD_MACROS)				\
$(STANDARD_TOOLS)

STANDARD_MACROS =
SPECIFIC_MACROS =

STANDARD_TOOLS =

SPECIFIC_TOOLS =				\
studentize					\
depcomp						\
install-sh					\
missing						\
mkinstalldirs					\
config.sub					\
config.guess

.PHONY: spy-macro
spy-macro:
	@echo $($(MACRO)))
