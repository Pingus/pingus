#include "CompHead.h"

#ifndef NO_COMPRESSION

/*
  Copyright (c) 1990-1999 Info-ZIP.  All rights reserved.

  See the accompanying file LICENSE, version 1999-Oct-05 or later
  (the contents of which are also included in zip.h) for terms of use.
  If, for some reason, both of these files are missing, the Info-ZIP license
  also may be found at:  ftp://ftp.cdrom.com/pub/infozip/license.html
*/
/*
 *  trees.c by Jean-loup Gailly
 *
 *  This is a new version of im_ctree.c originally written by Richard B. Wales
 *  for the defunct implosion method.
 *  The low level bit string handling routines from bits.c (originally
 *  im_bits.c written by Richard B. Wales) have been merged into this version
 *  of trees.c.
 *
 *  PURPOSE
 *
 *      Encode various sets of source values using variable-length
 *      binary code trees.
 *      Output the resulting variable-length bit strings.
 *      Compression can be done to a file or to memory.
 *
 *  DISCUSSION
 *
 *      The PKZIP "deflation" process uses several Huffman trees. The more
 *      common source values are represented by shorter bit sequences.
 *
 *      Each code tree is stored in the ZIP file in a compressed form
 *      which is itself a Huffman encoding of the lengths of
 *      all the code strings (in ascending order by source values).
 *      The actual code strings are reconstructed from the lengths in
 *      the UNZIP process, as described in the "application note"
 *      (APPNOTE.TXT) distributed as part of PKWARE's PKZIP program.
 *
 *      The PKZIP "deflate" file format interprets compressed file data
 *      as a sequence of bits.  Multi-bit strings in the file may cross
 *      byte boundaries without restriction.
 *      The first bit of each byte is the low-order bit.
 *
 *      The routines in this file allow a variable-length bit value to
 *      be output right-to-left (useful for literal values). For
 *      left-to-right output (useful for code strings from the tree routines),
 *      the bits must have been reversed first with bi_reverse().
 *
 *      For in-memory compression, the compressed bit stream goes directly
 *      into the requested output buffer. The buffer is limited to 64K on
 *      16 bit machines; flushing of the output buffer during compression
 *      process is not supported.
 *      The input data is read in blocks by the (*read_buf)() function.
 *
 *      For more details about input to and output from the deflation routines,
 *      see the actual input functions for (*read_buf)(), flush_outbuf(), and
 *      the filecompress() resp. memcompress() wrapper functions which handle
 *      the I/O setup.
 *
 *  REFERENCES
 *
 *      Lynch, Thomas J.
 *          Data Compression:  Techniques and Applications, pp. 53-55.
 *          Lifetime Learning Publications, 1985.  ISBN 0-534-03418-7.
 *
 *      Storer, James A.
 *          Data Compression:  Methods and Theory, pp. 49-50.
 *          Computer Science Press, 1988.  ISBN 0-7167-8156-5.
 *
 *      Sedgewick, R.
 *          Algorithms, p290.
 *          Addison-Wesley, 1983. ISBN 0-201-06672-6.
 *
 *  INTERFACE
 *
 *      void ct_init (ush *attr, int *method)
 *          Allocate the match buffer, initialize the various tables and save
 *          the location of the internal file attribute (ascii/binary) and
 *          method (DEFLATE/STORE)
 *
 *      void ct_tally (int dist, int lc);
 *          Save the match info and tally the frequency counts.
 *
 *      u32 flush_block (char *buf, u32 stored_len, int eof)
 *          Determine the best encoding for the current block: dynamic trees,
 *          static trees or store, and output the encoded block to the zip
 *          file. Returns the total compressed length for the file so far.
 *
 *      void bi_init (char *tgt_buf, unsigned tgt_size, int flsh_allowed)
 *          Initialize the bit string routines.
 *
 *    Most of the bit string output functions are only used internally
 *    in this source file, they are normally declared as "local" routines:
 *
 *      local void send_bits (int value, int length)
 *          Write out a bit string, taking the source bits right to
 *          left.
 *
 *      local unsigned bi_reverse (unsigned code, int len)
 *          Reverse the bits of a bit string, taking the source bits left to
 *          right and emitting them right to left.
 *
 *      local void bi_windup (void)
 *          Write out any remaining bits in an incomplete byte.
 *
 *      local void copy_block(char *buf, unsigned len, int header)
 *          Copy a stored block to the zip file, storing first the length and
 *          its one's complement if requested.
 *
 *    All output that exceeds the bitstring output buffer size (as initialized
 *    by bi_init() is fed through an externally provided transfer routine
 *    which flushes the bitstring output buffer on request and resets the
 *    buffer fill counter:
 *
 *      extern void flush_outbuf(char *o_buf, unsigned *o_idx);
 *
 */

#include <ctype.h>

/* ===========================================================================
 * Constants
 */

const uint MAX_BITS = 15;
/* All codes must not exceed MAX_BITS bits */

const uint MAX_BL_BITS = 7;
/* Bit length codes must not exceed MAX_BL_BITS bits */

const uint LENGTH_CODES = 29;
/* number of length codes, not counting the special END_BLOCK code */

const uint LITERALS = 256;
/* number of literal bytes 0..255 */

const uint END_BLOCK = 256;
/* end of block literal code */

const uint L_CODES = (LITERALS + 1 + LENGTH_CODES);
/* number of Literal or Length codes, including the END_BLOCK code */

const uint D_CODES = 30;
/* number of distance codes */

const uint BL_CODES = 19;
/* number of codes used to transfer the bit lengths */


int extra_lbits[LENGTH_CODES] /* extra bits for each length code */
   = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};

int extra_dbits[D_CODES] /* extra bits for each distance code */
   = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

int extra_blbits[BL_CODES]/* extra bits for each bit length code */
   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7};

const uint STORED_BLOCK = 0;
const uint STATIC_TREES = 1;
const uint DYN_TREES	= 2;
/* The three kinds of block type */

const uint LIT_BUFSIZE = 0x8000;
const uint DIST_BUFSIZE = LIT_BUFSIZE;
/* Sizes of match buffers for literals/lengths and distances.  There are
 * 4 reasons for limiting LIT_BUFSIZE to 64K:
 *   - frequencies can be kept in 16 bit counters
 *   - if compression is not successful for the first block, all input data is
 *     still in the window so we can still emit a stored block even when input
 *     comes from standard input.  (This can also be done for all blocks if
 *     LIT_BUFSIZE is not greater than 32K.)
 *   - if compression is not successful for a file smaller than 64K, we can
 *     even emit a stored file instead of a stored block (saving 5 bytes).
 *   - creating new Huffman trees less frequently may not provide fast
 *     adaptation to changes in the input data statistics. (Take for
 *     example a binary file with poorly compressible code followed by
 *     a highly compressible string table.) Smaller buffer sizes give
 *     fast adaptation but have of course the overhead of transmitting trees
 *     more frequently.
 *   - I can't count above 4
 * The current code is general and allows DIST_BUFSIZE < LIT_BUFSIZE (to save
 * memory at the expense of compression). Some optimizations would be possible
 * if we rely on DIST_BUFSIZE == LIT_BUFSIZE.
 */

const uint REP_3_6      = 16;
/* repeat previous bit length 3-6 times (2 bits of repeat count) */

const uint REPZ_3_10    = 17;
/* repeat a zero length 3-10 times  (3 bits of repeat count) */

const uint REPZ_11_138	= 18;
/* repeat a zero length 11-138 times  (7 bits of repeat count) */

/* ===========================================================================
 * Local data
 */

/* Data structure describing a single value and its code string. */
typedef struct ct_data {
    union {
        u16  freq;       /* frequency count */
        u16  code;       /* bit string */
    } fc;
    union {
        u16  dad;        /* father node in Huffman tree */
        u16  len;        /* length of bit string */
    } dl;
} ct_data;

#define Freq fc.freq
#define Code fc.code
#define Dad  dl.dad
#define Len  dl.len

const uint HEAP_SIZE = (2 * L_CODES) + 1;
// maximum heap size

ct_data dyn_ltree[HEAP_SIZE];   /* literal and length tree */
ct_data dyn_dtree[2*D_CODES+1]; /* distance tree */

ct_data static_ltree[L_CODES+2];
/* The static literal tree. Since the bit lengths are imposed, there is no
 * need for the L_CODES extra codes used during heap construction. However
 * The codes 286 and 287 are needed to build a canonical tree (see ct_init
 * below).
 */

ct_data static_dtree[D_CODES];
/* The static distance tree. (Actually a trivial tree since all codes use
 * 5 bits.)
 */

ct_data bl_tree[2*BL_CODES+1];
/* Huffman tree for the bit lengths */

struct tree_desc {
    ct_data near *dyn_tree;      /* the dynamic tree */
    ct_data near *static_tree;   /* corresponding static tree or NULL */
    int     near *extra_bits;    /* extra bits for each code or NULL */
    int     extra_base;          /* base index for extra_bits */
    int     elems;               /* max number of elements in the tree */
    int     max_length;          /* max bit length for the codes */
    int     max_code;            /* largest code with non zero frequency */
};

tree_desc l_desc =
{dyn_ltree, static_ltree, extra_lbits, LITERALS+1, L_CODES, MAX_BITS, 0};

tree_desc d_desc =
{dyn_dtree, static_dtree, extra_dbits, 0,          D_CODES, MAX_BITS, 0};

tree_desc bl_desc =
{bl_tree, NULL,       extra_blbits, 0,         BL_CODES, MAX_BL_BITS, 0};


u16 bl_count[MAX_BITS+1];
/* number of codes at each bit length for an optimal tree */

u8 bl_order[BL_CODES]
   = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
/* The lengths of the bit length codes are sent in order of decreasing
 * probability, to avoid transmitting the lengths for unused bit length codes.
 */

int heap[2*L_CODES+1]; /* heap used to build the Huffman trees */
int heap_len;               /* number of elements in the heap */
int heap_max;               /* element of largest frequency */
/* The sons of heap[n] are heap[2*n] and heap[2*n+1]. heap[0] is not used.
 * The same heap array is used to build all trees.
 */

u8 near depth[2*L_CODES+1];
/* Depth of each subtree used as tie breaker for trees of equal frequency */

u8 length_code[MaxMatch - MinMatch + 1];
/* length code for each normalized match length (0 == MIN_MATCH) */

u8 dist_code[512];
/* distance codes. The first 256 values correspond to the distances
 * 3 .. 258, the last 256 values correspond to the top 8 bits of
 * the 15 bit distances.
 */

int near base_length[LENGTH_CODES];
/* First normalized length for each code (0 = MIN_MATCH) */

int near base_dist[D_CODES];
/* First normalized distance for each code (0 = distance of 1) */

u8 l_buf[LIT_BUFSIZE];  /* buffer for literals/lengths */
u16 d_buf[DIST_BUFSIZE]; /* buffer for distances */


u8 flag_buf[(LIT_BUFSIZE/8)];
/* flag_buf is a bit array distinguishing literals from lengths in
 * l_buf, and thus indicating the presence or absence of a distance.
 */

uint last_lit;    /* running index in l_buf */
uint last_dist;   /* running index in d_buf */
uint last_flags;  /* running index in flag_buf */
u8 flags;            /* current flags not yet saved in flag_buf */
u8 flag_bit;         /* current bit used in flags */
/* bits are filled in flags starting at bit 0 (least significant).
 * Note: these flags are overkill in the current code since we don't
 * take advantage of DIST_BUFSIZE == LIT_BUFSIZE.
 */

u32 opt_len;        /* bit length of current block with optimal trees */
u32 static_len;     /* bit length of current block with static trees */

u32 cmpr_bytelen;     /* total byte length of compressed file */
u32 cmpr_len_bits;    /* number of bits past 'cmpr_bytelen' */

#ifdef _DEBUG
u32 input_len;        /* total byte length of input file */
/* input_len is for debugging only since we can get it by other means. */
#endif

int *file_method;     /* pointer to DEFLATE or STORE */

/* ===========================================================================
 * Local data used by the "bit string" routines.
 */

int flush_flg;

uint bi_buf;
/* Output buffer. bits are inserted starting at the bottom (least significant
 * bits). The width of bi_buf must be at least 16 bits.
 */

const uint Buf_size = (8 * 2*sizeof(char));
/* Number of bits used within bi_buf. (bi_buf may be implemented on
 * more than 16 bits on some systems.)
 */

int bi_valid;
/* Number of valid bits in bi_buf.  All bits above the last valid bit
 * are always zero.
 */

char *out_buf;
/* Current output buffer. */

uint out_offset;
/* Current offset in output buffer.
 * On 16 bit machines, the buffer is limited to 64K.
 */

uint out_size;
/* Size of current output buffer */

/* Output a 16 bit value to the bit stream, lower (oldest) byte first */
#define PUTSHORT(w) \
{ if (out_offset >= out_size-1) \
    flush_outbuf(out_buf, &out_offset); \
  out_buf[out_offset++] = (char) ((w) & 0xff); \
  out_buf[out_offset++] = (char) ((u16)(w) >> 8); \
}

#define PUTBYTE(b) \
{ if (out_offset >= out_size) \
    flush_outbuf(out_buf, &out_offset); \
  out_buf[out_offset++] = (char) (b); \
}

#ifdef _DEBUG
u32 bits_sent;   /* bit length of the compressed data */
extern u32 isize;      /* byte length of input file */
#endif

extern long BlockStart;       /* window offset of current block */
extern uint StrStart; /* window offset of current string */


/* ===========================================================================
 * Local (static) routines in this file.
 */

void init_block     (void);
void pqdownheap     (ct_data near *tree, int k);
void gen_bitlen     (tree_desc near *desc);
void gen_codes      (ct_data near *tree, int max_code);
void build_tree     (tree_desc near *desc);
void scan_tree      (ct_data near *tree, int max_code);
void send_tree      (ct_data near *tree, int max_code);
int  build_bl_tree  (void);
void send_all_trees (int lcodes, int dcodes, int blcodes);
void compress_block (ct_data near *ltree, ct_data near *dtree);
void set_file_type  (void);
void send_bits      (int value, int length);
unsigned bi_reverse (unsigned code, int len);
void bi_windup      (void);
void copy_block     (char *buf, unsigned len, int header);


#ifndef _DEBUG
#  define send_code(c, tree) send_bits(tree[c].Code, tree[c].Len)
   /* Send a code of the given tree. c and tree must not have side effects */

#else /* _DEBUG */
#  define send_code(c, tree) \
     { ; \
       send_bits(tree[c].Code, tree[c].Len); }
#endif

#define d_code(dist) \
   ((dist) < 256 ? dist_code[dist] : dist_code[256+((dist)>>7)])
/* Mapping from a distance to a distance code. dist is the distance - 1 and
 * must not have side effects. dist_code[256] and dist_code[257] are never
 * used.
 */

/* ===========================================================================
 * Allocate the match buffer, initialize the various tables and save the
 * location of the internal file attribute (ascii/binary) and method
 * (DEFLATE/STORE).
 */
void ct_init(int* method)
    //ush  *attr;   /* pointer to internal file attribute */
    //int  *method; /* pointer to compression method */
{
    int n;        /* iterates over tree elements */
    int bits;     /* bit counter */
    int length;   /* length value */
    int code;     /* code value */
    int dist;     /* distance index */

    file_method = method;
    cmpr_bytelen = cmpr_len_bits = 0L;
#ifdef _DEBUG
    input_len = 0L;
#endif

    if (static_dtree[0].Len != 0) return; /* ct_init already called */

    /* Initialize the mapping length (0..255) -> length code (0..28) */
    length = 0;
    for (code = 0; code < LENGTH_CODES-1; code++) {
        base_length[code] = length;
        for (n = 0; n < (1<<extra_lbits[code]); n++) {
            length_code[length++] = (u8)code;
        }
    }
    Assert(length == 256);//, "ct_init: length != 256");
    /* Note that the length 255 (match length 258) can be represented
     * in two different ways: code 284 + 5 bits or code 285, so we
     * overwrite length_code[255] to use the best encoding:
     */
    length_code[length-1] = (u8)code;

    /* Initialize the mapping dist (0..32K) -> dist code (0..29) */
    dist = 0;
    for (code = 0 ; code < 16; code++) {
        base_dist[code] = dist;
        for (n = 0; n < (1<<extra_dbits[code]); n++) {
            dist_code[dist++] = (u8)code;
        }
    }
    Assert(dist == 256);//, "ct_init: dist != 256");
    dist >>= 7; /* from now on, all distances are divided by 128 */
    for ( ; code < D_CODES; code++) {
        base_dist[code] = dist << 7;
        for (n = 0; n < (1<<(extra_dbits[code]-7)); n++) {
            dist_code[256 + dist++] = (u8)code;
        }
    }
    Assert(dist == 256);//, "ct_init: 256+dist != 512");

    /* Construct the codes of the static literal tree */
    for (bits = 0; bits <= MAX_BITS; bits++) bl_count[bits] = 0;
    n = 0;
    while (n <= 143) static_ltree[n++].Len = 8, bl_count[8]++;
    while (n <= 255) static_ltree[n++].Len = 9, bl_count[9]++;
    while (n <= 279) static_ltree[n++].Len = 7, bl_count[7]++;
    while (n <= 287) static_ltree[n++].Len = 8, bl_count[8]++;
    /* Codes 286 and 287 do not exist, but we must include them in the
     * tree construction to get a canonical Huffman tree (longest code
     * all ones)
     */
    gen_codes((ct_data near *)static_ltree, L_CODES+1);

    /* The static distance tree is trivial: */
    for (n = 0; n < D_CODES; n++) {
        static_dtree[n].Len = 5;
        static_dtree[n].Code = (u16)bi_reverse(n, 5);
    }

    /* Initialize the first block of the first file: */
    init_block();
}

/* ===========================================================================
 * Initialize a new block.
 */
void init_block()
{
    int n; /* iterates over tree elements */

    /* Initialize the trees. */
    for (n = 0; n < L_CODES;  n++) dyn_ltree[n].Freq = 0;
    for (n = 0; n < D_CODES;  n++) dyn_dtree[n].Freq = 0;
    for (n = 0; n < BL_CODES; n++) bl_tree[n].Freq = 0;

    dyn_ltree[END_BLOCK].Freq = 1;
    opt_len = static_len = 0L;
    last_lit = last_dist = last_flags = 0;
    flags = 0; flag_bit = 1;
}

#define SMALLEST 1
/* Index within the heap array of least frequent node in the Huffman tree */


/* ===========================================================================
 * Remove the smallest element from the heap and recreate the heap with
 * one less element. Updates heap and heap_len.
 */
#define pqremove(tree, top) \
{\
    top = heap[SMALLEST]; \
    heap[SMALLEST] = heap[heap_len--]; \
    pqdownheap(tree, SMALLEST); \
}

/* ===========================================================================
 * Compares to subtrees, using the tree depth as tie breaker when
 * the subtrees have equal frequency. This minimizes the worst case length.
 */
#define smaller(tree, n, m) \
   (tree[n].Freq < tree[m].Freq || \
   (tree[n].Freq == tree[m].Freq && depth[n] <= depth[m]))

/* ===========================================================================
 * Restore the heap property by moving down the tree starting at node k,
 * exchanging a node with the smallest of its two sons if necessary, stopping
 * when the heap property is re-established (each father smaller than its
 * two sons).
 */
void pqdownheap(ct_data near *tree, int k)
    //ct_data near *tree;  /* the tree to restore */
    //int k;               /* node to move down */
{
    int v = heap[k];
    int j = k << 1;  /* left son of k */
    int htemp;       /* required because of bug in SASC compiler */

    while (j <= heap_len) {
        /* Set j to the smallest of the two sons: */
        if (j < heap_len && smaller(tree, heap[j+1], heap[j])) j++;

        /* Exit if v is smaller than both sons */
        htemp = heap[j];
        if (smaller(tree, v, htemp)) break;

        /* Exchange v with the smallest son */
        heap[k] = htemp;
        k = j;

        /* And continue down the tree, setting j to the left son of k */
        j <<= 1;
    }
    heap[k] = v;
}

/* ===========================================================================
 * Compute the optimal bit lengths for a tree and update the total bit length
 * for the current block.
 * IN assertion: the fields freq and dad are set, heap[heap_max] and
 *    above are the tree nodes sorted by increasing frequency.
 * OUT assertions: the field len is set to the optimal bit length, the
 *     array bl_count contains the frequencies for each bit length.
 *     The length opt_len is updated; static_len is also updated if stree is
 *     not null.
 */
void gen_bitlen(tree_desc near *desc)
    //tree_desc near *desc; /* the tree descriptor */
{
    ct_data near *tree  = desc->dyn_tree;
    int near *extra     = desc->extra_bits;
    int base            = desc->extra_base;
    int max_code        = desc->max_code;
    int max_length      = desc->max_length;
    ct_data near *stree = desc->static_tree;
    int h;              /* heap index */
    int n, m;           /* iterate over the tree elements */
    int bits;           /* bit length */
    int xbits;          /* extra bits */
    u16 f;              /* frequency */
    int overflow = 0;   /* number of elements with bit length too large */

    for (bits = 0; bits <= MAX_BITS; bits++) bl_count[bits] = 0;

    /* In a first pass, compute the optimal bit lengths (which may
     * overflow in the case of the bit length tree).
     */
    tree[heap[heap_max]].Len = 0; /* root of the heap */

    for (h = heap_max+1; h < HEAP_SIZE; h++) {
        n = heap[h];
        bits = tree[tree[n].Dad].Len + 1;
        if (bits > max_length) bits = max_length, overflow++;
        tree[n].Len = (u16)bits;
        /* We overwrite tree[n].Dad which is no longer needed */

        if (n > max_code) continue; /* not a leaf node */

        bl_count[bits]++;
        xbits = 0;
        if (n >= base) xbits = extra[n-base];
        f = tree[n].Freq;
        opt_len += (u32)f * (bits + xbits);
        if (stree) static_len += (u32)f * (stree[n].Len + xbits);
    }
    if (overflow == 0) return;

    /* Find the first bit length which could increase: */
    do {
        bits = max_length-1;
        while (bl_count[bits] == 0) bits--;
        bl_count[bits]--;           /* move one leaf down the tree */
        bl_count[bits+1] += (u16)2; /* move one overflow item as its brother */
        bl_count[max_length]--;
        /* The brother of the overflow item also moves one step up,
         * but this does not affect bl_count[max_length]
         */
        overflow -= 2;
    } while (overflow > 0);

    /* Now recompute all bit lengths, scanning in increasing frequency.
     * h is still equal to HEAP_SIZE. (It is simpler to reconstruct all
     * lengths instead of fixing only the wrong ones. This idea is taken
     * from 'ar' written by Haruhiko Okumura.)
     */
    for (bits = max_length; bits != 0; bits--) {
        n = bl_count[bits];
        while (n != 0) {
            m = heap[--h];
            if (m > max_code) continue;
            if (tree[m].Len != (u16)bits) {
                opt_len += ((long)bits-(long)tree[m].Len)*(long)tree[m].Freq;
                tree[m].Len = (u16)bits;
            }
            n--;
        }
    }
}

/* ===========================================================================
 * Generate the codes for a given tree and bit counts (which need not be
 * optimal).
 * IN assertion: the array bl_count contains the bit length statistics for
 * the given tree and the field len is set for all tree elements.
 * OUT assertion: the field code is set for all tree elements of non
 *     zero code length.
 */
void gen_codes (ct_data near *tree, int max_code)
    //ct_data near *tree;        /* the tree to decorate */
    //int max_code;              /* largest code with non zero frequency */
{
    u16 next_code[MAX_BITS+1]; /* next code value for each bit length */
    u16 code = 0;              /* running code value */
    int bits;                  /* bit index */
    int n;                     /* code index */

    /* The distribution counts are first used to generate the code values
     * without bit reversal.
     */
    for (bits = 1; bits <= MAX_BITS; bits++) {
        next_code[bits] = code = (u16)((code + bl_count[bits-1]) << 1);
    }
    /* Check that the bit counts in bl_count are consistent. The last code
     * must be all ones.
     */
    Assert(code + bl_count[MAX_BITS]-1 == (1<< ((u16) MAX_BITS)) - 1);//inconsistent bit counts

    for (n = 0;  n <= max_code; n++) {
        int len = tree[n].Len;
        if (len == 0) continue;
        /* Now reverse the bits */
        tree[n].Code = (u16)bi_reverse(next_code[len]++, len);
    }
}

/* ===========================================================================
 * Construct one Huffman tree and assigns the code bit strings and lengths.
 * Update the total bit length for the current block.
 * IN assertion: the field freq is set for all tree elements.
 * OUT assertions: the fields len and code are set to the optimal bit length
 *     and corresponding code. The length opt_len is updated; static_len is
 *     also updated if stree is not null. The field max_code is set.
 */
void build_tree(tree_desc near *desc)
    //tree_desc near *desc; /* the tree descriptor */
{
    ct_data near *tree   = desc->dyn_tree;
    ct_data near *stree  = desc->static_tree;
    int elems            = desc->elems;
    int n, m;          /* iterate over heap elements */
    int max_code = -1; /* largest code with non zero frequency */
    int node = elems;  /* next internal node of the tree */

    /* Construct the initial heap, with least frequent element in
     * heap[SMALLEST]. The sons of heap[n] are heap[2*n] and heap[2*n+1].
     * heap[0] is not used.
     */
    heap_len = 0, heap_max = HEAP_SIZE;

    for (n = 0; n < elems; n++) {
        if (tree[n].Freq != 0) {
            heap[++heap_len] = max_code = n;
            depth[n] = 0;
        } else {
            tree[n].Len = 0;
        }
    }

    /* The pkzip format requires that at least one distance code exists,
     * and that at least one bit should be sent even if there is only one
     * possible code. So to avoid special checks later on we force at least
     * two codes of non zero frequency.
     */
    while (heap_len < 2) {
        int new_ = heap[++heap_len] = (max_code < 2 ? ++max_code : 0);
        tree[new_].Freq = 1;
        depth[new_] = 0;
        opt_len--; if (stree) static_len -= stree[new_].Len;
        /* new is 0 or 1 so it does not have extra bits */
    }
    desc->max_code = max_code;

    /* The elements heap[heap_len/2+1 .. heap_len] are leaves of the tree,
     * establish sub-heaps of increasing lengths:
     */
    for (n = heap_len/2; n >= 1; n--) pqdownheap(tree, n);

    /* Construct the Huffman tree by repeatedly combining the least two
     * frequent nodes.
     */
    do {
        pqremove(tree, n);   /* n = node of least frequency */
        m = heap[SMALLEST];  /* m = node of next least frequency */

        heap[--heap_max] = n; /* keep the nodes sorted by frequency */
        heap[--heap_max] = m;

        /* Create a new node father of n and m */
        tree[node].Freq = (u16)(tree[n].Freq + tree[m].Freq);
        depth[node] = (u8) (max(depth[n], depth[m]) + 1);
        tree[n].Dad = tree[m].Dad = (u16)node;

        /* and insert the new node in the heap */
        heap[SMALLEST] = node++;
        pqdownheap(tree, SMALLEST);

    } while (heap_len >= 2);

    heap[--heap_max] = heap[SMALLEST];

    /* At this point, the fields freq and dad are set. We can now
     * generate the bit lengths.
     */
    gen_bitlen((tree_desc near *)desc);

    /* The field len is now set, we can generate the bit codes */
    gen_codes ((ct_data near *)tree, max_code);
}

/* ===========================================================================
 * Scan a literal or distance tree to determine the frequencies of the codes
 * in the bit length tree. Updates opt_len to take into account the repeat
 * counts. (The contribution of the bit length codes will be added later
 * during the construction of bl_tree.)
 */
void scan_tree (ct_data near *tree, int max_code)
    //ct_data near *tree; /* the tree to be scanned */
    //int max_code;       /* and its largest code of non zero frequency */
{
    int n;                     /* iterates over all tree elements */
    int prevlen = -1;          /* last emitted length */
    int curlen;                /* length of current code */
    int nextlen = tree[0].Len; /* length of next code */
    int count = 0;             /* repeat count of the current code */
    int max_count = 7;         /* max repeat count */
    int min_count = 4;         /* min repeat count */

    if (nextlen == 0) max_count = 138, min_count = 3;
    tree[max_code+1].Len = (u16)-1; /* guard */

    for (n = 0; n <= max_code; n++) {
        curlen = nextlen; nextlen = tree[n+1].Len;
        if (++count < max_count && curlen == nextlen) {
            continue;
        } else if (count < min_count) {
            bl_tree[curlen].Freq += (u16)count;
        } else if (curlen != 0) {
            if (curlen != prevlen) bl_tree[curlen].Freq++;
            bl_tree[REP_3_6].Freq++;
        } else if (count <= 10) {
            bl_tree[REPZ_3_10].Freq++;
        } else {
            bl_tree[REPZ_11_138].Freq++;
        }
        count = 0; prevlen = curlen;
        if (nextlen == 0) {
            max_count = 138, min_count = 3;
        } else if (curlen == nextlen) {
            max_count = 6, min_count = 3;
        } else {
            max_count = 7, min_count = 4;
        }
    }
}

/* ===========================================================================
 * Send a literal or distance tree in compressed form, using the codes in
 * bl_tree.
 */
void send_tree (ct_data near *tree, int max_code)
    //ct_data near *tree; /* the tree to be scanned */
    //int max_code;       /* and its largest code of non zero frequency */
{
    int n;                     /* iterates over all tree elements */
    int prevlen = -1;          /* last emitted length */
    int curlen;                /* length of current code */
    int nextlen = tree[0].Len; /* length of next code */
    int count = 0;             /* repeat count of the current code */
    int max_count = 7;         /* max repeat count */
    int min_count = 4;         /* min repeat count */

    /* tree[max_code+1].Len = -1; */  /* guard already set */
    if (nextlen == 0) max_count = 138, min_count = 3;

    for (n = 0; n <= max_code; n++) {
        curlen = nextlen; nextlen = tree[n+1].Len;
        if (++count < max_count && curlen == nextlen) {
            continue;
        } else if (count < min_count) {
            do { send_code(curlen, bl_tree); } while (--count != 0);

        } else if (curlen != 0) {
            if (curlen != prevlen) {
                send_code(curlen, bl_tree); count--;
            }
            Assert(count >= 3 && count <= 6);//, " 3_6?");
            send_code(REP_3_6, bl_tree); send_bits(count-3, 2);

        } else if (count <= 10) {
            send_code(REPZ_3_10, bl_tree); send_bits(count-3, 3);

        } else {
            send_code(REPZ_11_138, bl_tree); send_bits(count-11, 7);
        }
        count = 0; prevlen = curlen;
        if (nextlen == 0) {
            max_count = 138, min_count = 3;
        } else if (curlen == nextlen) {
            max_count = 6, min_count = 3;
        } else {
            max_count = 7, min_count = 4;
        }
    }
}

/* ===========================================================================
 * Construct the Huffman tree for the bit lengths and return the index in
 * bl_order of the last bit length code to send.
 */
int build_bl_tree()
{
    int max_blindex;  /* index of last bit length code of non zero freq */

    /* Determine the bit length frequencies for literal and distance trees */
    scan_tree((ct_data near *)dyn_ltree, l_desc.max_code);
    scan_tree((ct_data near *)dyn_dtree, d_desc.max_code);

    /* Build the bit length tree: */
    build_tree((tree_desc near *)(&bl_desc));
    /* opt_len now includes the length of the tree representations, except
     * the lengths of the bit lengths codes and the 5+5+4 bits for the counts.
     */

    /* Determine the number of bit length codes to send. The pkzip format
     * requires that at least 4 bit length codes be sent. (appnote.txt says
     * 3 but the actual value used is 4.)
     */
    for (max_blindex = BL_CODES-1; max_blindex >= 3; max_blindex--) {
        if (bl_tree[bl_order[max_blindex]].Len != 0) break;
    }
    /* Update opt_len to include the bit length tree and counts */
    opt_len += 3*(max_blindex+1) + 5+5+4;

    return max_blindex;
}

/* ===========================================================================
 * Send the header for a block using dynamic Huffman trees: the counts, the
 * lengths of the bit length codes, the literal tree and the distance tree.
 * IN assertion: lcodes >= 257, dcodes >= 1, blcodes >= 4.
 */
void send_all_trees(int lcodes, int dcodes, int blcodes)
    //int lcodes, dcodes, blcodes; /* number of codes for each tree */
{
    int rank;                    /* index in bl_order */

    Assert(lcodes >= 257 && dcodes >= 1 && blcodes >= 4);//, "not enough codes");
    Assert(lcodes <= L_CODES && dcodes <= D_CODES && blcodes <= BL_CODES);//,
            //"too many codes");

    send_bits(lcodes-257, 5);
    /* not +255 as stated in appnote.txt 1.93a or -256 in 2.04c */
    send_bits(dcodes-1,   5);
    send_bits(blcodes-4,  4); /* not -3 as stated in appnote.txt */
    for (rank = 0; rank < blcodes; rank++) {
        send_bits(bl_tree[bl_order[rank]].Len, 3);
    }

    send_tree((ct_data near *)dyn_ltree, lcodes-1); /* send the literal tree */

    send_tree((ct_data near *)dyn_dtree, dcodes-1); /* send the distance tree */
}

/* ===========================================================================
 * Determine the best encoding for the current block: dynamic trees, static
 * trees or store, and output the encoded block to the zip file. This function
 * returns the total compressed length (in bytes) for the file so far.
 */
u32 flush_block(char *buf, u32 stored_len, int eof)
    //char *buf;        /* input block, or NULL if too old */
    //u32 stored_len;   /* length of input block */
    //int eof;          /* true if this is the last block for a file */
{
    u32 opt_lenb, static_lenb; /* opt_len and static_len in bytes */
    int max_blindex;  /* index of last bit length code of non zero freq */

    flag_buf[last_flags] = flags; /* Save the flags for the last 8 items */

    /* Construct the literal and distance trees */
    build_tree((tree_desc near *)(&l_desc));

    build_tree((tree_desc near *)(&d_desc));
    /* At this point, opt_len and static_len are the total bit lengths of
     * the compressed block data, excluding the tree representations.
     */

    /* Build the bit length tree for the above two trees, and get the index
     * in bl_order of the last bit length code to send.
     */
    max_blindex = build_bl_tree();

    /* Determine the best encoding. Compute first the block length in bytes */
    opt_lenb = (opt_len+3+7)>>3;
    static_lenb = (static_len+3+7)>>3;
#ifdef _DEBUG
    input_len += stored_len; /* for debugging only */
#endif
  
    if (static_lenb <= opt_lenb) opt_lenb = static_lenb;

    /* If compression failed and this is the first and last block,
     * the whole file is transformed into a stored file:
     */
    if (stored_len <= opt_lenb && eof && file_method != NULL &&
        cmpr_bytelen == 0L && cmpr_len_bits == 0L) {
        /* Since LIT_BUFSIZE <= 2*WSIZE, the input data must be there: */
        if (buf == NULL) error ("block vanished");

        copy_block(buf, (unsigned)stored_len, 0); /* without header */
        cmpr_bytelen = stored_len;
        *file_method = compStore;
    } else

    if (stored_len+4 <= opt_lenb && buf != (char*)NULL) {
                       /* 4: two words for the lengths *
		/* The test buf != NULL is only necessary if LIT_BUFSIZE > WSIZE.
         * Otherwise we can't have processed more than WSIZE input bytes since
         * the last block flush, because compression would have been
         * successful. If LIT_BUFSIZE <= WSIZE, it is never too late to
         * transform a block into a stored block.
         */
        send_bits((STORED_BLOCK<<1)+eof, 3);  /* send block type */
        cmpr_bytelen += ((cmpr_len_bits + 3 + 7) >> 3) + stored_len + 4;
        cmpr_len_bits = 0L;

        copy_block(buf, (unsigned)stored_len, 1); /* with header */

    } else if (static_lenb == opt_lenb) {
        send_bits((STATIC_TREES<<1)+eof, 3);
        compress_block((ct_data near *)static_ltree, (ct_data near *)static_dtree);
        cmpr_len_bits += 3 + static_len;
        cmpr_bytelen += cmpr_len_bits >> 3;
        cmpr_len_bits &= 7L;
    } else {
        send_bits((DYN_TREES<<1)+eof, 3);
        send_all_trees(l_desc.max_code+1, d_desc.max_code+1, max_blindex+1);
        compress_block((ct_data near *)dyn_ltree, (ct_data near *)dyn_dtree);
        cmpr_len_bits += 3 + opt_len;
        cmpr_bytelen += cmpr_len_bits >> 3;
        cmpr_len_bits &= 7L;
    }
    AssertD(((cmpr_bytelen << 3) + cmpr_len_bits) == bits_sent);//,
            //"bad compressed size");
    init_block();

    if (eof) {
        AssertD(input_len == isize);//, "bad input size");

        bi_windup();
        cmpr_len_bits += 7;  /* align on byte boundary */
    }

    return cmpr_bytelen + (cmpr_len_bits >> 3);
}

/* ===========================================================================
 * Save the match info and tally the frequency counts. Return true if
 * the current block must be flushed.
 */
bool ct_tally (int dist, int lc)
    //int dist;  /* distance of matched string */
    //int lc;    /* match length-MIN_MATCH or unmatched char (if dist==0) */
{
    l_buf[last_lit++] = (u8)lc;
    if (dist == 0) {
        /* lc is the unmatched char */
        dyn_ltree[lc].Freq++;
    } else {
        /* Here, lc is the match length - MIN_MATCH */
        dist--;             /* dist = match distance - 1 */
        Assert((u16)dist < (u16)MaxDist &&
               (u16)lc <= (u16)(MaxMatch - MinMatch) &&
               (u16)d_code(dist) < (u16)D_CODES);//,  "ct_tally: bad match");

        dyn_ltree[length_code[lc]+LITERALS+1].Freq++;
        dyn_dtree[d_code(dist)].Freq++;

        d_buf[last_dist++] = (u16)dist;
        flags |= flag_bit;
    }
    flag_bit <<= 1;

    /* Output the flags if they fill a byte: */
    if ((last_lit & 7) == 0) {
        flag_buf[last_flags++] = flags;
        flags = 0, flag_bit = 1;
    }
    /* Try to guess if it is profitable to stop the current block here */
    if ((last_lit & 0xfff) == 0) {
        /* Compute an upper bound for the compressed length */
        u32 out_length = (u32)last_lit*8L;
        u32 in_length = (u32)StrStart-BlockStart;
        int dcode;
        for (dcode = 0; dcode < D_CODES; dcode++) {
            out_length += (u32)dyn_dtree[dcode].Freq*(5L+extra_dbits[dcode]);
        }
        out_length >>= 3;
        if (last_dist < last_lit/2 && out_length < in_length/2) return true;
    }
    return ((last_lit == LIT_BUFSIZE-1) || (last_dist == DIST_BUFSIZE));
    /* We avoid equality with LIT_BUFSIZE because of wraparound at 64K
     * on 16 bit machines and because stored blocks are restricted to
     * 64K-1 bytes.
     */
}

/* ===========================================================================
 * Send the block data compressed using the given Huffman trees
 */
void compress_block(ct_data near *ltree, ct_data near *dtree)
    //ct_data near *ltree; /* literal tree */
    //ct_data near *dtree; /* distance tree */
{
    unsigned dist;      /* distance of matched string */
    int lc;             /* match length or unmatched char (if dist == 0) */
    unsigned lx = 0;    /* running index in l_buf */
    unsigned dx = 0;    /* running index in d_buf */
    unsigned fx = 0;    /* running index in flag_buf */
    u8 flag = 0;       /* current flags */
    unsigned code;      /* the code to send */
    int extra;          /* number of extra bits to send */

    if (last_lit != 0) do {
        if ((lx & 7) == 0) flag = flag_buf[fx++];
        lc = l_buf[lx++];
        if ((flag & 1) == 0) {
            send_code(lc, ltree); /* send a literal byte */
        } else {
            /* Here, lc is the match length - MIN_MATCH */
            code = length_code[lc];
            send_code(code+LITERALS+1, ltree); /* send the length code */
            extra = extra_lbits[code];
            if (extra != 0) {
                lc -= base_length[code];
                send_bits(lc, extra);        /* send the extra length bits */
            }
            dist = d_buf[dx++];
            /* Here, dist is the match distance - 1 */
            code = d_code(dist);
            Assert(code < D_CODES);//, "bad d_code");

            send_code(code, dtree);       /* send the distance code */
            extra = extra_dbits[code];
            if (extra != 0) {
                dist -= base_dist[code];
                send_bits(dist, extra);   /* send the extra distance bits */
            }
        } /* literal or match pair ? */
        flag >>= 1;
    } while (lx < last_lit);

    send_code(END_BLOCK, ltree);
}



/* ===========================================================================
 * Initialize the bit string routines.
 */
void bi_init (char *tgt_buf, unsigned tgt_size, int flsh_allowed)
    //char *tgt_buf;
    //unsigned tgt_size;
    //int flsh_allowed;
{
    out_buf = tgt_buf;
    out_size = tgt_size;
    out_offset = 0;
    flush_flg = flsh_allowed;

    bi_buf = 0;
    bi_valid = 0;
#ifdef _DEBUG
    bits_sent = 0L;
#endif
}

/* ===========================================================================
 * Send a value on a given number of bits.
 * IN assertion: length <= 16 and value fits in length bits.
 */
void send_bits(int value, int length)
    //int value;  /* value to send */
    //int length; /* number of bits */
{
#ifdef _DEBUG
    Assert(length > 0 && length <= 15);//, "invalid length");
    bits_sent += (u32)length;
#endif
    /* If not enough room in bi_buf, use (bi_valid) bits from bi_buf and
     * (Buf_size - bi_valid) bits from value to flush the filled bi_buf,
     * then fill in the rest of (value), leaving (length - (Buf_size-bi_valid))
     * unused bits in bi_buf.
     */
    bi_buf |= (value << bi_valid);
    bi_valid += length;
    if (bi_valid > (int)Buf_size) {
        PUTSHORT(bi_buf);
        bi_valid -= Buf_size;
        bi_buf = (unsigned)value >> (length - bi_valid);
    }
}

/* ===========================================================================
 * Reverse the first len bits of a code, using straightforward code (a faster
 * method would use a table)
 * IN assertion: 1 <= len <= 15
 */
unsigned bi_reverse(unsigned code, int len)
    //unsigned code; /* the value to invert */
    //int len;       /* its bit length */
{
    register unsigned res = 0;
    do {
        res |= code & 1;
        code >>= 1, res <<= 1;
    } while (--len > 0);
    return res >> 1;
}


/* ===========================================================================
 * Write out any remaining bits in an incomplete byte.
 */
void bi_windup()
{
    if (bi_valid > 8) {
        PUTSHORT(bi_buf);
    } else if (bi_valid > 0) {
        PUTBYTE(bi_buf);
    }
    if (flush_flg) {
        flush_outbuf(out_buf, &out_offset);
    }
    bi_buf = 0;
    bi_valid = 0;
#ifdef _DEBUG
    bits_sent = (bits_sent+7) & ~7;
#endif
}

/* ===========================================================================
 * Copy a stored block to the zip file, storing first the length and its
 * one's complement if requested.
 */
void copy_block(char *block, unsigned len, int header)
    //char *block;  /* the input data */
    //unsigned len; /* its length */
    //int header;   /* true if block header must be written */
{
    bi_windup();              /* align on byte boundary */

    if (header) {
        PUTSHORT((u16)len);
        PUTSHORT((u16)~len);
#ifdef _DEBUG
        bits_sent += 2*16;
#endif
    }
    if (flush_flg) {
        flush_outbuf(out_buf, &out_offset);
        out_offset = len;
        flush_outbuf(block, &out_offset);
    } else if (out_offset + len > out_size) {
        error("output buffer too small for in-memory compression");
    } else {
        memcpy(out_buf + out_offset, block, len);
        out_offset += len;
    }
#ifdef _DEBUG
    bits_sent += (u32)len<<3;
#endif
}

#endif //!NO_COMPRESSION
