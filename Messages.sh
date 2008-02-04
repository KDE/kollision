#!/usr/bin/env bash
$EXTRACTRC *.kcfg *.rc *.ui  >> rc.cpp || exit 11
$XGETTEXT *.cpp *.h -o $podir/kollision.pot

