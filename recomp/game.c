/* Twin World (Ubi Soft, 1990/1991; program by James Byrne, Cygnus Software
 * Engineering): the game-specific half of the runtime. See game.h. */
#include <stddef.h>
#include "game.h"

const char *const game_title = "Twin World (recompiled)";
const char *const game_default_image = "../extracted/!TwinWorld/!RunImage,fffff842-00000000";
const char *const game_default_appdir = "../extracted/!TwinWorld";
const char *const game_dir_prefixes[] = {"<TwinWorld$Dir>.", "<Obey$Dir>.", NULL};

/* !Run: Echo <22><9>... (MODE 9) and WimpSlot 656K. The program checks both:
 * OS_Byte 135 must report mode 9 ("Bad mode") and the RAM limit from
 * OS_GetEnv must leave it 656K ("Twin World requires 656k ..."). */
const int game_initial_mode = 9;
const uint32_t game_wimpslot = 656 * 1024;
const int game_pointer_y_offset = 0;

/* Return sites inside the self-recursive Huffman decoder at &9434 (loading
 * data files), which SuperSpider can't resolve - see ../README.md. */
const uint32_t game_interpreted[] = {0x9450, 0x9460, 0, 0};

void game_init(void) {}
void game_frame(void) {}
int game_swi(uint32_t swi, int x) { (void)swi; (void)x; return 0; }
int game_call_addr(uint32_t a) { (void)a; return 0; }
