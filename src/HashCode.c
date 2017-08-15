/*
 **************************************************************************
 *                                                                        *
 *          General Purpose Hash Function Algorithms Library              *
 *                                                                        *
 * Author: Arash Partow - 2002                                            *
 * URL: http://www.partow.net                                             *
 * URL: http://www.partow.net/programming/hashfunctions/index.html        *
 *                                                                        *
 * Copyright notice:                                                      *
 * Free use of the General Purpose Hash Function Algorithms Library is    *
 * permitted under the guidelines and in accordance with the most current *
 * version of the Common Public License.                                  *
 * http://www.opensource.org/licenses/cpl1.0.php                          *
 *                                                                        *
 **************************************************************************
*/

#include "HashCode.h"

/**
 * A simple hash function from Robert Sedgwicks Algorithms in C book.
 */
unsigned int RSHash(char *str, unsigned int len)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = hash * a + (*str);
        a = a * b;
    }

    return hash;
}

/**
 * A bitwise hash function written by Justin Sobel.
 */
unsigned int JSHash(char *str, unsigned int len)
{
    unsigned int hash = 1315423911;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }

    return hash;
}

/**
 * This hash algorithm is based on work by Peter J. Weinberger of AT&T Bell
 * Labs. The book Compilers (Principles, Techniques and Tools) by Aho, Sethi
 * and Ulman, recommends the use of hash functions that employ the hashing
 * methodology found in this particular algorithm.
 */
unsigned int PJWHash(char *str, unsigned int len)
{
    const unsigned int BitsInUnsignedInt =
        (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters =
        (unsigned int)((BitsInUnsignedInt * 3) / 4);
    const unsigned int OneEighth = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits =
        (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = (hash << OneEighth) + (*str);

        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

/**
 * Similar to the PJW Hash function, but tweaked for 32-bit processors.
 * Its the hash function widely used on most UNIX systems.
 */
unsigned int ELFHash(char *str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int x = 0;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = (hash << 4) + (*str);
        if ((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }

    return hash;
}

/**
 * This hash function comes from Brian Kernighan and Dennis Ritchie's book
 * "The C Programming Language". It is a simple hash function using a strange
 * set of possible seeds which all constitute a pattern of 31....31...31 etc,
 * it seems to be very similar to the DJB hash function.
 */
unsigned int BKDRHash(char *str, unsigned int len)
{
    unsigned int seed = 131;    /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = (hash * seed) + (*str);
    }

    return hash;
}

/**
 * This is the algorithm of choice which is used in the open source SDBM
 * project. The hash function seems to have a good over-all distribution for
 * many different data sets. It seems to work well in situations where there
 * is a high variance in the MSBs of the elements in a data set.
 */
unsigned int SDBMHash(char *str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

/**
 * An algorithm produced by Professor Daniel J. Bernstein and shown first to
 * the world on the usenet newsgroup comp.lang.c. It is one of the most
 * efficient hash functions ever published.
 */
unsigned int DJBHash(char *str, unsigned int len)
{
    unsigned int hash = 5381;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = ((hash << 5) + hash) + (*str);
    }

    return hash;
}

/**
 * An algorithm proposed by Donald E. Knuth in The Art Of Computer Programming
 * Volume 3, under the topic of sorting and search chapter 6.4.
 */
unsigned int DEKHash(char *str, unsigned int len)
{
    unsigned int hash = len;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }
    return hash;
}

/**
 * A simple hash function taken from Bruno Preiss's "Data Structures and
 * Algorithms with Object-Oriented Design Patterns in Java",
 * John Wiley & Sons, 2000
 */
unsigned int BPHash(char *str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int i = 0;
    for (i = 0; i < len; str++, i++) {
        hash = hash << 7 ^ (*str);
    }

    return hash;
}

/**
 * Fowler-Noll-Vo is a non-cryptographic hash function created by Glenn Fowler,
 * Landon Curt Noll, and Kiem-Phong Vo.
 */
unsigned int FNVHash(char *str, unsigned int len)
{
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash = 0;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash *= fnv_prime;
        hash ^= (*str);
    }

    return hash;
}

/**
 * An algorithm produced by Arash Partow.
 */
unsigned int APHash(char *str, unsigned int len)
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i = 0;

    for (i = 0; i < len; str++, i++) {
        hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
            (~((hash << 11) + ((*str) ^ (hash >> 5))));
    }

    return hash;
}
