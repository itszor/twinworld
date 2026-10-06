#!/bin/sh
# Code/data classification for Twinworld's !RunImage with SuperSpider
# (~/stuff/superspider) - no Ghidra, no hand seeds. Produces the artifacts
# recomp/gen_c.py reads:
#   twinworld.json(.pickle)  via SuperSpider's phase 6 adapter, which builds a
#                            spider.Spider object and runs export_ghidra.py
#   out/functions.tsv
# twinworld.roots adds entry points to the analysis (recomp/: "make roots" merges
# the gap entries a --trace-gaps run logged into it).
# spider.py / export_ghidra.py / gen_asm.py are copies of the Lemmings
# project's (gen_c.py imports them; the pickle refers to spider.Spider).
set -e
HERE="$(cd "$(dirname "$0")" && pwd)"
SUPERSPIDER="${SUPERSPIDER:-$HOME/stuff/superspider}"
IMG="$HERE/../extracted/!TwinWorld/!RunImage,fffff842-00000000"
(cd "$SUPERSPIDER" && dune build)
"$SUPERSPIDER/_build/default/bin/main.exe" s1b "$IMG" 8000 --roots "$HERE/twinworld.roots" -o "$HERE/ss.json"
python3 "$SUPERSPIDER/phase6/ss_to_spider.py" "$HERE/ss.json" "$IMG" 8000 "$HERE/twinworld.json" "$HERE/out" "$HERE"
