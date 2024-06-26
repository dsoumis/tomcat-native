#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

dnl
dnl Process this file with autoconf to produce a configure script
dnl

AC_PREREQ(2.68)
AC_INIT(configure.ac)

AC_CONFIG_AUX_DIR(build)

sinclude(build/apr_common.m4)
sinclude(build/tcnative.m4)
sinclude(build/find_apr.m4)

dnl Generate ./config.nice for reproducing runs of configure
dnl
APR_CONFIG_NICE(config.nice)

dnl # Some initial steps for configuration.  We setup the default directory
dnl # and which files are to be configured.

dnl Absolute source/build directory
abs_srcdir=`(cd $srcdir && pwd)`
abs_builddir=`pwd`

if test "$abs_builddir" != "$abs_srcdir"; then
  USE_VPATH=1
  TCN_CONFIG_LOCATION=build
else
  TCN_CONFIG_LOCATION=source
fi

AC_SUBST(TCN_CONFIG_LOCATION)
AC_CANONICAL_TARGET
AC_PROG_INSTALL

dnl Generate the libtool script which is needed for rlibtool
LT_INIT

dnl
dnl compute the top directory of the build
dnl note: this is needed for LIBTOOL and exporting the bundled Expat
dnl
top_builddir="$abs_builddir"
AC_SUBST(top_builddir)
AC_SUBST(abs_srcdir)
AC_SUBST(abs_builddir)

dnl Initialize mkdir -p functionality.
APR_MKDIR_P_CHECK($abs_srcdir/build/mkdir.sh)

dnl get our version information
get_version="$abs_srcdir/build/get-version.sh"
version_hdr="$abs_srcdir/include/tcn_version.h"
TCNATIVE_MAJOR_VERSION="`$get_version major $version_hdr TCN`"
TCNATIVE_DOTTED_VERSION="`$get_version all $version_hdr TCN`"
TCNATIVE_LIBTOOL_VERSION="`$get_version libtool $version_hdr TCN`"

AC_SUBST(TCNATIVE_DOTTED_VERSION)
AC_SUBST(TCNATIVE_MAJOR_VERSION)
AC_SUBST(TCNATIVE_LIBTOOL_VERSION)

echo "Tomcat Native Version: ${TCNATIVE_DOTTED_VERSION}"

dnl Enable the layout handling code, then reparse the prefix-style
dnl arguments due to autoconf being a PITA.
APR_ENABLE_LAYOUT(tcnative)
APR_PARSE_ARGUMENTS

dnl
dnl set up the compilation flags and stuff
dnl

TCNATIVE_INCLUDES=""
TCNATIVE_PRIV_INCLUDES="-I$srcdir/include"

dnl
dnl Find the APR includes directory and (possibly) the source (base) dir.
dnl
TCN_FIND_APR

dnl
dnl even though we use apr_rules.mk for building apr-util, we need
dnl to grab CC and CPP ahead of time so that apr-util config tests
dnl use the same compiler as APR; we need the same compiler options
dnl and feature test macros as well
dnl
APR_SETIFNULL(CC, `$apr_config --cc`)
APR_SETIFNULL(CPP, `$apr_config --cpp`)

AC_PROG_INSTALL

dnl
dnl  Find the JVM related information
dnl
NEED_JNI_MD=yes
TCN_FIND_JAVA

dnl MAC OS X does not used include but Headers
if test -d ${JAVA_HOME}/Headers; then
  JAVA_INC=Headers
else
  JAVA_INC=include
fi
APR_ADDTO(TCNATIVE_PRIV_INCLUDES,[-I$JAVA_HOME/$JAVA_INC])

dnl sableVM does not have/need $JAVA_OS/jni_md.h
if test "$NEED_JNI_MD" = "yes"; then
  TCN_FIND_JDK_OS
  if test -z "${JAVA_OS}"; then
    AC_MSG_RESULT([jni_md.h found in $JAVA_HOME/$JAVA_INC])
  else
    APR_ADDTO(TCNATIVE_PRIV_INCLUDES,[-I$JAVA_HOME/$JAVA_INC/$JAVA_OS])
  fi
fi

AC_SUBST(JAVA_HOME)
AC_SUBST(JAVA_OS)


AC_ARG_ENABLE(insecure-export-ciphers,
[AS_HELP_STRING([--enable-insecure-export-ciphers],[allow including insecure export and null ciphers in the cipher string (default is disabled=not allowed)])],
[
  case "${enableval}" in
    yes )
       APR_ADDTO(CFLAGS, [-DHAVE_EXPORT_CIPHERS])
       AC_MSG_WARN([Enabling insecure export and null cipher support])
       ;;
  esac
])

dnl
dnl  Find the OpenSSL installation
dnl
TCN_FIND_SSL_TOOLKIT

so_ext=$APR_SO_EXT
lib_target=$APR_LIB_TARGET
AC_SUBST(so_ext)
AC_SUBST(lib_target)

TCNATIVE_LIBNAME="tcnative${libsuffix}"
AC_SUBST(TCNATIVE_LIBNAME)

EXTRA_OS_LINK=""
host_alias=`uname -s`
case "$host_alias" in
    dnl ### BeOS requires that ALL symbols resolve at LINK time!
    dnl ###
    dnl ### So, if we're building on BeOS then we need to add in the
    dnl ### apr and expat libraries to the build or it'll die a truly horrible
    dnl ### death. We now use the apr-config tool to determine the correct
    dnl ### library to link against :)
*AIX*|*Darwin*|*BeOS*)
    dnl need such stuff as -liconv to be specified when building libaprutil.la
    EXTRA_OS_LINK='$(TCNATIVE_LDFLAGS) $(TCNATIVE_LIBS)'
    ;;
*)
    ;;
esac

AC_SUBST(EXTRA_OS_LINK)

dnl CFLAGS for maintainer mode
dnl it also allows the CFLAGS environment variable.
CFLAGS="${CFLAGS}"
AC_ARG_ENABLE(maintainer-mode,
[AS_HELP_STRING([--enable-maintainer-mode],[Turn on debugging and compile time warnings])],
[
  case "${enableval}" in
    yes )
       if test "$GCC" = "yes"; then
         CFLAGS="${CFLAGS} -DDEBUG -Wall"
       else
         CFLAGS="${CFLAGS} -DDEBUG"
       fi
       AC_MSG_RESULT([...Enabling Maintainer mode...])
       ;;
  esac
])

dnl
dnl Prep all the flags and stuff for compilation and export to other builds
dnl
APR_ADDTO(TCNATIVE_LIBS, [$LIBS])
APR_ADDTO(TCNATIVE_LIBS, [$APR_LIBTOOL_LIBS])
APR_ADDTO(TCNATIVE_LDFLAGS, [$LDFLAGS])

# Link libkstat for Solaris
case $host in
    *-solaris2*)
        APR_ADDTO(TCNATIVE_LIBS, -lkstat)
        ;;
    *linux*)
        APR_ADDTO(CFLAGS, -DTCNATIVE_LINUX)
        ;;
    *)
        ;;
esac

TCN_FIND_APR_FEATURE([apr_pollset_wakeup],[POLLSET_WAKEUP])

AC_SUBST(TCNATIVE_EXPORT_LIBS)
AC_SUBST(TCNATIVE_PRIV_INCLUDES)
AC_SUBST(TCNATIVE_INCLUDES)
AC_SUBST(TCNATIVE_LDFLAGS)
AC_SUBST(TCNATIVE_LIBS)
AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)

dnl copy apr's rules.mk into our build directory.
if test ! -d ./build; then
   $mkdir_p build
fi
cp $APR_BUILD_DIR/apr_rules.mk $abs_builddir/build/rules.mk


dnl
dnl BSD/OS (BSDi) needs to use a different include syntax in the Makefiles
dnl
case "$host_alias" in
*bsdi* | BSD/OS)
    # Check whether they've installed GNU make
    if make --version > /dev/null 2>&1; then
        INCLUDE_RULES="include $abs_builddir/build/rules.mk"
        INCLUDE_OUTPUTS="include $abs_srcdir/build-outputs.mk"
    else
        INCLUDE_RULES=".include \"$abs_builddir/build/rules.mk\""
        INCLUDE_OUTPUTS=".include \"$abs_srcdir/build-outputs.mk\""
    fi
    ;;
*)
    INCLUDE_RULES="include $abs_builddir/build/rules.mk"
    INCLUDE_OUTPUTS="include $abs_srcdir/build-outputs.mk"
    ;;
esac

AC_SUBST(INCLUDE_RULES)
AC_SUBST(INCLUDE_OUTPUTS)

if test -d $srcdir/test; then
    test_Makefile="test/Makefile"
fi

dnl
dnl everything is done.
MAKEFILES="Makefile"
AC_CONFIG_FILES([
    $MAKEFILES
	])
AC_CONFIG_COMMANDS([default],[
TCNATIVE_MAJOR_VERSION=$TCNATIVE_MAJOR_VERSION
],[])
AC_OUTPUT
