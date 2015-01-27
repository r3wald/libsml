#!/bin/bash

libtoolize --copy

autoheader
aclocal -I m4 --install
autoconf

#automake --foreign --add-missing --force-missing --copy
