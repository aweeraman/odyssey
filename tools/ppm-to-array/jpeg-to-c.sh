#!/bin/sh
# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

CURDIR=$(dirname $(readlink -f "$0"))

if [ "$#" -lt "2" ]; then
	echo "Usage: $0 jpeg-file header-file.h"
	exit 0
fi

JPEG_FILE=$1
HEADER_FILE=$2

which jpegtopnm > /dev/null
if [ $? -ne 0 ]; then
	echo "jpegtopnm not found: to install, run 'apt-get install netbpm'"
	exit
fi

if [ ! -e ${CURDIR}/ppm-to-array ]; then
	echo "Building..."
	make -C ${CURDIR}
fi

if [ ! -e "${JPEG_FILE}" ]; then
	echo "$JPEG_FILE not found!"
	exit 1
fi

TMPFILE=$(mktemp)

jpegtopnm $JPEG_FILE > $TMPFILE
if [ $? -ne 0 ]; then
	echo "jpegtopnm returned $?"
fi

${CURDIR}/ppm-to-array $TMPFILE $HEADER_FILE
