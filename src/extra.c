/*
 * COMP20007 Design of Algorithms
 * Semester 1 2016
 * Assignment 2 - Hashing
 *
 * Ivan Ken Weng Chee (ichee@student.unimelb.edu.au) (736901)
 *
 * Contains hashing-related algorithms and hash collision generators.
 *
*/
#include "extra.h"
#include "hash.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_PRIME  1
#define NOT_PRIME 0

#define NEWLINE  "\n"
#define NULLBYTE '\0'

#define MAXSTRLEN 256
#define LCHAR      33
#define HCHAR     126
#define N           2

typedef struct euclid_s euclid_t;

struct euclid_s {
	int x, y, d;
};

/* Prints n to file */
void int_print(FILE *file, long n);

/* Prints str to file */
void str_print(FILE *file, char *str);

/* Checks whether an integer is a prime number */
int check_prime(int n);

/* Recursively called by check_prime() */
int rec_check_prime(int n, int div);

/* Checks for the next prime after n */
int next_prime(int n);

/* Recursively called by collide_dumb() */
unsigned char **rec_collide_dumb(unsigned char **S, unsigned int size, int n);

/* Recursively prints zero-hashed strings */
void rec_print_string(unsigned char **strings, unsigned int n);

/* Genrates a random string */
unsigned char *random_string(unsigned int size);

/* Recursively called by random_string() */
unsigned char *rec_random_string(unsigned char *randstr, int n);

/* Extended Euclid algorithm */
euclid_t extended_euclid(int a, int b);

/* Determine appropriate size of a hash table given input size n */
unsigned int determine_size(unsigned int n) {
	return next_prime(2 * n);
}

/* Checks whether an integer is a prime number */
int check_prime(int n) {
	return (n < 2)
		? NOT_PRIME
		: rec_check_prime(n, 2);
}

/* Recursively called by check_prime() */
int rec_check_prime(int n, int div) {
	return (div * div <= n)
		? (n % div)
			? rec_check_prime(n, div + 1)
			: NOT_PRIME                                                                                                                
		: IS_PRIME;
}

/* Checks for the next prime after n */
int next_prime(int n) {
	return check_prime(n + 1)
		? n + 1
		: next_prime(n + 1);
}

/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_dumb(unsigned int size, unsigned int seed, int n) {
	srand(seed);
	unsigned char **strings = (unsigned char **)malloc(n * sizeof(strings));
	assert(strings);
	rec_print_string(rec_collide_dumb(strings, size, n), n);
}

/* Recursively called by collide_dumb() */
unsigned char **rec_collide_dumb(unsigned char **S, unsigned int size, int n) {
	return (n)
		? (!universal_hash(*S = random_string(size), size))
			? rec_collide_dumb(S + 1, size, n - 1) - 1
			: rec_collide_dumb(S, size, n)
		: S;
}

/* Recursively prints zero-hashed strings */
void rec_print_string(unsigned char **strings, unsigned int n) {
	if (n) {
		str_print(stdout, (char *)*strings);
		fprintf(stdout, NEWLINE);
		rec_print_string(strings + 1, n - 1);
	}
}

/* Genrates a random string */
unsigned char *random_string(unsigned int size) {
	unsigned int n = 1 + rand() % (size - 1);
	unsigned char *randstr = (unsigned char *)malloc(n * sizeof(randstr));
	assert(randstr);
	return rec_random_string(randstr, n);
}

/* Recursively called by random_string() */
unsigned char *rec_random_string(unsigned char *randstr, int n) {
	*randstr = n ? (unsigned char)((rand() % (HCHAR - LCHAR)) + LCHAR) : '\0';
	return n
		? rec_random_string(randstr + 1, n - 1) - 1
		: randstr;
}

/* Print n strings that are hashed to 0 by universal_hash seeded with seed */
void collide_clever(unsigned int size, unsigned int seed, int n) {
	srand(seed);
	unsigned int i, r0 = rand() % size, r1 = rand() % size, s0;
	int x1, y1, x2, y2, u, v, K1[size + 1], K2[size + 1], flag = 1;
	for (i = 0; i <= size; i ++) {
		K1[i] = 0;
		K2[i] = 0;
	}
	euclid_t euclid, euclid2;
	unsigned char clash[3] = {0};
	int_print(stdout, (long)N);
	str_print(stdout, NEWLINE);
	while (n) {
		/* Check if r0 is coprime with size */
		euclid = extended_euclid(r0, size);
		x1 = euclid.x;
		y1 = euclid.y;
		if (euclid.d == 1) {
			/* Creating strings of one character length */
			if (flag == 1) {
				for (i = 1; i <= size; i ++) {
					if (K1[i]) {
						continue;
					}
					/* Dealing with negative coefficients */
					if (x1 < 0) {
						s0 = x1 * -1 * i;
					} else {
						s0 = x1 * i;
					}
					/* Checks if universal_hash would hash string to 0 */
					if ((r0 * s0) % size == 0) {
						clash[0] = (unsigned char)(s0);
						str_print(stdout, (char *)clash);
						str_print(stdout, NEWLINE);
						n --;
						K1[i] = 1;
						flag ++;
						if (!n) {
							return;
						}
					}
				}
			}
			
			/* Check if r1 is coprime with size */
			euclid2 = extended_euclid(r1, size);
			x2 = euclid2.x;
			y2 = euclid2.y;
			if (euclid2.d == 1) {
				if (flag == 2) {
					for (i = 1; i <= size; i ++) {
						if (K2[i]) {
							continue;
						}
						/* Generating coefficients to solve diophantine eqn */
						/* u(x2 - x1) + v(y2 - y1) = 0                      */
						v = i * (x2 - x1);
						u = i * (y1 - y2);
						/* Checks if universal_hash would hash string to 0 */
						if ((r0 * ((u * (x2 - x1)) % size)
							+ r1 * ((v * (y2 - y1)) % size)) % size == 0) {
							clash[0] = (unsigned char)((u * (x2 - x1)) % size);
							clash[1] = (unsigned char)((v * (y2 - y1)) % size);
							str_print(stdout, (char *)clash);
							str_print(stdout, NEWLINE);
							n --;
							K2[i] = 1;
							flag ++;
							if (!n) {
								return;
							}
						}
					}
					str_print(stderr, "n too large, failing gracefully\n");
					break;
				}
			}
		}
	}
}

/* Extended Euclid algorithm */
euclid_t extended_euclid(int a, int b) {      
	euclid_t euclid, euclid2;
	if (b == 0) {
		euclid.x = 1;
		euclid.y = 0;
		euclid.d = a;
		return euclid;
	}
	euclid = extended_euclid(b, a % b);
	euclid2.x = euclid.y;
	euclid2.y = euclid.x - (a / b) * euclid.y;
	euclid2.d = euclid.d;
	return euclid2;
}
