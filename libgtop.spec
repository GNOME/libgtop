# Note that this is NOT a relocatable package
%define ver      0.25pre1
%define rel      SNAP
%define prefix   /usr

Summary: libgtop library
Name: libgtop
Version: %ver
Release: %rel
Copyright: LGPL
Group: X11/Libraries
Source: ftp://ftp.home-of-linux.org/pub/libgtop-%{ver}.tar.gz
BuildRoot: /tmp/libgtop-root
Packager: Martin Baulig <martin@home-of-linux.org>
URL: http://www.home-of-linux.org/gnome/libgtop/
Prereq: /sbin/install-info
Docdir: %{prefix}/doc

%description

A library that fetches information about the running system such as
cpu and memory usage, active processes etc.

On Linux systems, these information are taken directly from the /proc
filesystem while on other systems a server is used to read those
information from /dev/kmem or whatever. 

%changelog

* Sun Aug 16 1998 Martin Baulig <martin@home-of-linux.org>

- first version of the RPM

%prep
%setup

%build
# Needed for snapshot releases.
if [ ! -f configure ]; then
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --prefix=%prefix --without-linux-table
else
  CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%prefix --without-linux-table
fi

if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
rm -rf $RPM_BUILD_ROOT

make prefix=$RPM_BUILD_ROOT%{prefix} install

#
# msf - remove these as they are really supposed to come from gnome-libs
#
rm -f $RPM_BUILD_ROOT/%{prefix}/lib/libgnomesupport.a
rm -f $RPM_BUILD_ROOT/%{prefix}/lib/libgnomesupport.la
rm -f $RPM_BUILD_ROOT/%{prefix}/lib/libgnomesupport.so.0
rm -f $RPM_BUILD_ROOT/%{prefix}/lib/libgnomesupport.so.0.0.0

rm -fr $RPM_BUILD_ROOT/%{prefix}/include/libgtop

%clean
#rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)

%doc ANNOUNCE AUTHORS ChangeLog NEWS README README.LATEST copyright.txt doc
%{prefix}/bin/*
%{prefix}/lib/lib*.so.*
%{prefix}/lib/libgtopConf.sh
%{prefix}/lib/*a
%{prefix}/include/*
