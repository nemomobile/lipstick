Name:           duihome
Version:        0.21.8
Release:        1%{?dist}
Summary:        MeeGo Touch Homescreen

Group:          User Interface/Desktops
License:        LGPL v2.1
URL:            http://qt.gitorious.org/maemo-6-ui-framework/duihome

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

%package -n duihomescreen
Summary: MeeGo Touch Homescreen
%description -n duihomescreen
MeeGo Touch Homescreen

%package -n duihomescreen-l10n-eng-en
Summary: MeeGo Touch Homescreen engineering English messages
Provides: duihomescreen-l10n
%description -n duihomescreen-l10n-eng-en
MeeGo Touch Homescreen engineering English messages

%package -n duihomescreen-l10n-fi
Summary: MeeGo Touch Homescreen Finnish messages
Provides: duihomescreen-l10n
%description -n duihomescreen-l10n-fi
MeeGo Touch Homescreen Finnish messages

%package -n duihomescreen-applets
Summary: MeeGo Touch Homescreen Demo Applets
%description -n duihomescreen-applets
MeeGo Touch Homescreen Demo Applets

%package -n duihomescreen-applets-l10n-eng-en
Summary: MeeGo Touch Homescreen Demo Applets engineering English messages
%description -n duihomescreen-applets-l10n-eng-en
MeeGo Touch Homescreen Demo Applets engineering English messages

%package -n duihomescreen-applets-l10n-fi
Summary: MeeGo Touch Homescreen Demo Applets Finnish messages
%description -n duihomescreen-applets-l10n-fi
MeeGo Touch Homescreen Demo Applets Fin messages

%package -n duihomescreen-tests
Summary: MeeGo Touch Homescreen unit tests
%description -n duihomescreen-tests
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
mkdir -p %{buildroot}/usr/share/doc/libduihome/api
cp -r doc/html/* %{buildroot}/usr/share/doc/libduihome/api/
rm -f %{buildroot}/usr/share/doc/libduihome/api/installdox
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

%files -n duihomescreen
%defattr(-,root,root)
%dir /usr/share/themes/base/meegotouch/duihome
%dir /usr/share/themes/base/meegotouch/duihome/images
%dir /usr/share/themes/base/meegotouch/duihome/style
%dir /usr/share/themes/base/meegotouch/duihome/svg
%dir /usr/lib/meegotouch/applicationextensions
%dir /usr/share/meegotouch/applicationextensions
%dir /usr/share/themes/base/meegotouch/libduihome-plaindesktopbackgroundextension/style
/usr/bin/duihome
/usr/share/themes/base/meegotouch/duihome/*.conf
/usr/share/themes/base/meegotouch/duihome/images/*.png
/usr/share/themes/base/meegotouch/duihome/style/*.css
/usr/share/themes/base/meegotouch/duihome/svg/*.svg
/usr/lib/meegotouch/applicationextensions/*.so
/usr/share/meegotouch/applicationextensions/*.desktop
/usr/share/themes/base/meegotouch/libduihome-plaindesktopbackgroundextension/style/*.css

%files -n duihomescreen-l10n-eng-en
%defattr(-,root,root)
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/duihome.qm

%files -n duihomescreen-l10n-fi
%defattr(-,root,root)
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/duihome_fi.qm

%files -n duihomescreen-applets
%defattr(-,root,root)
%dir /usr/lib/meegotouch/applets
%dir /usr/share/meegotouch/applets
%dir /usr/share/meegotouch/applets/settings
%dir /usr/share/themes/base/meegotouch/imageapplet
%dir /usr/share/themes/base/meegotouch/imageapplet/images
%dir /usr/share/themes/base/meegotouch/weatherappletres
%dir /usr/share/themes/base/meegotouch/weatherappletres/images
%dir /usr/share/themes/base/meegotouch/weatherappletres/style
/usr/lib/meegotouch/applets/*.so
/usr/share/meegotouch/applets/settings/*.xml
/usr/share/meegotouch/applets/*.desktop
/usr/share/themes/base/meegotouch/imageapplet/images/*.png
/usr/share/themes/base/meegotouch/weatherappletres/images/*.png
/usr/share/themes/base/meegotouch/weatherappletres/style/*.css
/usr/share/themes/base/meegotouch/weatherappletres/*.conf

%files -n duihomescreen-applets-l10n-eng-en
%defattr(-,root,root)
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/weatherappletres.qm
/usr/share/l10n/meegotouch/weatherappletres_en.qm

%files -n duihomescreen-applets-l10n-fi
%defattr(-,root,root)
%dir /usr/share/l10n/meegotouch
/usr/share/l10n/meegotouch/weatherappletres_fi.qm

%files -n duihomescreen-tests
%defattr(-,root,root)
%dir /usr/share/duihomescreen-tests
%dir /usr/lib/duihomescreen-tests
/usr/share/duihomescreen-tests/*
/usr/lib/duihomescreen-tests/*

%files -n libmeegotouchhome
%defattr(-,root,root)
%{_libdir}/libmeegotouchhome.so*

%files -n libmeegotouchhome-devel
%defattr(-,root,root)
#%dir /usr/include/meegotouchhome
#/usr/include/meegotouchhome/*

%files -n libmeegotouchhome-doc
%defattr(-,root,root)
%dir /usr/share/doc/libduihome
%doc /usr/share/doc/libduihome/*

%changelog
