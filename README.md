Twin World (Ubi Soft, 1990/91) - static recompilation
=====================================================

Archimedes conversion of the Amiga/ST game (program by James Byrne, Cygnus
Software Engineering). Disc image: `Twinworld (1991)(ubisoft).adf`.

    tools/extract.py               unpack the ADF -> extracted/ (!TwinWorld, !TwinInfo)
    analysis/run.sh                code/data separation with SuperSpider (~/stuff/superspider):
                                   ss.json -> twinworld.json(.pickle), out/functions.tsv
    recomp/                        make regen (analysis + gen_c.py), make, ./twinworld
                                   make cov: coverage build (SuperSpider phase 0 recorder)

Unlike Lemmings and Galactic Dan there are no Ghidra projects, hand seeds or
dynamic facts: SuperSpider's analysis alone (5458 code words, 153
functions) drives gen_c.py. The only code the runtime has to interpret
is &9450-&945C (below).

Running
-------

    cd recomp && make && ./twinworld [--no-sound] [--scale=N] [--fullscreen]
    ./twinworld --trace-gaps          interpret untranslated code instead of stopping, logging
                                      each new address to untranslated.log (the known gap below,
                                      game.c's game_interpreted[], is always interpreted silently)
    ./twinworld --save=DIR            where files the game writes go (default savedata/)
    ./twinworld --wav=F.wav --script=5000:key:99,...,14000:quit   (with SDL_VIDEO_DRIVER=offscreen)

Keys: the game scans the keyboard with OS_Byte 121; its key list (&C2B1)
holds Z, X, INKEY -80, / (the four directions, also the joystick bits),
Return, Space, S, P, H, Escape and INKEY -45. Verified so far: Space then Return
start a game from the title menu, X moves right. `--script` key numbers are
the negated INKEY numbers (Return 74, Space 99, X 67).

Layout of the runtime: generic vs game
--------------------------------------

The runtime was copied from ../lemmings/recomp and is meant to become a
separate library shared by the recompilation projects. Everything specific
to Twin World is behind `game.h` / `game.c` (title, program image, app
directory and its `<TwinWorld$Dir>` prefixes, start-up mode 9, WimpSlot
656K, and hooks for init / per-frame / unknown SWIs / dispatch); the rest
(runtime.c, runtime.h, display.c, sound.c, vdu.c, os.h) contains no Twin
World knowledge. Lemmings-specific parts were removed (MusicModule2,
LemBoot relocation, the &100 kernel IRQ slot).

What the generic part gained for Twin World, all of it general Archimedes
hardware / RISC OS behaviour:

- OS_GetEnv with the RAM limit (&8000 + WimpSlot), OS_Byte 135 (mode),
  file handles (OS_Find/GBPB/Args/BGet/BPut) with writes redirected to
  --save, OS_ChangeEnvironment bookkeeping.
- Device vectors (OS_ClaimDeviceVector): device 3 (vsync/flyback) and
  device 6 (IOC timer 1) are called each frame; the palette the timer
  handler leaves applies below the line where the timer fires.
- MEMC video DMA registers (writes to &3600000+: Vinit/Vstart/Vend). The
  game flips between two play-area buffers by rewriting Vinit/Vend in its
  vsync handler, and the 40-line panel lives at physical &280, reached when
  DMA wraps from Vend to Vstart (0). Screen memory is the OS's 80K ending at
  &2000000, physical 0 = &1FEC000.
- VIDC vertical timing (VCR/VDSR/VDER, regs 40/43/44). Twin World shows 204
  lines (VDSR 64, VDER 268), centred in the standard 256-line window with
  border above and below. The "vsync" interrupt is VIDC's flyback, raised at
  the end of the display (VDER); the timer started there with latch &863B
  (268.5 lines) fires at raster 224.5 = display line 160.5, between the
  play area and the panel. With standard timings the same formula gives
  Lemmings' former constant (56 lines).
- The OS sound channel handler (SoundChannels): Sound_InstallVoice /
  AttachVoice / Control / Pitch / Volume / Speaker / QInit, and a native
  default Level 1 handler that calls each channel's voice generator
  (GateOn / Fill / GateOff, R6 >= 0, R7 = channel - 1, R9 = SCCB, R10-R12
  buffer) every DMA buffer. Twin World's single voice "TwinVoice" (&89A8)
  plays the game's samples from its own per-channel table at &9590; the
  four channels are gated on once at start-up (Sound_Control duration
  &100, treated as "until released"). Sound_Pitch: &4000 = middle C, 4096
  per octave, as a 256-sample wavetable step at the current sample period -
  pitch &1000 (the effects' usual pitch) comes out at 8.36 kHz, the Amiga's
  usual sample rate. Music and effects both play.

Notes for the library split
---------------------------

- Galactic Dan's runtime implements its voices natively (reading sample
  start/length from the voice generators' GateOn code); the channel
  handler here runs voice generators as translated code instead, so it
  should replace that. Lemmings brings its own Level 1 handler (musmod2),
  which keeps working (Sound_Configure's handler bypasses the channel
  handler).
- The display path is: OS banks / OS_Word 22 when the program uses the OS,
  MEMC Vinit/Vstart/Vend once it writes MEMC itself, cropped by VIDC
  VDSR/VDER either way.
- Border colour (VIDC reg 16) is not modelled; border lines show colour 0.

Known gaps
----------

- &9450-&945C: return sites inside the recursive Huffman decoder at &9434
  (used while loading data files). A leaf jumps through r8 to an output
  handler that pops the decoder's own frame; SuperSpider resolves that for
  the outer call but not for the self-recursive one. Interpreted.
- &D8B4 `addeq pc, r0, #8` (resuming an animation script; the scripts
  start with `bl &D2A8`/`bl &D2C4` + inline data, ARM code 16 bytes after
  the bl) is resolved by SuperSpider's field-offset fallback; the other
  unresolved jumps (&BFD8, &C654, &D8E0) were not hit in testing (a
  170 s randomised play session found no further gaps).
