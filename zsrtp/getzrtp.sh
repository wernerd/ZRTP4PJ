#!/bin/sh
if ! test -d zrtp; then
    git clone git://github.com/wernerd/ZRTPCPP.git zrtp
else
    cd zrtp
    if ! test -d .git; then
        echo "ZRTP source directory inconsistent."
        echo "Remove zrtp directory and call this script again"
        exit 1
    fi
    git pull
fi
