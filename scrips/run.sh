#!/bin/sh

BASEPATH="/tmp/lbin"

mkdir -p "$BASEPATH"

premake5 gmake && make && LBIN_BASEPATH="$BASEPATH" ./bin/Debug/lbin "$@"

