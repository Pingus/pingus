#include "CompHead.h"

//exclude from some builds
#ifndef NO_COMPRESSION


//PUBLIC SECTION
#ifdef _DEBUG
unsigned long isize;
#endif
//END PUBLIC SECTION



//INIT SECTION
char FileOutbuf[1024]; // output buffer for compression to file
//END



const uint HashBits = 15;  // Number of bits used to hash strings

const uint HashSize (1 << HashBits);
const uint HashMask = HashSize - 1;
const uint WMask = (WSize - 1);
// HASH_SIZE and WSIZE must be powers of two

const uint NIL = 0;
// Tail of hash chains

const uint TooFar = 4096;
// Matches of length 3 are discarded if their distance exceeds TOO_FAR

// ===========================================================================
// Local data used by the "longest match" routines.

typedef u16 Pos;
typedef uint IPos;
// A Pos is an index in the character window. We use short instead of int to
// save space in the various tables. IPos is used only for parameter passing.

u8 Window[2 * WSize];
// Sliding window. Input bytes are read into the second half of the window,
// and move to the first half later to keep a dictionary of at least WSIZE
// bytes. With this organization, matches are limited to a distance of
// WSIZE-MAX_MATCH bytes, but this ensures that IO is always
// performed with a length multiple of the block size. Also, it limits
// the window size to 64K, which is quite useful on MSDOS.

Pos Prev[WSize];
// Link to older string with same hash index. To limit the size of this
// array to 64K, this link is maintained only for the last 32K strings.
// An index in this array is thus a window index modulo 32K.

Pos Head[HashSize];
// Heads of the hash chains or NIL. If your compiler thinks that
// HASH_SIZE is a dynamic value, recompile with -DDYN_ALLOC.


const u32 WindowSize = 2 * WSize;
// The size of the window

long BlockStart;
// window position at the beginning of the current output block. Gets
// negative when the window is moved backwards.

uint InsH;  // hash index of string to be inserted

const uint HShift = ((HashBits + MinMatch - 1) / MinMatch);
// Number of bits by which ins_h and del_h must be shifted at each
// input step. It must be such that after MIN_MATCH steps, the oldest
// byte no longer takes part in the hash key, that is:
//   H_SHIFT * MIN_MATCH >= HASH_BITS

uint PrevLength;
// Length of the best match at previous step. Matches not greater than this
// are discarded. This is used in the lazy match evaluation.

uint StrStart;      // start of string to insert
uint MatchStart;    // start of matching string
bool EOFile;        // flag set at end of input file
uint LookAhead;     // number of valid bytes ahead in window

//Parameters for compression mode

const uint MaxChainLength = 4096;
// To speed up deflation, hash chains are never searched beyond this length.
// A higher limit improves compression ratio but degrades the speed.

const uint MaxLazyMatch = 258;
// Attempt to find a better match only when the current match is strictly
// smaller than this value. This mechanism is used only for compression
// levels >= 4.

const uint GoodMatch = 32;
// Use a faster search when the previous match is longer than this

int NiceMatch; // Stop searching when current match exceeds this


const int Equal = 0;
/* result of memcmp for equal strings */

/* ===========================================================================
 *  Prototypes for local functions.
 */

void FillWindow();
int  LongestMatch(IPos CurMatch);

#ifdef _DEBUG
void CheckMatch(IPos Start, IPos Match, int Length);
#endif

/* ===========================================================================
 * Update a hash value with the given input byte
 * IN  assertion: all calls to to UPDATE_HASH are made with consecutive
 *    input characters, so that a running hash key can be computed from the
 *    previous key instead of complete recalculation each time.
 */

void inline UpdateHash(u8 b)
{
	InsH = ((InsH << HShift) ^ b) & HashMask;
}

/* ===========================================================================
 * Insert string s in the dictionary and set match_head to the previous head
 * of the hash chain (the most recent string with same hash key). Return
 * the previous length of the hash chain.
 * IN  assertion: all calls to to INSERT_STRING are made with consecutive
 *    input characters and the first MIN_MATCH bytes of s are valid
 *    (except for the last MIN_MATCH-1 bytes of the input file).
 */

IPos inline InsertString(IPos HashHead) //StrStart is the other value
{
	UpdateHash(Window[StrStart + (MinMatch - 1)]);
	IPos i = Head[InsH];
	Prev[StrStart & WMask] = i;
	Head[InsH] = StrStart;
	return i;
}


/* ===========================================================================
 * Initialize the "longest match" routines for a new file
 *
 * IN assertion: window_size is > 0 if the input file is already read or
 *    mmap'ed in the window[] array, 0 otherwise. In the first case,
 *    window_size is sufficient to contain the whole input file plus
 *    MIN_LOOKAHEAD bytes (to avoid referencing memory beyond the end
 *    of window[] when looking for matches towards the end).
 */
void LmInit()
{
	// Initialize the hash table (avoiding 64K overflow for 16 bit systems).
    // prev[] will be initialized on the fly.
    Head[HashSize-1] = NIL;
    memset((char*) Head, NIL, (uint) (HashSize - 1) * sizeof(*Head));

    // Set the default configuration parameters:
	//Do not try to find matches longer than the maximum
    NiceMatch = MaxMatch;

    StrStart = 0;
    BlockStart = 0;

    // Read 64K in one step, double the window size
	LookAhead = ReadBuf((char*) Window, WSize * 2);

	EOFile = (LookAhead == 0);
	if (EOFile) return;

    // Make sure that we always have enough lookahead. This is important
    // if input comes from a device such as a tty.
    if (LookAhead < MinLookahead)
		FillWindow();

    InsH = 0;

	uint j;
    for (j = 0; j < MinMatch - 1; j++)
		UpdateHash(Window[j]);
    // If lookahead < MIN_MATCH, ins_h is garbage, but this is
    // not important since only literal bytes will be emitted.
}

// ===========================================================================
// Set match_start to the longest match starting at the given string and
// return its length. Matches shorter or equal to prev_length are discarded,
// in which case the result is equal to prev_length and match_start is
// garbage.
// IN assertions: cur_match is the head of the hash chain for the current
//   string (strstart) and its distance is <= MAX_DIST, and prev_length >= 1

int LongestMatch(IPos CurMatch)
{
    uint ChainLength = MaxChainLength;		// max hash chain length
    u8* Scan = Window + StrStart;			// current string
    u8* Match;								// matched string
    int Len;								// length of current match
    int BestLen = PrevLength;				// best match length so far

    IPos Limit = StrStart > (IPos)MaxDist ? StrStart - (IPos)MaxDist : NIL;
    // Stop when cur_match becomes <= limit. To simplify the code,
    // we prevent matches with the string of window index 0.


    u8* StrEnd  = Window + StrStart + MaxMatch;
    u8 ScanEnd1 = Scan[BestLen - 1];
    u8 ScanEnd  = Scan[BestLen];

    /* Do not waste too much time if we already have a good match: */
    if (PrevLength >= GoodMatch)
	{
        ChainLength >>= 2;
    }

    Assert(StrStart <= WindowSize - MinLookahead);//, "insufficient lookahead");

    do
	{
        Assert(CurMatch < StrStart);//, "no future");
        Match = Window + CurMatch;

        /* Skip to next match if the match length cannot increase
         * or if the match length is less than 2:
         */

        if (Match[BestLen]     != ScanEnd  ||
            Match[BestLen - 1] != ScanEnd1 ||
            *Match             != Scan[0]  ||
            *++Match           != Scan[1])
			continue;

        // The check at best_len-1 can be removed because it will be made
        // again later. (This heuristic is not always a win.)
        // It is not necessary to compare scan[2] and match[2] since they
        // are always equal when the other bytes match, given that
        // the hash keys are equal and that HASH_BITS >= 8.
        Scan += 2;
		Match++;

        /* We check for insufficient lookahead only every 8th comparison;
         * the 256th check will be made at strstart+258.
         */
        do {
        } while ((*++Scan == *++Match) && (*++Scan == *++Match) &&
				 (*++Scan == *++Match) && (*++Scan == *++Match) &&
				 (*++Scan == *++Match) && (*++Scan == *++Match) &&
				 (*++Scan == *++Match) && (*++Scan == *++Match) &&
                 Scan < StrEnd);

        Assert(Scan <= Window + (uint)(WindowSize-1));//, "wild scan");

        Len = MaxMatch - (int)(StrEnd - Scan);
        Scan = StrEnd - MaxMatch;

        if (Len > BestLen) {
            MatchStart = CurMatch;
            BestLen = Len;
            if (Len >= NiceMatch) break;

            ScanEnd1 = Scan[BestLen - 1];
            ScanEnd  = Scan[BestLen];
        }
    } while ((CurMatch = Prev[CurMatch & WMask]) > Limit
             && --ChainLength != 0);

    return BestLen;
}


#ifdef _DEBUG
/* ===========================================================================
 * Check that the match at match_start is indeed a match.
 */
void CheckMatch(IPos Start, IPos Match, int Len)
{
    // check that the match is indeed a match
    if (memcmp((char*) Window + Match,
                (char*) Window + Start, Len) != Equal)
	{
        error("invalid match");
    }
}
#else
#  define CheckMatch(Start, Match, Len)
#endif

/* ===========================================================================
 * Fill the window when the lookahead becomes insufficient.
 * Updates strstart and lookahead, and sets eofile if end of input file.
 *
 * IN assertion: lookahead < MIN_LOOKAHEAD && strstart + lookahead > 0
 * OUT assertions: strstart <= window_size-MIN_LOOKAHEAD
 *    At least one byte has been read, or eofile is set; file reads are
 *    performed for at least two bytes (required for the translate_eol option).
 */
void FillWindow()
{
    uint n, m;
    uint More;    // Amount of free space at the end of the window.

    do
	{
        More = (unsigned)(WindowSize - (u32)LookAhead - (u32)StrStart);

        // If the window is almost full and there is insufficient lookahead,
        // move the upper half to the lower one to make room in the upper half.

        if (More == (unsigned)EOF)
		{
			//Occurs for files one byte long
            More--;
		}
		else if (StrStart >= WSize + MaxDist)
		{
            // By the IN assertion, the window is not empty so we can't confuse
            // more == 0 with more == 64K on a 16 bit machine.
            memcpy((char*) Window, (char*) Window + WSize, (uint) WSize);
            MatchStart -= WSize;
            StrStart    -= WSize; // we now have strstart >= MAX_DIST:

            BlockStart -= (long) WSize;

			//Neil Optimisation, HASH_SIZE == WSIZE
			//:. only do one loop, with two operations in it

			//This must be true, if not use the original ZIP code
			Assert(HashSize == WSize);


            for (n = 0; n < HashSize; n++)
			{
				m = Head[n];
				//NB: Do NOT use MAX as More is uint :. always > 0
				Head[n] = (Pos) (m >= WSize ? m-WSize : NIL);

				//Dependant on WSIZE
				m = Prev[n];
				Prev[n] = (Pos)(m >= WSize ? m-WSize : NIL);
				// If n is not on any hash chain, prev[n] is garbage but
				// its value will never be used.
            }
            More += WSize;
        }
        if (EOFile) return;

        /* If there was no sliding:
         *    strstart <= WSIZE+MAX_DIST-1 && lookahead <= MIN_LOOKAHEAD - 1 &&
         *    more == window_size - lookahead - strstart
         * => more >= window_size - (MIN_LOOKAHEAD-1 + WSIZE + MAX_DIST-1)
         * => more >= window_size - 2*WSIZE + 2
         * In the MMAP or BIG_MEM case (not yet supported in gzip),
         *   window_size == input_size + MIN_LOOKAHEAD  &&
         *   strstart + lookahead <= input_size => more >= MIN_LOOKAHEAD.
         * Otherwise, window_size == 2*WSIZE so more >= 2.
         * If there was sliding, more >= WSIZE. So in all cases, more >= 2.
         */
        Assert(More >= 2); //more < 2

        n = ReadBuf((char*) Window + StrStart + LookAhead, More);
		EOFile = (n == 0);
		LookAhead += n;

    } while ((LookAhead < MinLookahead) && !EOFile);
}

/* ===========================================================================
 * Flush the current block, with given end-of-file flag.
 * IN assertion: strstart is set to the end of the current match.
 */

inline u32 DoFlushBlock(bool eof)
{
	return flush_block(BlockStart >= 0L ? (char*)&Window[(unsigned)BlockStart] : \
                (char*)NULL, (long)StrStart - BlockStart, eof);
}


/* ===========================================================================
 * Same as above, but achieves better compression. We use a lazy
 * evaluation for matches: a match is finally adopted only if there is
 * no better match at the next window position.
 */
u32 Deflate()
{

    IPos HashHead = NIL;				// head of hash chain
    IPos PrevMatch;						// previous match
    bool Flush;							// set if current block must be flushed
    bool MatchAvailable = false;		// set if previous match exists
    uint MatchLength = MinMatch - 1;	// length of best match

#ifdef _DEBUG
    extern u32 isize;          // byte length of input file, for debug only
#endif

    // Process the input block.
    while (LookAhead != 0)
	{
        // Insert the string window[strstart .. strstart+2] in the
        // dictionary, and set hash_head to the head of the hash chain:
        if (LookAhead >= MinMatch)
			HashHead = InsertString(HashHead);

        // Find the longest match, discarding those <= prev_length.
        PrevLength = MatchLength;
		PrevMatch = MatchStart;
        MatchLength = MinMatch - 1;

        if ((HashHead != NIL) &&
			(PrevLength < MaxLazyMatch) &&
            (StrStart - HashHead <= MaxDist))
		{
            // To simplify the code, we prevent matches with the string
            // of window index 0 (in particular we have to avoid a match
            // of the string with itself at the start of the input file).

            // Do not look for matches beyond the end of the input.
            // This is necessary to make deflate deterministic.
            if ((uint) NiceMatch > LookAhead)
				NiceMatch = (int) LookAhead;

            MatchLength = LongestMatch(HashHead);
            // LongestMatch() sets match_start
            if (MatchLength > LookAhead)
				MatchLength = LookAhead;

            // Ignore a length 3 match if it is too distant:
            if ((MatchLength == MinMatch) && (StrStart - MatchStart > TooFar))
			{
                // If prev_match is also MIN_MATCH, match_start is garbage
                // but we will ignore the current match anyway.
                MatchLength = MinMatch - 1;
            }
        }
        // If there was a match at the previous step and the current
        // match is not better, output the previous match:
        if ((PrevLength >= MinMatch) && (MatchLength <= PrevLength))
		{
            uint MaxInsert = StrStart + LookAhead - MinMatch;
            CheckMatch(StrStart - 1, PrevMatch, PrevLength);

            Flush = ct_tally(StrStart - 1 - PrevMatch, PrevLength - MinMatch);

            // Insert in hash table all strings up to the end of the match.
            // strstart-1 and strstart are already inserted.
            LookAhead -= PrevLength - 1;
            PrevLength -= 2;
            do
			{
                if (++StrStart <= MaxInsert)
				{
                    HashHead = InsertString(HashHead);
                    // strstart never exceeds WSIZE-MAX_MATCH, so there are
                    // always MIN_MATCH bytes ahead.
                }
            }
			while (--PrevLength != 0);

            StrStart++;
            MatchAvailable = false;
            MatchLength = MinMatch - 1;

            if (Flush)
			{
				DoFlushBlock(false);
				BlockStart = StrStart;
			}

        }
		else if (MatchAvailable)
		{
            // If there was no match at the previous position, output a
            // single literal. If there was a match but the current match
            // is longer, truncate the previous match to a single literal.
            if (ct_tally (0, Window[StrStart-1]))
			{
                DoFlushBlock(false);
				BlockStart = StrStart;
            }
            StrStart++;
            LookAhead--;
        }
		else
		{
            /* There is no previous match to compare with, wait for
             * the next step to decide.
             */
            MatchAvailable = true;
            StrStart++;
            LookAhead--;
        }
        AssertD((StrStart <= isize) && (LookAhead <= isize));//, "a bit too far");

        // Make sure that we always have enough lookahead, except
        // at the end of the input file. We need MAX_MATCH bytes
        // for the next match, plus MIN_MATCH bytes to insert the
        // string following the next match.
        if (LookAhead < MinLookahead)
			FillWindow();
    }

    if (MatchAvailable)
		ct_tally (0, Window[StrStart-1]);

    return DoFlushBlock(true); // eof
}







//NEIL SPECIFIC


u32 filecompress(int* cmpr_method)
{

    /* Set the defaults for file compression. */
    //zfile = zipfile; handled in SetupDefTree

#ifdef _DEBUG
	isize = 0;
#endif

    /* Initialize deflate's internals and execute file compression. */
    bi_init(FileOutbuf, sizeof(FileOutbuf), true);
    ct_init(cmpr_method);
    LmInit();
    return Deflate();
}

#endif //!NO_COMPRESSION