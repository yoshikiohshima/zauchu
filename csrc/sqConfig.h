/* sqConfig.h -- platform identification and configuration */

#if defined(__MWERKS__) && !defined(macintosh)
  /* CodeWarrior 8 neglects to define "macintosh" */
# define macintosh
#endif

#if defined(WIN32) || defined(_WIN32) || defined(Win32)
  /* Some compilers use different win32 definitions.
     Define WIN32 so we have only to check for one symbol. */
# if !defined(WIN32)
#  define WIN32
# endif
#endif

#if defined(macintosh)
# if defined(SQ_CONFIG_DONE)
#   error configuration conflict
# endif
# define SQ_CONFIG_DONE
#endif

#if defined(ACORN)
# if defined(SQ_CONFIG_DONE)
#   error configuration conflict
# endif
# define SQ_CONFIG_DONE
#endif

#if defined(WIN32)
# if defined(SQ_CONFIG_DONE)
#   error configuration conflict
# endif
# if defined(_M_IX86) || defined(X86)
  /* x86 systems */
#  define DOUBLE_WORD_ALIGNMENT
#  define DOUBLE_WORD_ORDER
  /* Note: We include a generic sqWin32.h to override some settings */
#  include "sqWin32.h"
#  define SQ_CONFIG_DONE
# elif defined(_WIN32_WCE)
#  include "sqWin32.h"
#  define SQ_CONFIG_DONE
# else
#  error unsupported win32 processor type (alpha?!)
# endif
#endif

/* for Unix variants, this file must define the following symbols as appropriate:

   HAS_D_NAMLEN
      defined if struct dirent has d_namlen field and hence directory names
      are not null terminated.  if underfined then directory names are null
      terminated.

   HAS_TIMEZONE
      defined if the external variable timezone is available, containing the
      local offset from GMT in seconds.  if  undefined then the tm structure
      must contain the same information in the tm_gmtoff field.  (Consider
      defining NEED_TZSET too!)

   HAS_ON_EXIT
      defined if cleanup functions are declared with on_exit().  If
      undefined then cleanup functions are declared with atexit().

   HAS_MSB_FIRST
      defined if the most significant byte is first in an int.

   HAS_LSB_FIRST
      defined if the least significant byte is first in an int.
      (Complains if neither of the last two are defined.)

   HAS_SUN_AUDIO
      defined if the platform supports the Sun /dev/audio device.

   DOUBLE_WORD_ALIGNMENT
      defined if the platform CANNOT support double-word accesses at an
      arbitrary word address.
      
   DOUBLE_WORD_ORDER
      defined if the platform stores floats in the opposite order
      to the Squeak image (the image is always PowerPC order).

   SUN_FUN_KEYS
      defined for Sun type 3 or 4 keyboards to enable the editing keys
      (Again, Undo, Copy, Paste, Cut, Find, Stop on the "left keypad",
      and PgUp, PgDn, Home, End keys on the right keypad).

   NEED_FILIO
      defined if the platform requires <sys/filio.h> to be included
      to get a definition for FIONBIO.

   NEED_SELECT
      defined if the platform requires <sys/select.h> to be included
      to get definitions for select().

   NEED_TZSET
      defined if the platform required tzset() to be called explicitly
      before reading the local wall clock.

   JUMP_ALIGN_BYTE
      defined if the platform has no instruction alignment restrictions
      (e.g. Pentium).

   JUMP_ALIGN_STRICT
      defined if the platform has word-aligned instructions, and cannot
      tolerate the low two bits of an address being non-zero (e.g.
      Sparc).

  The symbol "UNIX" should defined below if any variant of Unix or
  Linux is recognized.
*/

#if defined(sun) && (defined(sparc) || defined(__sparc))
# if defined(SQ_CONFIG_DONE)
#   error configuration conflict
# endif
# include <errno.h>
# ifdef ECHRNG					/* Sparc/Solaris */
#   undef  HAS_D_NAMLEN
#   define HAS_TIMEZONE
#   undef  HAS_ON_EXIT
#   define HAS_MSB_FIRST
#   undef  HAS_SUN_AUDIO
#   define DOUBLE_WORD_ALIGNMENT
#   undef  DOUBLE_WORD_ORDER
#   define SUN_FUN_KEYS
#   define NEED_FILIO
#   define NEED_TZSET
#   define JUMP_ALIGN_STRICT
#   define SQ_CONFIG_DONE
# else						/* Sparc/SunOS */
#   include <unistd.h>
#   define HAS_D_NAMLEN
#   undef  HAS_TIMEZONE
#   define HAS_ON_EXIT
#   define HAS_MSB_FIRST
#   undef  HAS_SUN_AUDIO
#   define DOUBLE_WORD_ALIGNMENT
#   undef  DOUBLE_WORD_ORDER
#   define SUN_FUN_KEYS
#   define NEED_TZSET
#   define JUMP_ALIGN_STRICT
#   define SQ_CONFIG_DONE
# endif
# if !defined(UNIX)
#  define UNIX
# endif
#endif

#if defined(sun) && defined(i386)		/* iX86/Solaris */
# if defined(SQ_CONFIG_DONE)
#   error configuration conflict
# endif
# undef  HAS_D_NAMLEN
# define HAS_TIMEZONE
# undef  HAS_ON_EXIT
# define HAS_LSB_FIRST
# undef  HAS_SUN_AUDIO
# undef  DOUBLE_WORD_ALIGNMENT
# define DOUBLE_WORD_ORDER
# define NEED_FILIO
# define NEED_TZSET
# define JUMP_ALIGN_BYTE
# define SQ_CONFIG_DONE
# if !defined(UNIX)
#  define UNIX
# endif
#endif

#if defined(mips) || defined(__mips)
# if defined(_SYSTYPE_SVR4)			/* (SGI)/IRIX */
#  if defined(SQ_CONFIG_DONE)
#    error configuration conflict
#  endif
#  undef  HAS_D_NAMLEN
#  define HAS_TIMEZONE
#  undef  HAS_ON_EXIT
#  define HAS_MSB_FIRST
#  undef  HAS_SUN_AUDIO
#  define  DOUBLE_WORD_ALIGNMENT
#  undef  DOUBLE_WORD_ORDER
#  define JUMP_ALIGN_STRICT
#  define SQ_CONFIG_DONE
#  if !defined(unix)
#   define unix
#  endif
# endif
#endif

#if defined(linux)
# if defined(i386)				/* iX86/Linux */
#   if defined(SQ_CONFIG_DONE)
#     error configuration conflict
#   endif
#   undef  HAS_D_NAMLEN
#   define HAS_TIMEZONE
#   undef  HAS_ON_EXIT
#   define HAS_LSB_FIRST
#   undef  HAS_SUN_AUDIO
#   undef  DOUBLE_WORD_ALIGNMENT
#   define DOUBLE_WORD_ORDER
#   define NEED_TZSET
#   define JUMP_ALIGN_BYTE
#  define SQ_CONFIG_DONE
# endif
# if defined(powerpc)				/* PPC/Linux */
#   if defined(SQ_CONFIG_DONE)
#     error configuration conflict
#   endif
#   undef  HAS_D_NAMLEN
#   define HAS_TIMEZONE
#   undef  HAS_ON_EXIT
#   define HAS_MSB_FIRST
#   undef  HAS_SUN_AUDIO
#   undef  DOUBLE_WORD_ALIGNMENT
#   undef  DOUBLE_WORD_ORDER
#   define NEED_TZSET
#   define SQ_CONFIG_DONE
# endif
# if defined(arm)				/* SA110/Linux, maybe Itsy */
#   if defined(SQ_CONFIG_DONE)
#     error configuration conflict
#   endif
#   undef  HAS_D_NAMLEN
#   define HAS_TIMEZONE
#   undef  HAS_ON_EXIT
#   define HAS_LSB_FIRST
#   undef  HAS_SUN_AUDIO
#   undef  DOUBLE_WORD_ALIGNMENT
#   undef  DOUBLE_WORD_ORDER
#   define NEED_TZSET
#   define SQ_CONFIG_DONE
# endif
# if !defined(UNIX)
#  define UNIX
# endif
#endif

#if defined(__FreeBSD__)
# if defined(i386)				/* iX86/FreeBSD */
#   if defined(SQ_CONFIG_DONE)
#     error configuration conflict
#   endif
#   undef  HAS_D_NAMLEN
#   undef  HAS_TIMEZONE
#   undef  HAS_ON_EXIT
#   define HAS_LSB_FIRST
#   undef  HAS_SUN_AUDIO
#   undef  DOUBLE_WORD_ALIGNMENT
#   define DOUBLE_WORD_ORDER
#   define JUMP_ALIGN_BYTE
#   define SQ_CONFIG_DONE
# endif
# if !defined(UNIX)
#  define UNIX
# endif
#endif

#if defined(__alpha)				/* Alpha/OSF1 */
# if defined(SQ_CONFIG_DONE)
#   error configuration conflict
# endif
# define HAS_D_NAMLEN
# undef  HAS_TIMEZONE
# undef  HAS_ON_EXIT
# define HAS_LSB_FIRST
# undef  HAS_SUN_AUDIO
# define DOUBLE_WORD_ALIGNMENT
# define DOUBLE_WORD_ORDER
# define NEED_TZSET
# define SQ_CONFIG_DONE
# if !defined(UNIX)
#  define UNIX
# endif
#endif

#if defined(hpux) || defined (__hpux)		/* HPPA/HP-UX */
# if defined(SQ_CONFIG_DONE)
#   error configuration conflict
# endif
# define HAS_D_NAMLEN
# define HAS_TIMEZONE
# undef  HAS_ON_EXIT
# define HAS_MSB_FIRST
# undef  HAS_SUN_AUDIO
# define DOUBLE_WORD_ALIGNMENT
# undef  DOUBLE_WORD_ORDER
# define NEED_TZSET
  extern int h_errno;	/* loser! */
# define SQ_CONFIG_DONE
# if !defined(UNIX)
#  define UNIX
# endif
#endif

#if defined(_AIX) || defined(_M_UNIX)
# if defined(SQ_CONFIG_DONE)
#   error configuration conflict
# endif
# undef  HAS_D_NAMLEN				/* IBM RS6000/AIX */
# define HAS_TIMEZONE
# undef  HAS_ON_EXIT
# define HAS_MSB_FIRST
# undef  HAS_SUN_AUDIO
# undef  DOUBLE_WORD_ALIGNMENT
# undef  DOUBLE_WORD_ORDER
# define NEED_SELECT
# define SQ_CONFIG_DONE
# if !defined(UNIX)
#  define UNIX
# endif
#endif

#if defined(ZAURUS)
#undef DOUBLE_WORD_ALIGNMENT
#define DOUBLE_WORD_ORDER
#define SQ_CONFIG_DONE
#endif


#if !defined(SQ_CONFIG_DONE)
# error test for, and describe, your architecture here.
#endif
