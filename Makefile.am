bin_PROGRAMS=monolight
monolight_SOURCES=monolight.c monolight.1 xmalloc.c system.h aclocal.m4 getopt.c getopt1.c getopt.h

#  uncomment the following if monolight requires the math library
#monolight_LDADD=-lm

EXTRA_DIST=monolight.lsm.in monolight.spec.in monolight.texinfo

#  if you write a self-test script named `chk', uncomment the
#  following and add `chk' to the EXTRA_DIST list
#TESTS=chk

#  build and install the .info pages
info_TEXINFOS = monolight.texinfo
monolight_TEXINFOS = gpl.texinfo

#  install the man pages
man_MANS=monolight.1
