dnl Process this file with autoconf to produce a configure script.

AC_INIT(retro-gtk, 0.6)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AC_PROG_CC

AM_PROG_AR

AM_PROG_VALAC

GOBJECT_INTROSPECTION_CHECK([0.6.7])

dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=retro-gtk
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT




AC_DISABLE_STATIC

LT_INIT


pkg_modules="gobject-2.0 glib-2.0 gtk+-3.0 cairo clutter-gtk-1.0 libpulse retro-gobject-0.6"
AC_SUBST(pkg_modules)
PKG_CHECK_MODULES(RETRO_GTK, [gobject-2.0 glib-2.0 gtk+-3.0 cairo clutter-gtk-1.0 libpulse retro-gobject-0.6])





AC_OUTPUT([
Makefile
retro-gtk/retro-gtk-0.6.pc
retro-gtk/Makefile
po/Makefile.in])
