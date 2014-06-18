#!/bin/bash

PACKAGE=jwmtools
VERSION=
DATE="`date -uI`"

if [ -z "$VERSION" ]; then
	VERSION='0.1'
	if [ -x "`which git 2>/dev/null`" -a -d .git ]; then
		DATE=$(git log --date=iso|grep '^Date:'|head -1|awk '{print$2}')
		VERSION="0.1.$(git rev-list --count HEAD)"
		(
		   echo -e "# created with git log --stat=76 -M -C|fmt -sct -w80\n"
		   git log --stat=76 -M -C|fmt -sct -w80
		)>ChangeLog
		(
		   echo "@PACKAGE@ -- authors file.  @DATE@"
		   echo ""
		   git log|grep '^Author:'|awk '{if(!authors[$0]){print$0;authors[$0]=1;}}'|tac
		)>AUTHORS.in
	fi
fi

/bin/cp -f configure.ac configure.in

sed -r -e "s:AC_INIT\([[]$PACKAGE[]],[[][^]]*[]]:AC_INIT([$PACKAGE],[$VERSION]:
	   s:AC_REVISION\([[][^]]*[]]\):AC_REVISION([$VERSION]):
	   s:^DATE=.*$:DATE='$DATE':" \
	  configure.in >configure.ac

rm -f configure.in

subst="s:@PACKAGE@:$PACKAGE:g
       s:@VERSION@:$VERSION:g
       s:@DATE@:$DATE:g"

sed -r -e "$subst" README.in >README
sed -r -e "$subst" NEWS.in >NEWS
sed -r -e "$subst" AUTHORS.in >AUTHORS
sed -r -e "$subst" THANKS.in >THANKS
sed -r -e "$subst" TODO.in >TODO

mkdir -p m4 2>/dev/null

autoreconf -iv
