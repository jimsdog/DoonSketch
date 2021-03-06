dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.53)

# Always use 0.xx+devel instead of 0.xxdevel for the version, e.g. 0.46+devel.
# Rationale: (i) placate simple version comparison software such as
# `dpkg --compare-versions'.  (ii) We don't always know what the next
# version is going to be called until about the time we release it
# (whereas we always know what the previous version was called).
AC_INIT(inkscape, 0.48.5)

AC_CANONICAL_HOST
AC_CONFIG_SRCDIR([src/main.cpp])
AM_INIT_AUTOMAKE([dist-zip dist-bzip2 tar-pax])

AC_ARG_ENABLE([lsb], AS_HELP_STRING([--enable-lsb], [LSB-compatible build configuration]), [
  prefix=/opt/inkscape
  PATH="/opt/lsb/bin:$PATH"
  CC=lsbcc
  CXX=lsbc++
  export CC CXX
])

AM_CONFIG_HEADER(config.h)

AC_LANG(C++)
AC_ISC_POSIX
AC_PROG_CXX
AM_PROG_CC_STDC
AM_PROG_AS
AC_PROG_RANLIB
AC_PROG_INTLTOOL(0.22)
AC_HEADER_STDC
INK_BZR_SNAPSHOT_BUILD

dnl If automake 1.11 shave the output to look nice
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

dnl These next few lines are needed only while libcroco is in our source tree.
AC_PROG_CC
AM_PROG_CC_C_O
if test "$GCC" = "yes"; then
  # Enable some warnings from gcc.
  AC_LANG_PUSH(C)

  ####
  # Generic cpp flags...

  # What is just plain "-W" ?
  # Fortify source requires -O2 or higher, which is handled with newer autoconf
  CPPFLAGS="-W -D_FORTIFY_SOURCE=2 $CPPFLAGS"
  # Enable format and format security warnings
  CPPFLAGS="-Wformat -Wformat-security $CPPFLAGS"
  # Enable all default warnings
  CPPFLAGS="-Wall $CPPFLAGS"
  # Enforce including only <glib.h> (required since 2.31)
  PKG_CHECK_MODULES(NEW_GLIBMM, glibmm-2.4 >= 2.28, new_glibmm=yes,
new_glibmm=no)
  if test "x$new_glibmm" = "xyes"; then
    CPPFLAGS="-DG_DISABLE_SINGLE_INCLUDES $CPPFLAGS"
  fi

  # Test for -Werror=... (introduced some time post-4.0)
  # If we hit a format error -- it should be fatal.
  AC_MSG_CHECKING([compiler support for -Werror=format-security])
  ink_svd_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="-Werror=format-security $CPPFLAGS"
  AC_COMPILE_IFELSE(AC_LANG_PROGRAM([]), [ink_opt_ok=yes], [ink_opt_ok=no])
  AC_MSG_RESULT([$ink_opt_ok])
  if test "x$ink_opt_ok" != "xyes"; then
    CPPFLAGS="$ink_svd_CPPFLAGS"
  fi

  ####
  # C-specific flags...

  # -Wno-pointer-sign is probably new in gcc 4.0; certainly it isn't accepted
  # by gcc 2.95.
  AC_MSG_CHECKING([compiler support for -Wno-pointer-sign])
  ink_svd_CFLAGS="$CFLAGS"
  CFLAGS="-Wno-pointer-sign $CFLAGS"
  AC_COMPILE_IFELSE(AC_LANG_PROGRAM([]), [ink_opt_ok=yes], [ink_opt_ok=no])
  AC_MSG_RESULT([$ink_opt_ok])
  if test "x$ink_opt_ok" != "xyes"; then
    CFLAGS="$ink_svd_CFLAGS"
  fi

  ####
  # Linker flags...

  # Have linker produce read-only relocations, if it knows how
  AC_MSG_CHECKING([linker tolerates -z relro])
  ink_svd_LDFLAGS="$LDFLAGS"
  LDFLAGS="-Wl,-z,relro $LDFLAGS"
  AC_LINK_IFELSE(AC_LANG_PROGRAM([]), [ink_opt_ok=yes], [ink_opt_ok=no])
  AC_MSG_RESULT([$ink_opt_ok])
  if test "x$ink_opt_ok" != "xyes"; then
    LDFLAGS="$ink_svd_LDFLAGS"
  fi

  AC_LANG_POP

  # C++-specific flags are defined further below.  Look for CXXFLAGS...
fi

dnl Honor aclocal flags
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

# Detect a working version of unordered containers.
AC_MSG_CHECKING([TR1 unordered_set usability])
AC_COMPILE_IFELSE([
#include <tr1/unordered_set>
int main() {
  std::tr1::unordered_set<int> i, j;
  i = j;
  return 0;
}
], [unordered_set_works=yes], [unordered_set_works=no])
if test "x$unordered_set_works" = "xyes"; then
	AC_MSG_RESULT([ok])
	AC_DEFINE(HAVE_TR1_UNORDERED_SET, 1, [Has working standard TR1 unordered_set])
else
	AC_MSG_RESULT([not working])
fi
AC_CHECK_HEADER([boost/unordered_set.hpp], [AC_DEFINE(HAVE_BOOST_UNORDERED_SET, 1, [Boost unordered_set (Boost >= 1.36)])], [])
AC_CHECK_HEADER([ext/hash_set], [AC_DEFINE(HAVE_EXT_HASH_SET, 1, [Legacy GNU ext/hash_set])], [])

# Test whether GCC emits a spurious warning when using boost::optional
# If yes, turn off strict aliasing warnings to reduce noise
# and allow the legitimate warnings to stand out
AC_MSG_CHECKING([for overzealous strict aliasing warnings])
ignore_strict_aliasing=no
CXXFLAGS_SAVE=$CXXFLAGS
CXXFLAGS="$CXXFLAGS -Werror=strict-aliasing"
AC_COMPILE_IFELSE([
#include <boost/optional.hpp>
boost::optional<int> x;
int func() {
  return *x;
}
], [ignore_strict_aliasing=no], [ignore_strict_aliasing=yes])
AC_MSG_RESULT($ignore_strict_aliasing)
CXXFLAGS=$CXXFLAGS_SAVE
if test "x$ignore_strict_aliasing" = "xyes"; then
  CXXFLAGS="$CXXFLAGS -Wno-strict-aliasing"
fi

dnl ******************************
dnl Gettext stuff
dnl ******************************
GETTEXT_PACKAGE="AC_PACKAGE_NAME"
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[Translation domain used])
dnl Add the languages which your application supports here.
ALL_LINGUAS="am ar az be bg bn br ca ca@valencia cs da de dz el en_AU en_CA en_GB en_US@piglatin eo es_MX es et eu fa fi fr ga gl he hr hu hy id it ja km ko lt mk mn nb ne nl nn pa pl pt_BR pt ro ru rw sk sl sq sr@latin sr sv te_IN th tr uk vi zh_CN zh_TW"
AM_GLIB_GNU_GETTEXT

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test "x$PKG_CONFIG" = "xno"; then
	AC_MSG_ERROR(You have to install pkg-config to compile inkscape.)
fi

dnl Find msgfmt.  Without this, po/Makefile fails to set MSGFMT on some platforms.
AC_PATH_PROG(MSGFMT, msgfmt, msgfmt)
AC_PATH_PROG(GMSGFMT, gmsgfmt, $MSGFMT)

dnl ******************************
dnl Check for OpenMP 
dnl Replace this with AC_OPENMP once Autoconf 2.62 is widespread
dnl ******************************
AX_OPENMP([AC_CHECK_HEADER([omp.h],
	                   [openmp_ok=yes],
			   [openmp_ok=no])
])

if test "x$openmp_ok" = "xyes"; then
	dnl We have it, now set up the flags
	CXXFLAGS="$CXXFLAGS $OPENMP_CXXFLAGS"
	AC_DEFINE(HAVE_OPENMP, 1, [Use OpenMP])
fi

dnl ******************************
dnl Check for libpng 
dnl ******************************
AC_CHECK_LIB(png, png_read_info, [AC_CHECK_HEADER(png.h, png_ok=yes, png_ok=yes)], png_ok=no, -lz -lm)
if test "x$png_ok" != "xyes"; then
	AC_MSG_ERROR([libpng >= 1.2 is needed to compile inkscape])
fi

dnl Handle possible dlopen requirement for libgc
dnl Isn't this internal to something in autoconf?  Couldn't find it...
AC_CHECK_LIB([dld], [shl_load], [lt_cv_dlopen="shl_load" lt_cv_dlopen_libs="-dld"], [AC_CHECK_FUNC([dlopen],
          [lt_cv_dlopen="dlopen"],
      [AC_CHECK_LIB([dl], [dlopen],
            [lt_cv_dlopen="dlopen" lt_cv_dlopen_libs="-ldl"],
        [AC_CHECK_LIB([svld], [dlopen],
              [lt_cv_dlopen="dlopen" lt_cv_dlopen_libs="-lsvld"],
          [AC_CHECK_LIB([dld], [dld_link],
                [lt_cv_dlopen="dld_link" lt_cv_dlopen_libs="-dld"])
          ])
        ])
      ])
    ])

AC_CHECK_HEADERS([gc.h gc/gc.h],
                 [
		    # To test for the different required libs, I have to
		    # overcome autoconf's caching system, so I change the
		    # desired function name.  They're all in libgc.
		    # The "break" will exit from the top level
                    # AC_CHECK_HEADERS.
		    gc_libs=""
		    AC_CHECK_LIB(gc, GC_init,
                                 [gc_ok=yes;
				  LIBS="-lgc $gc_libs $LIBS";
				  break], [gc_ok=no], [$gc_libs])
		    gc_libs="-lpthread"
		    AC_CHECK_LIB(gc, GC_malloc,
                                 [gc_ok=yes;
				  LIBS="-lgc $gc_libs $LIBS";
                                  break], [gc_ok=no], [$gc_libs])
		    gc_libs="$lt_cv_dlopen_libs"
		    AC_CHECK_LIB(gc, GC_realloc,
                                 [gc_ok=yes;
				  LIBS="-lgc $gc_libs $LIBS";
                                  break], [gc_ok=no], [$gc_libs])
		    gc_libs="-lpthread $lt_cv_dlopen_libs"
		    AC_CHECK_LIB(gc, GC_free,
                                 [gc_ok=yes;
				  LIBS="-lgc $gc_libs $LIBS";
				  break], [gc_ok=no], [$gc_libs])
		    break],
                 [gc_ok=no])
if test "x$gc_ok" = "xyes" && test "x$cross_compiling" = "xno" ; then 
	AC_MSG_CHECKING([libgc version 6.4+])
	AC_RUN_IFELSE(
		[AC_LANG_SOURCE([[
			#ifdef HAVE_GC_GC_H
			# include <gc/gc.h>
			#else
			# include <gc.h>
			#endif
			#include <stdio.h>
			extern unsigned GC_version;
			int main(void){
				unsigned min = ((6 << 16) | (4 << 8) | 0);
				printf("%d.%d.%d ",GC_version >> 16, (GC_version >> 8) & 0xFF, GC_version & 0xFF);
				if (GC_version>=min) return 0;
				return 1;
			}]])],
		[gc_ok=yes],
		[gc_ok=no]
	)
	AC_MSG_RESULT([$gc_ok])
fi
if test "x$gc_ok" != "xyes"; then
	AC_MSG_ERROR([libgc (the Boehm Conservative Collector) 6.4+, is needed to compile inkscape -- http://www.hpl.hp.com/personal/Hans_Boehm/gc])
fi

dnl This check is to get a FIONREAD definition on Solaris 8
AC_CHECK_HEADERS([sys/filio.h])


AC_CHECK_HEADERS([malloc.h])
AC_CHECK_FUNCS([mallinfo], [
	AC_CHECK_MEMBERS([struct mallinfo.usmblks,
			  struct mallinfo.fsmblks,
			  struct mallinfo.uordblks,
			  struct mallinfo.fordblks,
			  struct mallinfo.hblkhd],,,
			 [#include <malloc.h>])
])

AC_PATH_PROG(FREETYPE_CONFIG, freetype-config, no)
if test "x$FREETYPE_CONFIG" = "xno"; then
	AC_MSG_ERROR([Cannot find freetype-config])
fi
FREETYPE_CFLAGS=`$FREETYPE_CONFIG --cflags`
FREETYPE_LIBS=`$FREETYPE_CONFIG --libs`
AC_SUBST(FREETYPE_CFLAGS)
AC_SUBST(FREETYPE_LIBS)

dnl ******************************
dnl Win32
dnl ******************************
AC_MSG_CHECKING([for Win32 platform])
case "$host" in
  *-*-mingw*)
    platform_win32=yes
    WIN32_CFLAGS="-mms-bitfields -DLIBXML_STATIC"
    ;;
  *)
    platform_win32=no
    ;;
esac
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL(PLATFORM_WIN32, test "$platform_win32" = "yes")
AC_SUBST(WIN32_CFLAGS)

dnl ******************************
dnl MacOS X
dnl ******************************
AC_MSG_CHECKING([for OSX platform])
if test "x$build_vendor" = "xapple" ; then
  platform_osx=yes
else
  platform_osx=no
fi
AC_MSG_RESULT([$platform_osx])
AM_CONDITIONAL(PLATFORM_OSX, test "$platform_osx" = "yes")
	
AC_MSG_CHECKING([for Solaris platform])
case "$host" in
  *-solaris2.*)
    platform_solaris=yes
    solaris_version=`echo $host|sed -e 's/^.*-solaris2\.//' -e s'/\..*$//'`
    CFLAGS="$CFLAGS -DSOLARIS=$solaris_version"
    CXXFLAGS="$CXXFLAGS -DSOLARIS=$solaris_version"
    ;;
  *)
    platform_solaris=no
    ;;
esac
AC_MSG_RESULT([$platform_solaris])
AM_CONDITIONAL(PLATFORM_SOLARIS, test "$platform_solaris" = "yes")

dnl ******************************
dnl gnome vfs checking
dnl ******************************

AC_ARG_WITH(gnome-vfs,
	AC_HELP_STRING([--with-gnome-vfs], [use gnome vfs for loading files]),
	[with_gnome_vfs=$withval], [with_gnome_vfs=auto])

if test "x$with_gnome_vfs" = "xno"; then
	dnl Asked to ignore gnome-vfs
	gnome_vfs=no
else
        dnl Have to test gnome-vfs presence
	PKG_CHECK_MODULES(GNOME_VFS, gnome-vfs-2.0 >= 2.0, gnome_vfs=yes, gnome_vfs=no)
	if test "x$gnome_vfs" != "xyes"; then
	        dnl No gnome-vfs found
		if test "x$with_gnome_vfs" = "xyes"; then
		        dnl Gnome-VFS was explicitly asked for, so stop
			AC_MSG_ERROR([--with-gnome-vfs was specified, but appropriate libgnomevfs development packages could not be found])
		else
			# gnome-vfs is no, tell us for the log file
			AC_MSG_RESULT($gnome_vfs)
		fi
	fi
fi

AM_CONDITIONAL(USE_GNOME_VFS, test "x$gnome_vfs" = "xyes")
if test "x$gnome_vfs" = "xyes"; then
	AC_DEFINE(WITH_GNOME_VFS, 1, [Use gnome vfs file load functionality])
fi

AC_SUBST(GNOME_VFS_CFLAGS)
AC_SUBST(GNOME_VFS_LIBS)

dnl ******************************
dnl libinkjar checking
dnl ******************************

AC_ARG_WITH(inkjar,
	AC_HELP_STRING([--without-inkjar], [disable openoffice files (SVG jars)]),[with_ij=$withval], [with_ij=yes])

if test "x$with_ij" = "xyes"; then
	AC_DEFINE(WITH_INKJAR, 1, [enable openoffice files (SVG jars)])
	AC_C_BIGENDIAN
	AC_CHECK_HEADERS(zlib.h)
	ij=yes
else
	ij=no
fi
AM_CONDITIONAL(INKJAR, test "$with_ij" = "yes")

ink_spell_pkg=
if pkg-config --exists gtkspell-2.0; then
	ink_spell_pkg=gtkspell-2.0
	AC_DEFINE(WITH_GTKSPELL, 1, [enable gtk spelling widget])
fi

dnl ******************************
dnl PERL checking
dnl ******************************

AC_MSG_CHECKING(for Perl development environment)
AC_ARG_WITH(perl,
            AC_HELP_STRING([--with-perl], [use Perl for embedded scripting (EXPERIMENTAL)]),
            [with_perl=$withval], [with_perl=skipped])

if test "x$with_perl" = "xyes"; then
    checkPERL_CFLAGS=`perl -MExtUtils::Embed -e perl_inc 2>/dev/null`
    if test "$?" -gt "0"; then
        with_perl="no"
    else
        checkPERL_LIBS=`perl -MExtUtils::Embed -e ldopts 2>/dev/null`
        if test "$?" -gt "0"; then
            with_perl="no"
        else
            with_perl="yes"
        fi
    fi
fi
AC_MSG_RESULT([$with_perl])
if test "x$with_perl" = "xyes"; then
    # Test that we actually have the perl libraries installed
    oldCFLAGS="$CFLAGS"
    oldLIBS="$LIBS"
    CFLAGS="$CFLAGS $checkPERL_CFLAGS"
    LIBS="$LIBS $checkPERL_LIBS"
    AC_CHECK_FUNC([perl_parse],[
	PERL_CFLAGS="$checkPERL_CFLAGS"
	PERL_LIBS="$checkPERL_LIBS"
        AC_DEFINE(WITH_PERL, 1, [use Perl for embedded scripting])
        ],[
	with_perl="no"
	])
    CFLAGS="$oldCFLAGS"
    LIBS="$oldLIBS"
fi
AM_CONDITIONAL(WITH_PERL, test "x$with_perl" = "xyes")
AC_SUBST(PERL_CFLAGS)
AC_SUBST(PERL_LIBS)

dnl ******************************
dnl Python checking
dnl ******************************

AC_MSG_CHECKING(for Python development environment)
AC_ARG_WITH(python,
            AC_HELP_STRING([--with-python], [use Python for embedded scripting (EXPERIMENTAL)]),
            [with_python=$withval], [with_python=skipped])

if test "x$with_python" = "xyes"; then
    checkPYTHON_CFLAGS=`python -c "import distutils.sysconfig ; print '-I%s' % distutils.sysconfig.get_config_var('INCLUDEPY')" 2>/dev/null`
    if test "$?" -gt "0"; then
        with_python="no"
    else
        checkPYTHON_LIBS=`python -c "import distutils.sysconfig ; print '%s/%s %s' % (distutils.sysconfig.get_config_var('LIBPL'),distutils.sysconfig.get_config_var('LDLIBRARY'),distutils.sysconfig.get_config_var('LIBS'))" 2>/dev/null`
        if test "$?" -gt "0"; then
            with_python="no"
        else
            with_python="yes"
        fi
    fi
fi
AC_MSG_RESULT([$with_python])
if test "x$with_python" = "xyes"; then
    # Test that we actually have the python libraries installed
    oldCFLAGS="$CFLAGS"
    oldLIBS="$LIBS"
    CFLAGS="$CFLAGS $checkPYTHON_CFLAGS"
    LIBS="$LIBS $checkPYTHON_LIBS"
    AC_CHECK_FUNC([Py_Initialize],[
	PYTHON_CFLAGS="$checkPYTHON_CFLAGS"
	PYTHON_LIBS="$checkPYTHON_LIBS"
        AC_DEFINE(WITH_PYTHON, 1, [use Python for embedded scripting])
        ],[
	with_python="no"
	])
    CFLAGS="$oldCFLAGS"
    LIBS="$oldLIBS"
fi
AM_CONDITIONAL(WITH_PYTHON, test "x$with_python" = "xyes")
AC_SUBST(PYTHON_CFLAGS)
AC_SUBST(PYTHON_LIBS)

dnl ******************************
dnl LittleCms checking
dnl ******************************

AC_ARG_ENABLE(lcms,
	AC_HELP_STRING([--enable-lcms], [enable LittleCms for color management]),
	[enable_lcms=$enableval], [enable_lcms=yes])

if test "x$enable_lcms" = "xno"; then
    # Asked to ignore LittleCms
    lcms=no
    have_lcms2=no
else
    # Have to test LittleCms presence
    if test "x${platform_osx}" != "xyes"; then
       # lcms 2.2 & 2.3 have problems on OSX
	PKG_CHECK_MODULES(LCMS2, lcms2, have_lcms2="yes", have_lcms2="no")
    fi

    if test "x${have_lcms2}" = "xyes"; then
        LIBS="$LIBS $LCMS2_LIBS"
        AC_DEFINE(HAVE_LIBLCMS2, 1, [define to 1 if you have lcms version 2.x])
	AC_SUBST(LCMS2_CFLAGS)
	AC_SUBST(LCMS2_LIBS)
    else
        PKG_CHECK_MODULES(LCMS, lcms >= 1.13, lcms=yes, lcms=no)
        if test "x$lcms" = "xyes"; then
            LIBS="$LIBS $LCMS_LIBS"
            AC_DEFINE(HAVE_LIBLCMS1, 1, [define to 1 if you have lcms version 1.x])
	    AC_SUBST(LCMS_CFLAGS)
	    AC_SUBST(LCMS_LIBS)
        else
            # No lcms found. LittleCms was explicitly asked for, so stop
            AC_MSG_ERROR([--enable-lcms was specified, but appropriate LittleCms development packages could not be found])
        fi
    fi
fi

dnl ******************************
dnl Libpoppler checking
dnl ******************************

AC_ARG_ENABLE(poppler-cairo,
	AC_HELP_STRING([--enable-poppler-cairo], [Enable libpoppler-cairo for rendering PDF preview]),
	[enable_poppler_cairo=$enableval], [enable_poppler_cairo=yes])

POPPLER_CFLAGS=""
PKG_CHECK_MODULES(POPPLER, poppler >= 0.5.9, poppler=yes, poppler=no)

if test "x$poppler" = "xyes"; then
	dnl Working libpoppler
	dnl Have to test libpoppler-glib presence
	PKG_CHECK_MODULES(POPPLER_GLIB, poppler-glib >= 0.5.9, poppler_glib=yes, poppler_glib=no)
	if test "x$poppler_glib" = "xyes"; then
		dnl Working libpoppler-glib found
		dnl Check whether the Cairo SVG backend is available
		PKG_CHECK_MODULES(CAIRO_SVG, cairo-svg, cairo_svg=yes, cairo_svg=no)
		if test "x$cairo_svg" = "xyes"; then
			POPPLER_LIBS="$POPPLER_LIBS $POPPLER_GLIB_LIBS "
		fi
	fi
	if test "x$enable_poppler_cairo" = "xyes"; then
		dnl Have to test libpoppler-cairo presence for PDF preview
		dnl AC_CHECK_HEADER(Magick++.h, magick_ok=yes, magick_ok=no)
		PKG_CHECK_MODULES(POPPLER_CAIRO, poppler-cairo >= 0.5.9, poppler_cairo=yes, poppler_cairo=no)
		if test "x$poppler_glib" = "xyes" -a "x$poppler_cairo" = "xyes" -a \
			"x$cairo_svg" = "xno"
		then
			POPPLER_LIBS="$POPPLER_LIBS $POPPLER_CAIRO_LIBS "
		fi
	fi
fi

if test "x$poppler" = "xyes"; then
	LIBS="$LIBS $POPPLER_LIBS"
	AC_DEFINE(HAVE_POPPLER, 1, [Use libpoppler for direct PDF import])
fi
if test "x$poppler_cairo" = "xyes" -a "x$poppler_glib" = "xyes"; then
	AC_DEFINE(HAVE_POPPLER_CAIRO, 1, [Use libpoppler-cairo for rendering PDF preview])
fi
if test "x$poppler_glib" = "xyes" -a "x$cairo_svg" = "xyes"; then
	AC_DEFINE(HAVE_POPPLER_GLIB, 1, [Use libpoppler-glib and Cairo-SVG for PDF import])
fi
AC_SUBST(POPPLER_CFLAGS)
AC_SUBST(POPPLER_LIBS)

PKG_CHECK_MODULES(POPPLER_GFXFONT, poppler >= 0.8.3, popplernew=yes, popplernew=no)
if test "x$popplernew" = "xyes"; then
	AC_DEFINE(POPPLER_NEW_GFXFONT, 1, [Use GfxFont from Poppler >= 0.8.3])
fi

PKG_CHECK_MODULES(POPPLER_NEWERRORAPI, poppler >= 0.20.0, popplernewerror=yes, popplernewerror=no)
if test "x$popplernewerror" = "xyes"; then
	AC_DEFINE(POPPLER_NEW_ERRORAPI, 1, [Use new error API from Poppler >= 0.20.0])
fi


ink_svd_CPPFLAGS=$CPPFLAGS
ink_svd_LIBS=$LIBS
CPPFLAGS="$CPPFLAGS $POPPLER_CFLAGS"
LIBS="$LIBS $POPPLER_LIBS"

AC_MSG_CHECKING(for new color space API in Poppler)
popplercolor="no"
AC_COMPILE_IFELSE([
#include <GfxState.h>

int main() {
  typedef GfxColorSpace *(*parse_p)(Object *, Gfx *);
  parse_p p;
  p = &GfxColorSpace::parse;
  return 0;
}
], [popplercolor=yes])
if test "x$popplercolor" = "xyes"; then
	AC_DEFINE(POPPLER_NEW_COLOR_SPACE_API, 1, [Use color space API from Poppler >= 0.12.2])
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

PKG_CHECK_MODULES(POPPLER_EVEN_NEWER_COLOR_SPACE_API, poppler >= 0.26.0, popplernewercolorspaceapi=yes, popplernewercolorspaceapi=no)
if test "x$popplernewercolorspaceapi" = "xyes"; then
	AC_DEFINE(POPPLER_EVEN_NEWER_COLOR_SPACE_API, 1, [Use even newer color space API from Poppler >= 0.26.0])
fi

# Poppler's b604a008 commit changes this
AC_MSG_CHECKING([whether Poppler's GfxPatch no longer uses GfxColor])
popplergfxcolor="no"
AC_COMPILE_IFELSE([
#include <GfxState.h>

int main() {
  GfxPatch::ColorValue color = {c: {0}};
  GfxPatch patch;
  patch.color[[0]][[0]] = color;
  return 0;
}
], [popplergfxcolor=yes])
if test "x$popplergfxcolor" = "xyes"; then
	AC_DEFINE(POPPLER_NEW_GFXPATCH, 1, [GfxPatch no longer uses GfxColor in >= 0.15.1])
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

CPPFLAGS=$ink_svd_CPPFLAGS
LIBS=$ink_svd_LIBS

dnl ******************************
dnl Inkboard dependency checking
dnl ******************************

dnl with_inkboard="no"
dnl with_inkboard_ssl="no"

dnl INKBOARD_CFLAGS=""

dnl AC_ARG_ENABLE(inkboard,
dnl 		AS_HELP_STRING([--enable-inkboard], [enable Inkboard online whiteboard facility (disabled by default)]),
dnl        with_inkboard=$enableval,with_inkboard=no)

dnl if test "x$with_inkboard" = "xyes"; then
dnl 		with_inkboard="yes"
dnl 		AC_DEFINE(WITH_INKBOARD,1,[Build in Inkboard support])
		
		dnl Test for OpenSSL
dnl 		PKG_CHECK_MODULES(INKBOARD, openssl, with_inkboard_ssl=yes, with_inkboard_ssl=no)
dnl 		if test "x$with_inkboard_ssl" = "xyes"; then
			dnl OpenSSL found; enable SSL support in Pedro
dnl 			INKBOARD_CFLAGS="$INKBOARD_CFLAGS -DHAVE_SSL"
dnl 			RELAYTOOL([ssl], INKBOARD_LIBS, INKBOARD_CFLAGS, [inkboard_weak=yes])
dnl 			RELAYTOOL([crypto], INKBOARD_LIBS, INKBOARD_CFLAGS, [inkboard_weak=yes])
dnl 			AC_DEFINE(WITH_INKBOARD_SSL,1,[Build in SSL support for Inkboard])
dnl 		fi
dnl else
dnl 	with_inkboard="no"
dnl fi

dnl AM_CONDITIONAL(WITH_INKBOARD, test "x$with_inkboard" = "xyes")

INKBOARD_CFLAGS=""
INKBOARD_LIBS=""
AC_SUBST(INKBOARD_LIBS)
AC_SUBST(INKBOARD_CFLAGS)

dnl ******************************
dnl Check for libwpg for extension
dnl ******************************

with_libwpg=no

PKG_CHECK_MODULES(LIBWPG01, libwpg-0.1 libwpg-stream-0.1, with_libwpg01=yes, with_libwpg01=no)
if test "x$with_libwpg01" = "xyes"; then
	AC_DEFINE(WITH_LIBWPG01,1,[Build in libwpg 0.1.x])
	with_libwpg=yes
	AC_SUBST(LIBWPG_LIBS, $LIBWPG01_LIBS)
	AC_SUBST(LIBWPG_CFLAGS, $LIBWPG01_CFLAGS)
fi
AM_CONDITIONAL(WITH_LIBWPG01, test "x$with_libwpg01" = "xyes")

PKG_CHECK_MODULES(LIBWPG02, libwpg-0.2 libwpd-0.9 libwpd-stream-0.9, with_libwpg02=yes, with_libwpg02=no)
if test "x$with_libwpg02" = "xyes"; then
	AC_DEFINE(WITH_LIBWPG02,1,[Build in libwpg 0.2.x])
	with_libwpg=yes
	AC_SUBST(LIBWPG_LIBS, $LIBWPG02_LIBS)
	AC_SUBST(LIBWPG_CFLAGS, $LIBWPG02_CFLAGS)
fi
AM_CONDITIONAL(WITH_LIBWPG02, test "x$with_libwpg02" = "xyes")

if test "x$with_libwpg" = "xyes"; then
	AC_DEFINE(WITH_LIBWPG,1,[Build in libwpg])
fi
AM_CONDITIONAL(WITH_LIBWPG, test "x$with_libwpg" = "xyes")

dnl ******************************
dnl Check for ImageMagick Magick++ 
dnl ******************************

PKG_CHECK_MODULES(IMAGEMAGICK, ImageMagick++, magick_ok=yes, magick_ok=no)
if test "x$magick_ok" = "xyes"; then
      AC_DEFINE(WITH_IMAGE_MAGICK,1,[Image Magick++ support for bitmap effects])
fi
AM_CONDITIONAL(USE_IMAGE_MAGICK, test "x$magick_ok" = "xyes")

AC_SUBST(IMAGEMAGICK_LIBS)
AC_SUBST(IMAGEMAGICK_CFLAGS)

dnl ***********************************************************************************************************
dnl Check for a Cairo version that implements user-fonts feature (with a stable API, that is cairo > 1.7.6),
dnl so that we conditionally add SVGFonts support
dnl ***********************************************************************************************************

PKG_CHECK_MODULES(CAIRO_USER_FONTS, cairo > 1.7.6, cairouserfonts=yes, cairouserfonts=no)
if test "x$cairouserfonts" = "xyes"; then
       AC_DEFINE(ENABLE_SVG_FONTS, 1, [SVG Fonts should be used])
fi

dnl ******************************
dnl   Unconditional dependencies
dnl ******************************

PKG_CHECK_MODULES(INKSCAPE, gdkmm-2.4  glibmm-2.4 giomm-2.4 gtkmm-2.4 >= 2.10.0  gtk+-2.0 pangoft2 libxml-2.0 >= 2.6.11  libxslt >= 1.0.15  cairo  sigc++-2.0 >= 2.0.12  $ink_spell_pkg  gthread-2.0 >= 2.0 libpng >= 1.2 gsl)

dnl gtkmm provides no preprocessor macros for easily checking versions.  This check is used
dnl to figure out if we are working with the newer 2.24 series or not.
PKG_CHECK_MODULES(GTKMM_2_24, gtkmm-2.4 >= 2.24, with_gtkmm_2_24=yes, with_gtkmm_2_24=no)
if test "x$with_gtkmm_2_24" = "xyes"; then
	AC_DEFINE(WITH_GTKMM_2_24,1,[Build with Gtkmm 2.24.x])
fi

PKG_CHECK_MODULES(GTKMM_2_22, gtkmm-2.4 >= 2.22, with_gtkmm_2_22=yes, with_gtkmm_2_22=no)
if test "x$with_gtkmm_2_22" = "xyes"; then
	AC_DEFINE(WITH_GTKMM_2_22,1,[Build with Gtkmm 2.22.x])
fi

# Check for GTK+ backend target
AC_MSG_CHECKING([for GTK+ backend target])
gtk_backend_target=`pkg-config --variable=target gtk+-2.0`
AC_MSG_RESULT($gtk_backend_target)

# Add X11 library if needed
if test "x$gtk_backend_target" = "xx11"; then
  INKSCAPE_LIBS="$INKSCAPE_LIBS -lX11"
fi

# Check for Apple Mac OS X Carbon framework
carbon_ok=no
AC_MSG_CHECKING([for Mac OS X Carbon support])
AC_COMPILE_IFELSE([
#include <Carbon/Carbon.h>
#include <CoreServices/CoreServices.h>
], [carbon_ok=yes])
AC_MSG_RESULT($carbon_ok)
if test "x$carbon_ok" = "xyes"; then
  AC_DEFINE(HAVE_CARBON, 1, [define to 1 if Carbon is available])
  CARBON_LDFLAGS="-framework Carbon"
  AC_SUBST(CARBON_LDFLAGS)
fi
AM_CONDITIONAL(HAVE_CARBON, test "x$carbon_ok" = "xyes")

# Check for some boost header files
AC_CHECK_HEADERS([boost/concept_check.hpp], [], AC_MSG_ERROR([You need the boost package (e.g. libboost-dev)]))

PKG_CHECK_MODULES(CAIRO_PDF, cairo-pdf, cairo_pdf=yes, cairo_pdf=no)
if test "x$cairo_pdf" = "xyes"; then
  AC_DEFINE(HAVE_CAIRO_PDF, 1, [Whether the Cairo PDF backend is available])
fi

dnl Shouldn't we test for libpng and libz?
INKSCAPE_LIBS="$INKSCAPE_LIBS -lpng -lz -lxml2 -ldl"
if test "x$openmp_ok" = "xyes"; then
	INKSCAPE_LIBS="$INKSCAPE_LIBS -lgomp"
fi

AC_CHECK_HEADER(popt.h,
		[INKSCAPE_LIBS="$INKSCAPE_LIBS -lpopt"],
		AC_MSG_ERROR([libpopt is required]))

dnl **************************
dnl Check for aspell 
dnl ******************************
AC_CHECK_LIB(aspell, new_aspell_config, [AC_CHECK_HEADER(aspell.h, aspell_ok=yes, aspell_ok=no)], aspell_ok=no, -lz -lm)
if test "x$aspell_ok" = "xyes"; then
	AC_DEFINE(HAVE_ASPELL, 1, [Use aspell for built-in spellchecker])
  INKSCAPE_LIBS="$INKSCAPE_LIBS -laspell"
else
	AC_MSG_CHECKING([Aspell not found, spell checker will be disabled])
fi

dnl Check for bind_textdomain_codeset, including -lintl if GLib brings it in.
sp_save_LIBS=$LIBS
LIBS="$LIBS $INKSCAPE_LIBS"
AC_CHECK_FUNCS(bind_textdomain_codeset)
dnl Check for gtk_window_fullscreen in gtk (>= 2.2)
AC_CHECK_FUNCS(gtk_window_set_default_icon_from_file)
AC_CHECK_FUNCS(gtk_window_fullscreen)
LIBS=$sp_save_LIBS


dnl Check for binary relocation support
dnl Hongli Lai <h.lai@chello.nl>

AC_ARG_ENABLE(binreloc,
       [  --enable-binreloc       compile with binary relocation support],
       enable_binreloc=$enableval,enable_binreloc=no)

AC_MSG_CHECKING(whether binary relocation support should be enabled)
if test "$enable_binreloc" = "yes"; then
       AC_MSG_RESULT(yes)
       AC_MSG_CHECKING(for linker mappings at /proc/self/maps)
       if test -e /proc/self/maps; then
               AC_MSG_RESULT(yes)
       else
               AC_MSG_RESULT(no)
               AC_MSG_ERROR(/proc/self/maps is not available. Binary relocation cannot be enabled.)
               enable_binreloc="no"
       fi

elif test "$enable_binreloc" = "auto"; then
       AC_MSG_RESULT(yes when available)
       AC_MSG_CHECKING(for linker mappings at /proc/self/maps)
       if test -e /proc/self/maps; then
               AC_MSG_RESULT(yes)
               enable_binreloc=yes

               AC_MSG_CHECKING(whether everything is installed to the same prefix)
               if test "$bindir" = '${exec_prefix}/bin' -a "$sbindir" = '${exec_prefix}/sbin' -a \
                       "$datadir" = '${prefix}/share' -a "$libdir" = '${exec_prefix}/lib' -a \
                       "$libexecdir" = '${exec_prefix}/libexec' -a "$sysconfdir" = '${prefix}/etc'
               then
                       AC_MSG_RESULT(yes)
               else
                       AC_MSG_RESULT(no)
                       AC_MSG_NOTICE(Binary relocation support will be disabled.)
                       enable_binreloc=no
               fi

       else
               AC_MSG_RESULT(no)
               enable_binreloc=no
       fi

elif test "$enable_binreloc" = "no"; then
       AC_MSG_RESULT(no)
else
       AC_MSG_RESULT([no (unknown value "$enable_binreloc")])
       enable_binreloc=no
fi
AC_DEFINE(BR_PTHREADS,[0],[Use binreloc thread support?])
if test "$enable_binreloc" = "yes"; then
   AC_DEFINE(ENABLE_BINRELOC,,[Use AutoPackage?])
fi

AC_ARG_ENABLE(osxapp,
       [  --enable-osxapp         compile with OSX .app data dir paths],
       enable_osxapp=$enableval,enable_osxapp=no)
if test "$enable_osxapp" = "yes"; then
   AC_DEFINE(ENABLE_OSX_APP_LOCATIONS,,[Build with OSX .app data dir paths?])
fi

dnl ******************************
dnl   Reported by autoscan
dnl ******************************
AC_CHECK_FUNCS(pow)
# if we did not find pow(), see if it's in libm.
if test x"$ac_cv_func_pow" = x"no" ; then
	AC_CHECK_LIB(m,pow)
fi
AC_CHECK_FUNCS(sqrt)
AC_CHECK_FUNCS(floor)
AC_CHECK_FUNCS(gettimeofday)
AC_CHECK_FUNCS(memmove)
AC_CHECK_FUNCS(memset)
AC_CHECK_FUNCS(mkdir)
AC_CHECK_FUNCS(strncasecmp)
AC_CHECK_FUNCS(strpbrk)
AC_CHECK_FUNCS(strrchr)
AC_CHECK_FUNCS(strspn)
AC_CHECK_FUNCS(strstr)
AC_CHECK_FUNCS(strtoul)
AC_CHECK_FUNCS(fpsetmask)
AC_CHECK_FUNCS(ecvt)
AC_CHECK_HEADERS(ieeefp.h)
AC_CHECK_HEADERS(fcntl.h)
AC_CHECK_HEADERS(libintl.h)
AC_CHECK_HEADERS(stddef.h)
AC_CHECK_HEADERS(sys/time.h)
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_STRTOD
AC_HEADER_STAT
AC_HEADER_TIME
AC_STRUCT_TM
AC_TYPE_MODE_T
AC_TYPE_SIGNAL

dnl Work around broken gcc 3.3 (seen on OSX) where "ENABLE_NLS" isn't
dnl set correctly because the gettext function isn't noticed.
if test "$ac_cv_header_libintl_h" = "yes" &&
   test "$ac_cv_func_bind_textdomain_codeset" = "yes"  &&
   test "$gt_cv_func_have_gettext" != "yes"; then
    AC_DEFINE(ENABLE_NLS)
fi

dnl ******************************
dnl   Compilation warnings
dnl ******************************
if test "$GXX" = "yes"; then
  # Enable some warnings from g++.

  # Rationale: a number of bugs in inkscape have been fixed by enabling g++
  # warnings and addressing the produced warnings.  Usually the committing
  # developer is the best person to address the warnings.

  ink_svd_CXXFLAGS="$CXXFLAGS"
  CXXFLAGS="-Wno-unused-parameter $CXXFLAGS"
  # -Wno-unused-parameter isn't accepted by gcc 2.95.
  AC_COMPILE_IFELSE([int dummy;
], , CXXFLAGS="-Wno-unused $ink_svd_CXXFLAGS",)
  # Note: At least one bug has been caught from unused parameter warnings,
  # so it might be worth trying not to disable it.
  # One way of selectively disabling the warnings (i.e. only where the
  # programmer deliberately isn't using the parameter, e.g. for a callback)
  # is to remove the parameter name (leaving just its type), as is done
  # in src/seltrans.cpp:sp_seltrans_handle_event; this indicates that the
  # programmer deliberately has an unused parameter (e.g. because it's used
  # as a callback or similar function pointer use).

  # Add even more stuff
  CXXFLAGS="-Wpointer-arith -Wcast-align -Wsign-compare -Woverloaded-virtual -Wswitch $CXXFLAGS"

fi

dnl ******************************
dnl   libinkscape
dnl ******************************
dnl
dnl AC_ARG_ENABLE(libinkscape, AC_HELP_STRING([--enable-libinkscape], [Compile dynamic library (experimental)]), [splib=$enableval], [splib=no])
dnl
dnl AM_CONDITIONAL(ENABLE_LIBINKSCAPE, test "x$splib" != "xno")
dnl

AC_SUBST(INKSCAPE_CFLAGS)
AC_SUBST(INKSCAPE_LIBS)

dnl Define our data paths for config.h
AC_DEFINE_DIR([INKSCAPE_DATADIR], [datadir], [Base data directory])
AC_DEFINE_DIR([PACKAGE_LOCALE_DIR], [localedir], [Locatization directory])

AC_CONFIG_FILES([
Makefile
src/Makefile
src/check-header-compile
src/application/makefile
src/bind/makefile
src/debug/makefile
src/dialogs/makefile
src/display/makefile
src/dom/makefile
src/extension/implementation/makefile
src/extension/internal/makefile
src/extension/makefile
src/extension/script/makefile
src/filters/makefile
src/helper/makefile
src/io/makefile
src/libcroco/makefile
src/libgdl/makefile
src/libnr/makefile
src/libnrtype/makefile
src/libavoid/makefile
src/livarot/makefile
src/live_effects/makefile
src/live_effects/parameter/makefile
src/pedro/makefile
src/jabber_whiteboard/makefile
src/svg/makefile
src/trace/makefile
src/ui/cache/makefile
src/ui/dialog/makefile
src/ui/makefile
src/ui/view/makefile
src/ui/widget/makefile
src/util/makefile
src/widgets/makefile
src/xml/makefile
src/2geom/makefile
doc/Makefile
po/Makefile.in
share/Makefile
share/clipart/Makefile
share/examples/Makefile
share/extensions/Makefile
share/extensions/alphabet_soup/Makefile
share/extensions/Barcode/Makefile
share/extensions/Poly3DObjects/Makefile
share/extensions/test/Makefile
share/extensions/xaml2svg/Makefile
share/filters/Makefile
share/fonts/Makefile
share/gradients/Makefile
share/icons/Makefile
share/icons/application/Makefile
share/icons/application/16x16/Makefile
share/icons/application/22x22/Makefile
share/icons/application/24x24/Makefile
share/icons/application/32x32/Makefile
share/icons/application/48x48/Makefile
share/icons/application/256x256/Makefile
share/keys/Makefile
share/markers/Makefile
share/palettes/Makefile
share/patterns/Makefile
share/screens/Makefile
share/templates/Makefile
share/tutorials/Makefile
share/ui/Makefile
packaging/autopackage/default.apspec
inkscape.spec
Info.plist
inkview.1
])

AH_BOTTOM([


])

AC_OUTPUT

echo "
Configuration:

        Source code location:     ${srcdir}
        Destination path prefix:  ${prefix}
        Compiler:                 ${CXX}
        CPPFLAGS:                 ${CPPFLAGS}
        CXXFLAGS:                 ${CXXFLAGS}
        CFLAGS:                   ${CFLAGS}
        LDFLAGS:                  ${LDFLAGS}

        Use gnome-vfs:            ${gnome_vfs}
        Use openoffice files:     ${ij}
        Use relocation support:   ${enable_binreloc}
        Internal Python:          ${with_python}
        Internal Perl:            ${with_perl}
        Enable LittleCms:         ${enable_lcms}
        Enable Poppler-Cairo:     ${enable_poppler_cairo}
        ImageMagick Magick++:     ${magick_ok}
        Libwpg:                   ${with_libwpg}
        GTK+ backend target:      ${gtk_backend_target}
"
