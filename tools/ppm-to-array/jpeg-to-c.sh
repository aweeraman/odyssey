#!/bin/sh

CURDIR=$(dirname $(readlink -f "$0"))
JPEG_FILE=$1
HEADER_FILE=$2

if [ "$#" -lt "2" ]; then
	echo "Usage: $0 jpeg-file header-file.h"
	exit 0
fi

which jpegtopnm > /dev/null
if [ $? -ne 0 ]; then
	echo "jpegtopnm not found: to install, run 'apt-get install netbpm'"
	exit
fi

if [ ! -e ${CURDIR}/ppm-to-array ]; then
	echo "Building..."
	make -C ${CURDIR}
fi

if [ ! -e "${HEADER_FILE}" ]; then
	echo "$HEADER_FILE not found!"
	exit 1
fi

TMPFILE=$(mktemp)

jpegtopnm $JPEG_FILE > $TMPFILE
if [ $? -ne 0 ]; then
	echo "jpegtopnm returned $?"
fi

${CURDIR}/ppm-to-array $TMPFILE $HEADER_FILE
