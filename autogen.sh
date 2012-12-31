#!/bin/bash

set -e

B=$(pwd)
S=${0%/*}

V=$(head -n 1 ${S}/configure.ac | cut -d , -f 2)

verbose() {
	echo ">>> Running ${@} ..."
	${@}
}

cd ${S}
rm -fr autom4te.cache
verbose autoreconf --force --install

cd ${B}
verbose ${S}/configure \
	--srcdir=${S} \
	--enable-maintainer-mode \
	"${@}"
