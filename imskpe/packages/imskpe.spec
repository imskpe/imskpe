Name: imskpe
Summary:  imskpe - formantsynthesis gui
Version: 1.0beta1
Release: 1
License: GPL
Group: Applications
URL: http://imskpe.sf.net
Source0: %{name}-%{version}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root
Requires: GTK+-2.4.0

%description 
imskpe is gui for formantsynthesis based on algorthms by Dennis Klatt.
%build 
%configure --prefix=/usr/local
make

%install 
rm -fr %{buildroot}

%makeinstall

%clean 
rm -fr %{buildroot}

%files 
%defattr(-,root,root) 
%{_bindir}/* 
%{_mandir}/* 
%{_datadir}/* 
%{_doc}/*

%changelog 

* So Nov 21 2004 Andreas Madsack <bolsog@users.sf.net>
- first version
