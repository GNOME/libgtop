# Note that this is NOT a relocatable package
%define ver      0.01
%define rel      SNAP
%define prefix   /usr

Summary: GNOME Top Library
Name: libgtop
Version: %ver
Release: %rel
Copyright: LGPL
Group: X11/gnome
Source: ftp://ftp.gnome.org/pub/libgtop-%{ver}.tar.gz
BuildRoot: /tmp/libgtop-root
Obsoletes: gnome
Packager: Marc Ewing <marc@redhat.com>
URL: http://www.gnome.org/
Docdir: %{prefix}/doc

%description
Library that fetches information about the running system such as cpu
and memory usage, active processes etc. On Linux systems, these information
are taken directly from the /proc filesystem. For other systems such as
Solaris, where such programs need to be suid root (or only setgid kmem/mem
on some systems), it provides a suid/setgid server that fetches those
information and a client-side library that talks to this server.

Main idea was to have the same interface for all operating systems,
thus all system dependent details are hidden in the implementation
of that server.

%package devel
Summary: Libraries, includes, etc to use libgtop in GNOME applications
Group: X11/gnome
Requires: libgtop
Obsoletes: gnome

%description devel
Libraries, include files, etc you need to use libgtop in GNOME applications.

%prep
%setup

%build
# Needed for snapshot releases.
if [ ! -f configure ]; then
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --prefix=%prefix
else
  CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%prefix
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

%clean
rm -rf $RPM_BUILD_ROOT

%post 
if ! grep %{prefix}/lib /etc/ld.so.conf > /dev/null ; then
  echo "%{prefix}/lib" >> /etc/ld.so.conf
fi

/sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)

%doc AUTHORS COPYING ChangeLog NEWS README
%{prefix}/lib/lib*.so.*
%{prefix}/bin/*

%files devel
%defattr(-, root, root)

%{prefix}/lib/lib*.so
%{prefix}/lib/*a
%{prefix}/lib/*.sh
%{prefix}/lib/libgtop
%{prefix}/include/*
