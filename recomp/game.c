/* Twin World (Ubi Soft, 1990/1991; program by James Byrne, Cygnus Software
 * Engineering): what libroger needs to know about the game. */
#include <roger.h>

extern const struct roger_image recomp_image;      /* gen/entries.c (gen_c.py) */

static const char *const prefixes[] = {"<TwinWorld$Dir>.", "<Obey$Dir>.", NULL};

/* Return sites inside the self-recursive Huffman decoder at &9434 (loading
 * data files), which SuperSpider can't resolve - see ../README.md. */
static const uint32_t interpreted[] = {0x9450, 0x9460, 0, 0};

static const struct roger_config config = {
    .title = "Twin World (recompiled)",
    /* a controller: Z X ' / (the game's directions), A Return, B and start
     * Space */
    .pad_map = "left=Z,right=X,up=',down=/,a=Return,b=Space,start=Space",
    .image_path = "../game/!TwinWorld/!RunImage,fffff842-00000000",
    .image = &recomp_image,
    .appdir = "../game/!TwinWorld",
    .dir_prefixes = prefixes,
    /* !Run: Echo <22><9>... (MODE 9) and WimpSlot 656K. The program checks
     * both: OS_Byte 135 must report mode 9 ("Bad mode") and the RAM limit
     * from OS_GetEnv must leave it 656K ("Twin World requires 656k ..."). */
    .initial_mode = 9,
    .wimpslot = 656 * 1024,
    .interpreted = interpreted,
};

int main(int argc, char **argv) { roger_main(argc, argv, &config); }
