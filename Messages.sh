#!/usr/bin/env bash
$EXTRACTRC *.kcfg *.rc *.ui  >> rc.cpp || exit 11
$XGETTEXT *.cpp -o $podir/kollision.pot
rm -f rc.cpp
