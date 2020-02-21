pkgdatadir = $(datadir)/@PACKAGE@/images
FONTS_DIR=$(pkgdatadir)
INSTALL=@INSTALL@
pkgdata_DATA = All.xpm  Icon1.xpm font.ttf
EXTRA_DIST = $(pkgdata_DATA)

# Targets

install:
	@echo Installing resources to $(FONTS_DIR)... 
	@$(INSTALL) -m755 -d $(FONTS_DIR)
	@$(INSTALL) -m644 font.ttf $(FONTS_DIR)

uninstall:
	cd $(FONTS_DIR) ; $(RM) $(pkgdata_DATA)
	rmdir $(FONTS_DIR)
