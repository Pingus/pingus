#include "BlueHead.h"

#define error(a)		_asm int 3



const uint MinMatch = 3;
const uint MaxMatch = 258;
/* The minimum and maximum match lengths */

const uint WSize = 0x8000;
/* Maximum window size = 32K. If you are really short of memory, compile
 * with a smaller WSIZE but this reduces the compression ratio for files
 * of size > WSIZE. WSIZE must be a power of two in the current implementation.
 */

const uint MinLookahead = (MaxMatch + MinMatch + 1);
/* Minimum amount of lookahead, except at the end of the input file.
 * See deflate.c for comments about the MIN_MATCH+1.
 */

const uint MaxDist = (WSize - MinLookahead);
/* In order to simplify the code, particularly on 16 bit machines, match
 * distances are limited to MAX_DIST instead of WSIZE.
 */

/* Public globals */


void flush_outbuf(char *, unsigned *);
uint ReadBuf(char *buf, unsigned size);



        /* in deflate.c */
void LmInit();
u32  Deflate();

        /* in trees.c */
void     ct_init      (int *);
bool      ct_tally     (int, int);
u32      flush_block  (char far *, u32, int);
void     bi_init      (char *, unsigned int, int);

