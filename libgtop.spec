# Note that this is NOT a relocatable package
%define ver      0.01
%define rel      SNAP
%define prefix   /usr

Summary: libgtop library
Name: libgtop
Version: %ver
Release: %rel
Copyright: LGPL
Group: X11/Libraries
Source: ftp://ftp.gnome.org/pub/libgtop-%{ver}.tar.gz
BuildRoot: /tmp/libgtop-root
Packager: Michael Fulbright <msf@redhat.com>
URL: http://www.gnome.org
Prereq: /sbin/install-info
Docdir: %{prefix}/doc

%description

A library that fetches 
information about the running system such as cpu and memory usage,
active processes etc. On Linux systems, these information are taken
directly from the /proc filesystem. 


%changelog

* Tue Jun  2 1998 Michael Fulbright <msf@redhat.com>

- first version of the RPM

%prep
%setup

%build
# Needed for snapshot releases.
if [ ! -f configure ]; then
  CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh --prefix=%prefix --disable-libgtop-examples
else
  CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%prefix --disable-libgtop-examples
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
#rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)

%doc ANNOUNCE AUTHORS ChangeLog NEWS README README.LATEST copyright.txt
%docdir doc
%{prefix}/bin/*
%{prefix}/lib/lib*.so.*
%{prefix}/share/*
%{prefix}/lib/libgtop/*
%{prefix}/lib/libgtopConf.sh
%{prefix}/lib/*a
%{prefix}/include/*
