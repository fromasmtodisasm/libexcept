
AUX_DIST_GEN = $(ac_aux_dir)

EXTRA_DIST = $(AUX_DIST_GEN) libexcept-1.0.pc.in 
MAINTAINERCLEANFILES = $(AUX_DIST_GEN)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libexcept-1.0.pc

$(pkgconfig_DATA) : config.status

SUBDIRS=src test doc

