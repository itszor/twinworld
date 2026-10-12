#!/bin/sh
# Code/data analysis of Twinworld's !RunImage with SuperSpider
# (~/stuff/superspider): twinworld.json, which recomp/'s "make regen" translates
# to C with superspider gen.
# twinworld.roots adds entry points to the analysis (recomp/: "make roots" merges
# the gap entries a --trace-gaps run logged into it).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
IMG="$HERE/../game/!TwinWorld/!RunImage,fffff842-00000000"
(cd "$SUPERSPIDER" && dune build)
"$SUPERSPIDER/_build/default/bin/main.exe" analyse "$IMG" 8000 --full --roots "$HERE/twinworld.roots" -o "$HERE/twinworld.json"
