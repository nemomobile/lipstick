Name:       lipstick-qt5

# >> macros
# << macros

Summary:    QML toolkit for homescreen creation
Version:    0.11.0
Release:    1
Group:      System/Libraries
License:    LGPLv2.1
URL:        http://github.com/nemomobile/lipstick
Source0:    %{name}-%{version}.tar.bz2
Requires:   mce >= 1.12.4
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Declarative)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(Qt5Sql)
BuildRequires:  pkgconfig(Qt5SystemInfo)
BuildRequires:  pkgconfig(Qt5Sensors)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(contentaction-0.1)
BuildRequires:  pkgconfig(mlite5) >= 0.0.6
BuildRequires:  pkgconfig(mce) >= 1.12.2
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  pkgconfig(libresourceqt1)
BuildRequires:  pkgconfig(ngf-qt5)
BuildRequires:  qt5-qttools-linguist
BuildRequires:  doxygen
Conflicts:   meegotouch-systemui < 1.5.7
Obsoletes:   libnotificationsystem0

%description
A QML toolkit for homescreen creation

%package devel
Summary:    Development files for lipstick
License:    LGPLv2.1
Group:      System/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Files useful for building homescreens.


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
export QT_SELECT=5
# << build pre

%qmake5

make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake_install

# >> install post
# << install post


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/liblipstick-qt5.so.*
%{_libdir}/qt5/imports/org/nemomobile/lipstick/liblipstickplugin.so
%{_libdir}/qt5/imports/org/nemomobile/lipstick/qmldir
%exclude %config %{_sysconfdir}/dbus-1/system.d/lipstick.conf
%exclude %{_datadir}/translations/lipstick_eng_en.qm
%exclude %{_datadir}/lipstick/notificationcategories/*.conf
%exclude /opt/tests/lipstick-tests/*
%exclude %{_bindir}/notificationtool
%exclude %{_datadir}/doc/lipstick/*
%exclude %{_datadir}/translations/source/lipstick.ts
%exclude %{_libdir}/debug/*
%exclude %{_libdir}/debug/.build-id/*
# >> files
# << files

%files devel
%defattr(-,root,root,-)
%{_includedir}/lipstick-qt5/*.h
%{_libdir}/liblipstick-qt5.so
%{_libdir}/liblipstick-qt5.prl
%{_libdir}/pkgconfig/lipstick-qt5.pc
# >> files devel
# << files devel
