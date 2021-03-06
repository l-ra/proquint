/* This file is part of proquint: http://github.com/dsw/proquint .
   See License.txt for copyright and terms of use. */

/*
Main driver for library proquint.c.
Please see the article on proquints: http://arXiv.org/html/0901.4016
Daniel S. Wilkerson
*/

#include "proquint.h"

#include <stdio.h>              /* printf, fprintf */
#include <stdlib.h>             /* exit */
#include <ctype.h>              /* isdigit */

/* My atoi; it's easier to write it than to figure out how to get the
   libraries to parse a string into an unsigned int.
*/
static uint32_t my_atoi(int base, char const *s) {
  char const *const s_orig = s; /* for error messages */
  uint32_t ret = 0;
  int c;

  for(; (c=*s); ++s) {
    int value = -1;

    switch(c) {
    /* decimal digits */
    case '0': value = 0; break;
    case '1': value = 1; break;
    case '2': value = 2; break;
    case '3': value = 3; break;
    case '4': value = 4; break;

    case '5': value = 5; break;
    case '6': value = 6; break;
    case '7': value = 7; break;
    case '8': value = 8; break;
    case '9': value = 9; break;

    case 'a': case 'A': value = 10; break;
    case 'b': case 'B': value = 11; break;
    case 'c': case 'C': value = 12; break;
    case 'd': case 'D': value = 13; break;
    case 'e': case 'E': value = 14; break;
    case 'f': case 'F': value = 15; break;

    /* illegal characters */
    default:
      fprintf(stderr, "Illegal character in uint-word '%s': '%c'.\n",
              s_orig, c);
      exit(1);
      break;
    }

    if (value < 0) {
      fprintf(stderr, "This cannot happen.");
      exit(1);
    }
    if (value >= base) {
      fprintf(stderr,
              "Numbers of base %d may not contain the digit '%c': '%s'.\n",
              base, c, s_orig);
      exit(1);
    }

    ret *= base;
    ret += value;
  }

  return ret;
}

static void main_convertNumber(int base, char const *s) {
  /* Length of a 32-bit quint word, without trailing NUL:
     two quints plus a separator. */
  int const QUINT_LEN = 5*2 + 1;
  /* Double length plus another separator in case we switch to
     64-bits. */
  int const DOUBLE_QUINT_LEN = 2*QUINT_LEN + 1;

  int const n = my_atoi(base, s);
  char quint[DOUBLE_QUINT_LEN+1];
  int i;

  for(i=0; i<DOUBLE_QUINT_LEN+1; ++i) quint[i] = '\0';
  uint2quint(quint, n, '-');

  /* fprintf(stderr, "uint %s -> quint %s\n", s, quint); */
  printf("%s ", quint);
}

static void main_convertQuint(char const *s) {
  uint32_t uint0 = quint2uint(s);
  /* fprintf(stderr, "quint %s -> uint %u = x%x\n", s, uint0, uint0); */
  printf("x%x ", uint0);
}

/* Convert command-line strings.
 */
int main(int argc, char const **argv) {
  --argc; ++argv;
  for(; *argv; --argc, ++argv) {
    char const *s = *argv;
    if (!s[0]) continue;        /* this would be very strange */
    if (s[0] == '0') {
      fprintf(stderr,
              "Do not lead input strings with a zero.\n"
              "For decimal, trim leading zeros.\n"
              "For hex, lead with an 'x'.\n"
              "We do not process octal.\n");
      exit(1);
    } else if (s[0] == 'x' || s[0] == 'X') {
      /* a hexadecimal number */
      main_convertNumber(16, s+1);
    } else if (isdigit(s[0])) {
      /* a decimal number */
      main_convertNumber(10, s);
    } else {
      /* a quint word */
      main_convertQuint(s);
    }
  }

  printf("\n");
  return 0;
}
