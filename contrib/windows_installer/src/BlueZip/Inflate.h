//privzip.h

#define WSIZE   0x8000  /* window size--must be a power of two, and */


#define MAX_BITS    13                 /* used in unshrink() */
#define HSIZE       (1 << MAX_BITS)    /* size of global work area */
   union work {
     struct {                 /* unshrink(): */
       int Parent[HSIZE];    /* (8192 * sizeof(shrint)) == 16KB minimum */
       u8 value[HSIZE];        /* 8KB */
       u8 Stack[HSIZE];        /* 8KB */
     } shrink;                  /* total = 32KB minimum; 80KB on Cray/Alpha */
     u8 Slide[WSIZE];        /* explode(), inflate(), unreduce() */
   };



/*
  
	Copyright (c) 1990-2001 Info-ZIP.  All rights reserved.

  See the accompanying file LICENSE, version 2000-Apr-09 or later
  (the contents of which are also included in unzip.h) for terms of use.
  If, for some reason, all these files are missing, the Info-ZIP license
  also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
*/
/*---------------------------------------------------------------------------

  globals.h

  There is usually no need to include this file since unzip.h includes it.

  This header file is used by all of the UnZip source files.  It contains
  a struct definition that is used to "house" all of the global variables.
  This is done to allow for multithreaded environments (OS/2, NT, Win95,
  Unix) to call UnZip through an API without a semaphore.  REENTRANT should
  be defined for all platforms that require this.

  GLOBAL CONSTRUCTOR AND DESTRUCTOR (API WRITERS READ THIS!!!)
  ------------------------------------------------------------

  No, it's not C++, but it's as close as we can get with K&R.

  The main() of each process that uses these globals must include the
  CONSTRUCTGLOBALS; statement.  This will malloc enough memory for the
  structure and initialize any variables that require it.  This must
  also be done by any API function that jumps into the middle of the
  code.

  The DESTROYGLOBALS; statement should be inserted before EVERY "EXIT(n)".
  Naturally, it also needs to be put before any API returns as well.
  In fact, it's much more important in API functions since the process
  will NOT end, and therefore the memory WON'T automatically be freed
  by the operating system.

  USING VARIABLES FROM THE STRUCTURE
  ----------------------------------

  All global variables must now be prefixed with `G.' which is either a
  global struct (in which case it should be the only global variable) or
  a macro for the value of a local pointer variable that is passed from
  function to function.  Yes, this is a pain.  But it's the only way to
  allow full reentrancy.

  ADDING VARIABLES TO THE STRUCTURE
  ---------------------------------

  If you make the inclusion of any variables conditional, be sure to only
  check macros that are GUARANTEED to be included in every module.
  For instance, newzip and pwdarg are needed only if CRYPT is TRUE,
  but this is defined after unzip.h has been read.  If you are not careful,
  some modules will expect your variable to be part of this struct while
  others won't.  This will cause BIG problems. (Inexplicable crashes at
  strange times, car fires, etc.)  When in doubt, always include it!

  Note also that UnZipSFX needs a few variables that UnZip doesn't.  However,
  it also includes some object files from UnZip.  If we were to conditionally
  include the extra variables that UnZipSFX needs, the object files from
  UnZip would not mesh with the UnZipSFX object files.  Result: we just
  include the UnZipSFX variables every time.  (It's only an extra 4 bytes
  so who cares!)

  ADDING FUNCTIONS
  ----------------

  To support this new global struct, all functions must now conditionally
  pass the globals pointer (pG) to each other.  This is supported by 5 macros:
  __GPRO, __GPRO__, __G, __G__ and __GDEF.  A function that needs no other
  parameters would look like this:

    int extract_or_test_files(__G)
      __GDEF
    {
       ... stuff ...
    }

  A function with other parameters would look like:

    int memextract(__G__ tgt, tgtsize, src, srcsize)
        __GDEF
        u8 *tgt, *src;
        u32 tgtsize, srcsize;
    {
      ... stuff ...
    }

  In the Function Prototypes section of unzpriv.h, you should use __GPRO and
  __GPRO__ instead:

    int  uz_opts                   OF((__GPRO__ int *pargc, char ***pargv));
    int  process_zipfiles          OF((__GPRO));

  Note that there is NO comma after __G__ or __GPRO__ and no semi-colon after
  __GDEF.  I wish there was another way but I don't think there is.


  TESTING THE CODE
  -----------------

  Whether your platform requires reentrancy or not, you should always try
  building with REENTRANT defined if any functions have been added.  It is
  pretty easy to forget a __G__ or a __GDEF and this mistake will only show
  up if REENTRANT is defined.  All platforms should run with REENTRANT
  defined.  Platforms that can't take advantage of it will just be paying
  a performance penalty needlessly.

  SIGNAL MADNESS
  --------------

  This whole pointer passing scheme falls apart when it comes to SIGNALs.
  I handle this situation 2 ways right now.  If you define USETHREADID,
  UnZip will include a 64-entry table.  Each entry can hold a global
  pointer and thread ID for one thread.  This should allow up to 64
  threads to access UnZip simultaneously.  Calling DESTROYGLOBALS()
  will free the global struct and zero the table entry.  If somebody
  forgets to call DESTROYGLOBALS(), this table will eventually fill up
  and UnZip will exit with an error message.  A good way to test your
  code to make sure you didn't forget a DESTROYGLOBALS() is to change
  THREADID_ENTRIES to 3 or 4 in globals.c, making the table real small.
  Then make a small test program that calls your API a dozen times.

  Those platforms that don't have threads still need to be able to compile
  with REENTRANT defined to test and see if new code is correctly written
  to work either way.  For these platforms, I simply keep a global pointer
  called GG that points to the Globals structure.  Good enough for testing.

  I believe that NT has thread level storage.  This could probably be used
  to store a global pointer for the sake of the signal handler more cleanly
  than my table approach.

  ---------------------------------------------------------------------------*/

#ifndef __globals_h
#define __globals_h

#ifdef USE_ZLIB
#  include "zlib.h"
#endif


/*************/
/*  Globals  */
/*************/

typedef struct Globals {
#ifdef DLL
    zvoid *callerglobs; /* pointer to structure of pass-through global vars */
#endif


#ifndef FUNZIP
    /* command options specific to the high level command line interface */
#ifdef MORE
    int M_flag;         /* -M: built-in "more" function */
#endif

    /* internal flags and general globals */
#ifdef MORE
    int height;           /* check for SIGWINCH, etc., eventually... */
    int lines;            /* count of lines displayed on current screen */
# if (defined(SCREENWIDTH) && defined(SCREENLWRAP))
    int width;
    int chars;            /* count of screen characters in current line */
# endif
#endif /* MORE */
#if (defined(IZ_CHECK_TZ) && defined(USE_EF_UT_TIME))
    int tz_is_valid;      /* indicates that timezone info can be used */
#endif
    int noargs;           /* did true command line have *any* arguments? */
    unsigned filespecs;   /* number of real file specifications to be matched */
    unsigned xfilespecs;  /* number of excluded filespecs to be matched */
    int process_all_files;
    int overwrite_mode;   /* 0 - query, 1 - always, 2 - never */
    int create_dirs;      /* used by main(), mapname(), checkdir() */
    int extract_flag;
    int newzip;           /* reset in extract.c; used in crypt.c */
    
	//NEIL: WHAT ARE THESE?
	//LONGINT   real_ecrec_offset;
    //LONGINT   expect_ecrec_offset;
    long csize;           /* used by decompr. (NEXTBYTE): must be signed */
    long used_csize;      /* used by extract_or_test_member(), explode() */

#ifdef DLL
     int fValidate;       /* true if only validating an archive */
     int filenotfound;
     int redirect_data;   /* redirect data to memory buffer */
     int redirect_text;   /* redirect text output to buffer */
# ifndef NO_SLIDE_REDIR
     int redirect_slide;  /* redirect decompression area to mem buffer */
     unsigned _wsize;
# endif
     unsigned redirect_size;       /* size of redirected output buffer */
     u8 *redirect_buffer;         /* pointer to head of allocated buffer */
     u8 *redirect_pointer;        /* pointer past end of written data */
# ifndef NO_SLIDE_REDIR
     u8 *redirect_sldptr;         /* head of decompression slide buffer */
# endif
# ifdef OS2DLL
     cbList(processExternally);    /* call-back list */
# endif
#endif /* DLL */

    char **pfnames;
    char **pxnames;
    char sig[4];
    char answerbuf[10];
//    min_info info[DIR_BLKSIZ];
//    min_info *pInfo;
#endif /* !FUNZIP */
    union work area;                /* see unzpriv.h for definition of work */

#ifdef FUNZIP
    FILE     *in;                   /* file descriptor of compressed stream */
#endif
    u8       *inbuf;               /* input buffer (any size is OK) */
    u8       *inptr;               /* pointer into input buffer */
    int       incnt;

#ifndef FUNZIP
    u32       bitbuf;
    int       bits_left;            /* unreduce and unshrink only */
    int       zipeof;
    char      *argv0;               /* used for NT and EXE_EXTENSION */
    char      *wildzipfn;
    char      *zipfn;    /* GRR:  WINDLL:  must nuke any malloc'd zipfn... */
#ifdef USE_STRM_INPUT
    FILE      *zipfd;               /* zipfile file descriptor */
#else
    int       zipfd;                /* zipfile file handle */
#endif
//    LONGINT   ziplen;
    //LONGINT   cur_zipfile_bufstart; /* extract_or_test, readbuf, ReadByte */
//    LONGINT   extra_bytes;          /* used in unzip.c, misc.c */
    u8       *extra_field;         /* Unix, VMS, Mac, OS/2, Acorn, ... */
    u8       *hold;

//    local_file_hdr  lrec;          /* used in unzip.c, extract.c */
//    cdir_file_hdr   crec;          /* used in unzip.c, extract.c, misc.c */
//    ecdir_rec       ecrec;         /* used in unzip.c, extract.c */
//    struct stat     statbuf;       /* used by main, mapname, check_for_newer */

    int      mem_mode;
    u8      *outbufptr;           /* extract.c static */
    u32      outsize;              /* extract.c static */
    int      reported_backslash;   /* extract.c static */
    int      disk_full;
    int      newfile;

    int      didCRlast;            /* fileio static */
    u32      numlines;             /* fileio static: number of lines printed */
    int      sol;                  /* fileio static: at start of line */
    int      no_ecrec;             /* process static */
#ifdef SYMLINKS
    int      symlnk;
#endif
#ifdef NOVELL_BUG_FAILSAFE
    int      dne;                  /* true if stat() says file doesn't exist */
#endif

    FILE     *outfile;
    u8      *outbuf;
    u8      *realbuf;

#ifndef VMS                        /* if SMALL_MEM, outbuf2 is initialized in */
    u8      *outbuf2;             /*  process_zipfiles() (never changes); */
#endif                             /*  else malloc'd ONLY if unshrink and -a */
#endif /* !FUNZIP */
    u8      *outptr;
    u32      outcnt;               /* number of chars stored in outbuf */
#ifndef FUNZIP
    char     filename[MAX_PATH];  /* also used by NT for temporary SFX path */

#ifdef CMS_MVS
    char     *tempfn;              /* temp file used; erase on close */
#endif

    char *key;         /* crypt static: decryption password or NULL */
    int nopwd;         /* crypt static */
#endif /* !FUNZIP */
    u32 keys[3];       /* crypt static: keys defining pseudo-random sequence */

#if (!defined(DOS_FLX_H68_NLM_OS2_W32) && !defined(AMIGA) && !defined(RISCOS))
#if (!defined(MACOS) && !defined(ATARI) && !defined(VMS))
    int echofd;        /* ttyio static: file descriptor whose echo is off */
#endif /* !(MACOS || ATARI || VMS) */
#endif /* !(DOS_FLX_H68_NLM_OS2_W32 || AMIGA || RISCOS) */

    unsigned hufts;    /* track memory usage */

#ifdef USE_ZLIB
    int inflInit;             /* inflate static: zlib inflate() initialized */
    z_stream dstrm;           /* inflate global: decompression stream */
#else
    struct huft *fixed_tl;    /* inflate static */
    struct huft *fixed_td;    /* inflate static */
    int fixed_bl, fixed_bd;   /* inflate static */
    unsigned wp;              /* inflate static: current position in slide */
    u32 bb;                   /* inflate static: bit buffer */
    unsigned bk;              /* inflate static: bits in bit buffer */
#endif /* ?USE_ZLIB */

#ifndef FUNZIP
#ifdef SMALL_MEM
    char rgchBigBuffer[512];
    char rgchSmallBuffer[96];
    char rgchSmallBuffer2[160];  /* boosted to 160 for local3[] in unzip.c */
#endif

  //  MsgFn *message;
    //InputFn *input;
//    PauseFn *mpause;
  //  PasswdFn *decr_passwd;
    //StatCBFn *statreportcb;
#ifdef WINDLL
    LPUSERFUNCTIONS lpUserFunctions;
#endif

    int incnt_leftover;       /* so improved NEXTBYTE does not waste input */
    u8 *inptr_leftover;

#ifdef VMS_TEXT_CONV
    int VMS_line_state;       /* so native VMS variable-length text files are */
    int VMS_line_length;      /*  readable on other platforms */
    int VMS_line_pad;
#endif
#endif /* !FUNZIP */

#ifdef SYSTEM_SPECIFIC_GLOBALS
    SYSTEM_SPECIFIC_GLOBALS
#endif

} Uz_Globs;  /* end of struct Globals */


/***************************************************************************/


#ifdef FUNZIP
#  if (!defined(USE_ZLIB) || defined(USE_OWN_CRCTAB))
     extern ZCONST u32 near  crc_32_tab[256];
#  else
     extern ZCONST u32 Far *crc_32_tab;
#  endif
#  define CRC_32_TAB  crc_32_tab
#else
#  define CRC_32_TAB  G.crc_32_tab
#endif


Uz_Globs *globalsCtor(void);

/* pseudo constant sigs; they are initialized at runtime so unzip executable
 * won't look like a zipfile
 */
extern char local_hdr_sig[4];
extern char central_hdr_sig[4];
extern char end_central_sig[4];
/* extern char extd_local_sig[4];  NOT USED YET */

#ifdef REENTRANT
#  define G                   (*(Uz_Globs *)pG)
#  define __G                 pG
#  define __G__               pG,
#  define __GPRO              Uz_Globs *pG
#  define __GPRO__            Uz_Globs *pG,
#  define __GDEF              Uz_Globs *pG;
#  ifdef  USETHREADID
     extern int               lastScan;
     void deregisterGlobalPointer OF((__GPRO));
     Uz_Globs *getGlobalPointer   OF((void));
#    define GETGLOBALS()      Uz_Globs *pG = getGlobalPointer();
#    define DESTROYGLOBALS()  {free_G_buffers(pG); deregisterGlobalPointer(pG);}
#  else
     extern Uz_Globs          *GG;
#    define GETGLOBALS()      Uz_Globs *pG = GG;
#    define DESTROYGLOBALS()  {free_G_buffers(pG); free(pG);}
#  endif /* ?USETHREADID */
#  define CONSTRUCTGLOBALS()  Uz_Globs *pG = globalsCtor()
#else /* !REENTRANT */
   extern Uz_Globs            G;
#  define __G
#  define __G__
#  define __GPRO              void
#  define __GPRO__
#  define __GDEF
#  define GETGLOBALS()
#  define CONSTRUCTGLOBALS()  globalsCtor()
#  define DESTROYGLOBALS()
#endif /* ?REENTRANT */

#define uO              G.UzO

#endif /* __globals_h */


//COPIED FROM Inflate.cpp by Neil
int inflate();
void Neil_Init(File Out, File In);

extern struct Globals  G;
