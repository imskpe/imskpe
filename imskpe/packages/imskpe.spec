Name: imskpe
Summary:  imskpe - formantsynthesis gui
Version: 1.0.4
Release: 1
License: GPL
Group: Applications/Sound
URL: http://imskpe.sf.net
Source0: %{name}-%{version}.tar.gz
#Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root
Buildroot: /tmp/bol
#Requires:        gtk+ >= @GTK_REQUIRED@
#BuildRequires:   gtk+-devel >= @GTK_REQUIRED@
prefix: /usr/local

# cheats
%define _unpackaged_files_terminate_build 0 
%define _missing_doc_files_terminate_build 0

%description 
imskpe is gui for formantsynthesis based on algorthms by Dennis Klatt.
%prep
%setup -q
%build 
./configure --prefix=%{prefix}
make

%install 
rm -fr %{buildroot}

%makeinstall
make DESTDIR=%{buildroot} install

%clean 
[ "%{buildroot}" != '/' ] && rm -rf %{buildroot}

%files 
%defattr(-,root,root) 
%{prefix}/bin/imskpe
%{prefix}/share/locale/* 
%{prefix}/share/imskpe/pixmaps/* 
#%doc AUTHORS ChangeLog INSTALL README NEWS TODO

%changelog 

* Sun May 02 2010 Andreas Madsack <bolsog@users.sf.net>
- 1.0.2 Release
- including new Win32-Release
- fixed segfault on saving waves

* Fri Apr 27 2007 Andreas Madsack <bolsog@users.sf.net>
- 1.0.1 release

* Mon Oct 31 2005 Andreas Madsack <bolsog@users.sf.net>
- 1.0 release

* Mon Nov 22 2004 Andreas Madsack <bolsog@users.sf.net>
- first version
