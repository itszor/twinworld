/* What a game supplies to the shared RISC OS runtime.
 *
 * runtime.c, runtime.h, display.c, sound.c, vdu.c and os.h are the generic
 * part: ARM2 CPU semantics, dispatcher and interpreter, interrupts, the RISC
 * OS SWI/VDU/sound emulation and the SDL front end. They were copied from
 * ../../lemmings/recomp and are meant to become a separate library shared by
 * the recompilation projects. Everything specific to one game lives behind
 * this interface (game.c), so that split is mostly a matter of moving files.
 */
#ifndef GAME_H
#define GAME_H
#include <stdint.h>

/* --- configuration */
extern const char *const game_title;            /* window title */
extern const char *const game_default_image;    /* the program, relative to recomp/ */
extern const char *const game_default_appdir;   /* host directory standing in for the
                                                   application directory */
extern const char *const game_dir_prefixes[];   /* filename prefixes meaning "the app
                                                   directory" (e.g. "<TwinWorld$Dir>."),
                                                   NULL-terminated */
extern const int game_initial_mode;             /* screen mode !Run selects before the program */
extern const uint32_t game_wimpslot;            /* application memory (!Run's WimpSlot): the
                                                   RAM limit OS_GetEnv reports is &8000 + this */
extern const uint32_t game_interpreted[];       /* known gaps: [from, to) address pairs the
                                                   runtime interprets without stopping (other
                                                   untranslated addresses stop the run unless
                                                   --trace-gaps), terminated by 0, 0 */
extern const int game_pointer_y_offset;         /* OS units between the host pointer and the
                                                   game's own pointer hit point (0 if none) */

/* --- hooks */
void game_init(void);                   /* after the program is loaded, before it runs */
void game_frame(void);                  /* once per vsync, before the vsync event */
int  game_swi(uint32_t swi, int x);     /* a SWI the runtime doesn't implement: 1 if handled
                                           (x: X-flagged) */
int  game_call_addr(uint32_t a);        /* dispatch hook, before the address lookup: 1 if
                                           the game handled the transfer (e.g. a jump into
                                           relocated code) */
#endif
