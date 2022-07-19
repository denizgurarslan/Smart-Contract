#ifndef SHA3_H
#define SHA3_h

#define SHA3_L 6
#define SHA3_NR (12 + 2 * SHA3_L)
#define SHA3_B (25 << SHA3_L)

#define SHA3_128 128
#define SHA3_256 256
#define SHA3_512 512

#define false 0
#define true !false
/**
 * calculates the number of arguments in a __VA_ARGS__ list
 * @param type the type of the argument in the list
 * @param ... the list
 * @return the number of arguments in the list
 */
#define NUMARGS(type, ...) (sizeof((type[]){0.0f, ##__VA_ARGS__}) / sizeof(type) - 1)

/**
 * finds the maximum of two values
 * @param i1 the first value
 * @param i2 the second value
 * @return the larger element
 */
#define MAX(i1, i2) (i1 > i2 ? i1 : i2)
/**
 * finds the minimum of two values
 * @param i1 the first value
 * @param i2 the second value
 * @return the larger element
 */
#define MIN(i1, i2) (i1 < i2 ? i1 : i2)
//Function declarations
extern unsigned int sha3_rotConst[5][5];
extern unsigned long long sha3_roundConsts[SHA3_NR];

unsigned long long sha3_rotWord(unsigned long long w,unsigned int d);

void sha3_keccak_f(unsigned long long A[5][5]);
int sha3_hash(unsigned char *in,int n,int mode, unsigned char **out);

#endif // SHA3_H
