#!/bin/sh

# Copyright 2008 Paul Wise
# Copyright 2009 Konstantin Dmitriev
#
# This file is part of Synfig.
#
# Synfig is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Synfig is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Synfig.  If not, see <https://www.gnu.org/licenses/>.

get_git_id(){
	export SCM=git
	export REVISION_ID=`cd "$1"; git log --no-color -1 | head -n 1 | cut -f 2 -d ' ' | cut -c -6`
	export BRANCH="`cd "$1"; git branch -a --no-color --contains HEAD | sed -e s/\*\ // | sed -e s/\(no\ branch\)// | tr '\n' ' ' | tr -s ' ' | sed s/^' '//`"
	if ( echo $BRANCH | egrep origin/master > /dev/null ); then
		#give a priority to master branch
		BRANCH='master'
	else
		BRANCH=`echo $BRANCH | cut -d ' ' -f 1`
		BRANCH=${BRANCH##*/}
	fi
	export REVISION=`git show --pretty=format:%ci HEAD |  head -c 10 | tr -d '-'`
	export COMPARE=`git rev-parse --git-dir`
	# The extra * at the end is for Modified
	#REVISION="$REVISION"`cd "$1"; [[ $(git status 2> /dev/null | tail -n1) != "nothing to commit (working directory clean)" ]] && echo "*"`
}

HEADER="$2/autorevision.h"
SCM=none


if [ ! -f "$HEADER" ] ; then
	touch -t 197001010101 "$HEADER"
fi


# Extract the revision from git
if git rev-parse --git-dir > /dev/null  2>&1 ; then
	get_git_id "."
fi


# Allow overriding both REVISION and DEVEL_VERSION
if [ -f "$2/autorevision.conf" ] ; then
	SCM=manual
	. "$2/autorevision.conf"
fi


# Abort if the header is newer
if [ "$COMPARE" -ot "$HEADER" ] ; then exit; fi

# Set the development version string
if [ x = "x$DEVEL_VERSION" ] ; then
	if [ x != "x$REVISION" ] ; then
		if [ $SCM = git ] ; then
			DEVEL_VERSION="Revision: ${REVISION}\\\\nBranch: ${BRANCH}\\\\nRevision ID: ${REVISION_ID}"
		elif [ $SCM = manual ] ; then
			DEVEL_VERSION="$REVISION (manually configured)"
		fi
	fi
fi


# Output the header
if [ x != "x$DEVEL_VERSION" ] ; then
	printf "#define DEVEL_VERSION \"$DEVEL_VERSION\"\n" > "$HEADER"
fi
