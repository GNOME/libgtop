/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf sysdeps/linux/procmap_smaps.gperf  */
/* Computed positions: -k'1,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 8 "sysdeps/linux/procmap_smaps.gperf"

#include "glibtop_private.h"
#include <glibtop/procmap.h>
#include <stddef.h>
#define SMAP_OFFSET(MEMBER) offsetof(glibtop_map_entry, MEMBER)
#line 14 "sysdeps/linux/procmap_smaps.gperf"
struct smap_value { int name; ptrdiff_t offset; };
#include <string.h>

#define TOTAL_KEYWORDS 8
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 13
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 18
/* maximum key range = 16, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
       0, 19,  5,  0, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19,  5, 19, 19, 19, 19, 19, 19, 19, 19,
       5, 19,  0, 19, 19,  0, 19, 19, 19, 19,
      19,  0, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
      19, 19, 19, 19, 19, 19
    };
  return len + asso_values[(unsigned char)str[len - 1]] + asso_values[(unsigned char)str[0]];
}

struct stringpool_t
  {
    char stringpool_str3[sizeof("Pss")];
    char stringpool_str4[sizeof("Swap")];
    char stringpool_str8[sizeof("Rss")];
    char stringpool_str9[sizeof("Size")];
    char stringpool_str12[sizeof("Shared_Dirty")];
    char stringpool_str13[sizeof("Private_Dirty")];
    char stringpool_str17[sizeof("Shared_Clean")];
    char stringpool_str18[sizeof("Private_Clean")];
  };
static const struct stringpool_t stringpool_contents =
  {
    "Pss",
    "Swap",
    "Rss",
    "Size",
    "Shared_Dirty",
    "Private_Dirty",
    "Shared_Clean",
    "Private_Clean"
  };
#define stringpool ((const char *) &stringpool_contents)
static /* manually added */
const struct smap_value *
_glibtop_find_smap (register const char *str, register size_t len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  3,  4,  0,  0,  0,  3,  4,  0,  0, 12, 13,
       0,  0,  0, 12, 13
    };
  static const struct smap_value wordlist[] =
    {
      {-1}, {-1}, {-1},
#line 18 "sysdeps/linux/procmap_smaps.gperf"
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str3,		SMAP_OFFSET(pss)},
#line 23 "sysdeps/linux/procmap_smaps.gperf"
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str4,		SMAP_OFFSET(swap),},
      {-1}, {-1}, {-1},
#line 19 "sysdeps/linux/procmap_smaps.gperf"
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str8,		SMAP_OFFSET(rss)},
#line 22 "sysdeps/linux/procmap_smaps.gperf"
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str9,		SMAP_OFFSET(size)},
      {-1}, {-1},
#line 21 "sysdeps/linux/procmap_smaps.gperf"
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str12,	SMAP_OFFSET(shared_dirty)},
#line 17 "sysdeps/linux/procmap_smaps.gperf"
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str13,	SMAP_OFFSET(private_dirty)},
      {-1}, {-1}, {-1},
#line 20 "sysdeps/linux/procmap_smaps.gperf"
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str17,	SMAP_OFFSET(shared_clean)},
#line 16 "sysdeps/linux/procmap_smaps.gperf"
      {(int)(size_t)&((struct stringpool_t *)0)->stringpool_str18,	SMAP_OFFSET(private_clean)}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name + stringpool;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
