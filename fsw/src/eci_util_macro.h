/** \file
 * Defines utility macros which are used throughout the code.
 */
 
#ifndef ECI_UTIL_MACRO_H
#define ECI_UTIL_MACRO_H

/**** Macros used in various places. Need a better place for it. ****/

/* Converts Define or Enum to string. */
#define D2STR(x) #x

/* Returns the number of elements in the array. */
#define SIZEOF_ARRAY(ar) (sizeof(ar)/sizeof(ar[0]))

/* Returns the number of bytes for the member of the structure. */
#define SIZEOF_MEMBER(memberOf, member) sizeof(((memberOf*)0)->member)

/* Returns the number of elements of an array for a member of a structure. */
#define SIZEOF_ARRAY_MEMBER(memberOf, member) (sizeof(((memberOf*)0)->member)/sizeof(((memberOf*)0)->member[0]))

/*
 * Below are byte swapping macros that are as efficient as using an in-line
 * Assembly function.
 */
#define BSWAP_16(x)  (((uint16_t)(x) << 8) | \
                      ((uint16_t)(x) >> 8))

#define BSWAP_32(x)  (((uint32_t)(x) << 24) | \
                     (((uint32_t)(x) <<  8) & 0xff0000) | \
                     (((uint32_t)(x) >>  8) & 0xff00) | \
                      ((uint32_t)(x) >> 24))

#define BSWAP_64(x)  (((uint64_t)(x) << 56) | \
                     (((uint64_t)(x) << 40) & 0xff000000000000ULL) | \
                     (((uint64_t)(x) << 24) & 0xff0000000000ULL) | \
                     (((uint64_t)(x) <<  8) & 0xff00000000ULL) | \
                     (((uint64_t)(x) >>  8) & 0xff000000ULL) | \
                     (((uint64_t)(x) >> 24) & 0xff0000ULL) | \
                     (((uint64_t)(x) >> 40) & 0xff00ULL) | \
                      ((uint64_t)(x) >> 56))

/*
 * Binary Constant Generator Macros
 *   By Tom Torfs - donated to the public domain
 *
 * All macro's evaluate to compile-time constants.
 */

/*
 * Turn a numeric literal into a hex constant. This avoids problems with
 * leading zeroes. 8-bit constants have a max value of 0x11111111, which always
 * fits in an unsigned long
 */
#define HEX__(n) 0x##n##LU

/* 8-bit Conversion Function */
#define B8__(x)   ((x&0x0000000FLU)?1:0) \
                + ((x&0x000000F0LU)?2:0) \
                + ((x&0x00000F00LU)?4:0) \
                + ((x&0x0000F000LU)?8:0) \
                + ((x&0x000F0000LU)?16:0) \
                + ((x&0x00F00000LU)?32:0) \
                + ((x&0x0F000000LU)?64:0) \
                + ((x&0xF0000000LU)?128:0)

/*** USER MACROS ***/

/* For up to 8-bit binary constants. */
#define B8(d) ((uint8_t)B8__(HEX__(d)))

/* For up to 16-bit binary constants, MSB first. */
#define B16(dmsb, dlsb) (((uint16_t)B8(dmsb)<< \
                       + B8(dlsb))

/* For up to 32-bit binary constants, MSB first. */
#define B32(dmsb, db2, db3, dlsb) (((uint32_t)B8(dmsb) << 24) \
                                 + ((uint32_t)B8(db2) << 16) \
                                 + ((uint32_t)B8(db3) << \
                                 + B8(dlsb))

/*
 * Sample usage:
 * B8(01010101) = 85 = 0x55 = 'U'
 * B16(10101010, 01010101) = 43605 = 0xAA55
 * B32(10000000, 11111111, 10101010, 01010101) = 2164238933 = 0x80FFAA55
 */

#endif /* ECI_UTIL_MACRO_H */

/************************/
/*  End of File Comment */
/************************/
