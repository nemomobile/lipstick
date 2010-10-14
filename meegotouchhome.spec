Name:           meegotouchhome
Version:        0.21.8
Release:        1%{?dist}
Summary:        MeeGo Touch Homescreen

Group:          User Interface/Desktops
License:        LGPL v2.1
URL:            http://meego.gitorious.org/meegotouch/meegotouch-home

Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: pkgconfig(meegotouch) >= 0.20
BuildRequires: pkgconfig(QtCore) >= 4.6.0
BuildRequires: pkgconfig(contextprovider-1.0)
BuildRequires: pkgconfig(xcomposite)
BuildRequires: pkgconfig(xdamage)
BuildRequires: pkgconfig(xext)
BuildRequires: qt-devel
BuildRequires: doxygen
#BuildRequires: maemo-services

%description
The home screen for the MeeGo Touch environment

%package -n meegotouchhome
Summary: MeeGo Touch Homescreen
%description -n meegotouchhome
MeeGo Touch Homescreen

%package -n meegotouchhome-l10n-eng-en
Summary: MeeGo Touch Homescreen engineering English messages
Provides: meegotouchhome-l10n
%description -n meegotouchhome-l10n-eng-en
MeeGo Touch Homescreen engineering English messages

%package -n meegotouchhome-l10n-fi
Summary: MeeGo Touch Homescreen Finnish messages
Provides: meegotouchhome-l10n
%description -n meegotouchhome-l10n-fi
MeeGo Touch Homescreen Finnish messages

%package -n meegotouchhome-tests
Summary: MeeGo Touch Homescreen unit tests
%description -n meegotouchhome-tests
MeeGo Touch Homescreen unit tests

%package -n libmeegotouchhome
Summary: MeeGo Touch Homescreen library
%description -n libmeegotouchhome
Meego Touch home library

%package -n libmeegotouchhome-devel
Summary: MeGgo Touch Homescreen library development files
Group: Development/Desktops
%description -n libmeegotouchhome-devel
Development files for MeeGo Touch Homescreen

%package -n libmeegotouchhome-doc
Summary: Meego Touch Homescreen library API documentation
%description -n libmeegotouchhome-doc
Meego Touch Homescreen library API documentation

%prep
%setup -q -n %{name}-%{version}

%build
qmake -recursive
make %{?_smp_mflags}
make %{?_smp_mflags} doc

%install
rm -fR %{buildroot}
make INSTALL_ROOT=%{buildroot} install
mkdir -p %{buildroot}/usr/share/doc/libmeegotouchhome/api
cp -r doc/html/* %{buildroot}/usr/share/doc/libmeegotouchhome/api/
rm -f %{buildroot}/usr/share/doc/libmeegotouchhome/api/installdox
install -m 644 -D -p LICENSE.LGPL %{buildroot}/usr/share/doc/packages/%{name}/LICENSE.LGPL

%clean
rm -fR %{buildroot}

%post -n libmeegotouchhome
/sbin/ldconfig

%postun -n libmeegotouchhome
/sbin/ldconfig

%files
%defattr(-,root,root)
%dir /usr/share/doc/packages/%{name}
%doc /usr/share/doc/packages/%{name}/*

%files -n meegotouchhome
%defattr(-,root,root)
%dir /usr/share/themes/base/meegotouch/meegotouchhome
%dir /usr/share/themes/base/meegotouch/meegotouchhome/images
%dir /usr/share/themes/base/meegotouch/meegotouchhome/style
%dir /usr/share/themes/base/meegotouch/meegotouchhome/svg
%dir /usr/lib/meegotouch/applicationextensions
%dir /usr/share/meegotouch/applicationextensions
%dir /usr/share/themes/base/meegotouch/libmeegotouchhome-plaindesktopbackgroundextension/style
/usr/bin/meegotouchhome
/usr/share/themes/base/meegotouch/meegotouchhome/*.conf
/usr/share/themes/base/meegotouch/meegotouchhome/images/*.png
/usr/share/themes/base/meegotouch/meegotouchhome/style/*.css
/usr/share/themes/base/meegotouch/meegotouchhome/svg/*.svg
/usr/lib/meegotouch/applicationextensions/*.so
/usr/share/meegotouch/applicationextensions/*.desktop
/usr/share/themes/base/meegotouch/libmeegotouchhome-plaindesktopbackgroundextension/style/*.css

%files -n meegotouchhome-l10n-eng-en
%defattr(-,root,root)
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/libmeegotouchhome.qm

%files -n meegotouchhome-l10n-fi
%defattr(-,root,root)
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/libmeegotouchhome_fi.qm

%files -n meegotouchhome-tests
%defattr(-,root,root)
%dir /usr/share/meegotouchhome-tests
%dir /usr/lib/meegotouchhome-tests
/usr/share/meegotouchhome-tests/*
/usr/lib/meegotouchhome-tests/*

%files -n libmeegotouchhome
%defattr(-,root,root)
%{_libdir}/libmeegotouchhome.so*

%files -n libmeegotouchhome-devel
%defattr(-,root,root)
#%dir /usr/include/meegotouchhome
#/usr/include/meegotouchhome/*

%files -n libmeegotouchhome-doc
%defattr(-,root,root)
%dir /usr/share/doc/libmeegotouchhome
%doc /usr/share/doc/libmeegotouchhome/*

%changelog
