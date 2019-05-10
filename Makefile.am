# Copyright (C) 2019 Jo\u00EBl Kr\u00E4hemann
# 
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

AUTOMAKE_OPTIONS = foreign
SUBDIRS = po

ACLOCAL_AMFLAGS = -I m4

# what flags you want to pass to the C compiler & linker
CFLAGS =  @CFLAGS@
LDFLAGS = @LDFLAGS@ $(LTLIBINTL)
LIBTOOL_DEPS = @LIBTOOL_DEPS@

monolight_led_CFLAGS = @MONOLIGHT_LED_CFLAGS@
monolight_led_LDFLAGS = @MONOLIGHT_LED_LDFLAGS@

monolight_test_CFLAGS = @MONOLIGHT_TEST_CFLAGS@
monolight_test_LDFLAGS = @MONOLIGHT_TEST_LDFLAGS@

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

bin_PROGRAMS=monolight-led monolight-test

libmonolightdir = $(libdir)/monolight
libmonolight_LTLIBRARIES = libmonolight.la

EXTRA_DIST = config.rpath \
	monolight/config.h.in \
	gtk-doc.make \
	aclocal.m4 \
	README \
	m4/Makefile.am \
	m4/Makefile.in \
	monolight/i18n.h \
	monolight/gettext.h \
	monolight/object/monolight_marshallers.list \
	m4/README \
	po/README

CLEANFILES = \
	m4/Makefile

libmonolight_la_HEADERS_0 = $(libmonolight_ui_h_sources)
libmonolight_la_SOURCES = $(libmonolight_la_HEADERS_0) $(libmonolight_ui_c_sources)

# library libmonolight
libmonolight_la_CFLAGS = $(CFLAGS) $(COMPILER_FLAGS) $(WARN_FLAGS) -I./ -I/usr/include $(GOBJECT_CFLAGS) $(GDKPIXBUF_CFLAGS) $(CAIRO_CFLAGS) $(GTK_CFLAGS)
libmonolight_la_LDFLAGS = $(LDFLAGS) -version-info 1:0:0 -shared -fPIC -Wl,-soname,libmonolight.so -pthread
libmonolight_la_LIBADD = -ldl -lm $(GOBJECT_LIBS) $(GDKPIXBUF_LIBS) $(CAIRO_LIBS) $(GTK_LIBS)

# monolight-test
monolight_test_SOURCES = $(monolight_testheaders) $(monolight_test_c_sources)

monolight_test_CFLAGS += $(COMPILER_FLAGS) $(WARN_FLAGS) -I./ -I/usr/include $(GOBJECT_CFLAGS) $(GDKPIXBUF_CFLAGS) $(CAIRO_CFLAGS) $(GTK_CFLAGS)
monolight_test_LDFLAGS += -pthread
monolight_test_LDADD = libmonolight.la -ldl -lm $(GOBJECT_LIBS) $(GDKPIXBUF_LIBS) $(CAIRO_LIBS) $(GTK_LIBS)

# monolight-led
monolight_led_SOURCES = $(monolight_ledheaders) $(monolight_led_c_sources)

monolight_led_CFLAGS += $(COMPILER_FLAGS) $(WARN_FLAGS) -I./ -I/usr/include $(GOBJECT_CFLAGS) $(GDKPIXBUF_CFLAGS) $(CAIRO_CFLAGS) $(GTK_CFLAGS)
monolight_led_LDFLAGS += -pthread
monolight_led_LDADD = libmonolight.la -ldl -lm $(GOBJECT_LIBS) $(GDKPIXBUF_LIBS) $(CAIRO_LIBS) $(GTK_LIBS)

# libmonolight_la UI sources
deprecated_libmonolight_ui_h_sources =

libmonolight_ui_h_sources = \
	$(deprecated_libmonolight_ui_h_sources) \
	monolight/ui/monolight_window.h \
	monolight/ui/monolight_server_config.h \
	monolight/ui/monolight_drawing_area.h \
	monolight/ui/monolight_rgb_matrix.h

deprecated_libmonolight_ui_c_sources =

libmonolight_ui_c_sources = \
	$(deprecated_libmonolight_ui_c_sources) \
	monolight/ui/monolight_window.c \
	monolight/ui/monolight_server_config.c \
	monolight/ui/monolight_drawing_area.c \
	monolight/ui/monolight_rgb_matrix.c

# monolight_test all files
monolight_testheaders = \
	monolight/monolight_test.h

monolight_test_c_sources = \
	monolight/monolight_test.c

# monolight_led all files
monolight_ledheaders = \
	monolight/monolight_main.h

monolight_led_c_sources = \
	monolight/monolight_main.c

gen-marshallers:
	glib-genmarshal --prefix=monolight_cclosure_marshal --header monolight/object/monolight_marshallers.list > monolight/object/monolight_marshal.h
	glib-genmarshal --prefix=monolight_cclosure_marshal --body monolight/object/monolight_marshallers.list > monolight/object/monolight_marshal.c
	echo "generated marshallers in monolight/object/"

check-gettext:
	@if test x$(USE_NLS) != "xyes" ; then echo "Missing gettext. Rerun configure and check for" \
	"'checking whether to use NLS... yes'!" ; exit 1 ; fi

update-po: check-gettext
	@find $(srcdir)/monolight/{object,ui}/ -name "*.c" -print | sort > $(srcdir)/po/POTFILES.in.2 ; \
	if diff $(srcdir)/po/POTFILES.in $(srcdir)/po/POTFILES.in.2 >/dev/null 2>&1 ; then \
		rm -f $(srcdir)/po/POTFILES.in.2 ; \
	else \
		mv $(srcdir)/po/POTFILES.in.2 $(srcdir)/po/POTFILES.in ; \
	fi
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-po

update-gmo: check-gettext
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

force-update-gmo: check-gettext
	touch po/*.po
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

force-update-gmo-%: check-gettext
	@language=`echo $@ | sed s/force-update-gmo-//` ; \
	if test ! -f po/$$language.po ; then echo "file po/$$language.po does not exist" ; exit 1 ; fi ; \
	touch po/$$language.po ; \
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

.PHONY: check-gettext update-po update-gmo force-update-gmo
