dnl
dnl LIBGTOP_XML_HOOK (script-if-xml-found, failflag)
dnl
dnl If failflag is "failure", script aborts due to lack of XML
dnl 
dnl Check for availability of the libxml library
dnl the XML parser uses libz if available too
dnl

AC_DEFUN([LIBGTOP_XML_HOOK],[
	LIBGTOP_XML_LIB=
	AC_PATH_PROG(XML_CONFIG,xml-config,no)
	if test "$XML_CONFIG" = no; then
		if test x$2 = xfailure; then
			AC_MSG_ERROR(Could not find xml-config)
		else
			AC_MSG_WARN(Could not find xml-config)
		fi
	else
		AC_CHECK_LIB(xml, xmlNewDoc, [
			$1
			LIBGTOP_XML_LIB=`$XML_CONFIG --libs`
			AC_DEFINE(HAVE_LIBXML)
		], [
			if test x$2 = xfailure; then 
				AC_MSG_ERROR(Could not link sample xml program)
			else
				AC_MSG_WARN(Could not link sample xml program)
			fi
		], `$XML_CONFIG --libs`)
	fi
	AC_SUBST(LIBGTOP_XML_LIB)
])

AC_DEFUN([LIBGTOP_XML_CHECK], [
	LIBGTOP_XML_HOOK([],failure)
])
