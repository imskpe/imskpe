Name: imskpe
Summary:  imskpe - formantsynthesis gui
Version: 1.0beta1
Release: 1
License: GPL
Group: Applications/Sound
URL: http://imskpe.sf.net
Source0: %{name}-%{version}.tar.gz
Buildroot: %{_tmppath}/%{name}-%{version}-%{release}-root
Requires:        gtk+ >= @GTK_REQUIRED@
BuildRequires:   gtk+-devel >= @GTK_REQUIRED@

%description 
imskpe is gui for formantsynthesis based on algorthms by Dennis Klatt.
%prep
%setup -q
%build 
./configure --prefix=%{_prefix}
make

%install 
rm -fr %{buildroot}

%makeinstall
make DESTDIR=%{buildroot} install

%clean 
[ "%{buildroot}" != '/' ] && rm -rf %{buildroot}

%files 
%defattr(-,root,root) 
%{_prefix}/bin/imskpe
%{_datadir}/* 
%doc AUTHORS ChangeLog INSTALL README NEWS TODO

%changelog 

* Mon Nov 22 2004 Andreas Madsack <bolsog@users.sf.net>
- first version
