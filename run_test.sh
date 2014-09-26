#!/bin/sh

if [ ! -f Makefile ]
then
    echo "Run configure..."
    ./configure --silent
fi

echo "Run make..."
make --silent
cd test/
../src/test
