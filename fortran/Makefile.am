# See http://www.delorie.com/gnu/docs/automake/automake_48.html

AM_CFLAGS =  @WARN_PEDANTIC@

lib_LIBRARIES = libgrib_api_f77.a libgrib_api_f90.a
include_HEADERS = grib_api_f77.h

libgrib_api_f77_a_SOURCES= grib_fortran.c grib_f77.c 
libgrib_api_f77_a_DEPENDENCIES = $(top_builddir)/src/libgrib_api.a

libgrib_api_f90_a_SOURCES= grib_fortran.c grib_f90.f90 
libgrib_api_f90_a_DEPENDENCIES = $(top_builddir)/src/libgrib_api.a grib_api_externals.h grib_api_visibility.h grib_api_constants.h grib_kinds.h

libgrib_api_fortran_prototypes= grib_fortran.c

if  UPPER_CASE_MOD_FALSE
nodist_include_HEADERS = grib_api.mod
grib_api.mod: grib_f90.o
else
nodist_include_HEADERS = GRIB_API.mod
GRIB_API.mod: grib_f90.o
endif

# set the include path
INCLUDES=  -I$(top_builddir)/src

## Make sure these will be cleaned even when they're not built by
## default.
CLEANFILES = libgrib_api_f77.a libgrib_api_f90.a grib_f90.f90 *.mod grib_types grib_kinds.h

#noinst_HEADERS = 

EXTRA_DIST= grib_fortran_prototypes.h grib_api_constants.h grib_api_externals.h \
            grib_api_visibility.h grib_types.f90 create_grib_f90.sh \
            grib_f90.f90.head grib_f90.f90.tail grib_f90_int.f90 grib_f90_long_int.f90 \
			same_int_long.f90 grib_fortran_kinds.c

grib_f90.f90: grib_f90.f90.head grib_f90.f90.tail same_int_long grib_kinds.h
	./create_grib_f90.sh 

grib_f90.o : grib_kinds.h

grib_kinds.h: grib_types
	./grib_types > grib_kinds.h

grib_types: grib_types.o grib_fortran_kinds.o
	$(FC) $(FCFLAGS) -o grib_types grib_types.o grib_fortran_kinds.o

same_int_long: same_int_long.o grib_fortran_kinds.o
	$(FC) $(FCFLAGS) -o same_int_long same_int_long.o grib_fortran_kinds.o

include extrules.am
