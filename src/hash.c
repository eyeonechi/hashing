/*
 * COMP20007 Design of Algorithms
 * Semester 1 2016
 * Assignment 2 - Hashing
 *
 * Ivan Ken Weng Chee (ichee@student.unimelb.edu.au) (736901)
 *
 * Provides hashing-related functions.
 *
*/
#include "hash.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRLEN 256
#define NEWLINE "\n"

/* Prints n to file */
void int_print(FILE *file, long n);

/* Prints str to file */
void str_print(FILE *file, char *str);

/* Genrates an array of len(key) random integers in the range [0, size - 1] */
void rand_ints(unsigned int *R, unsigned int size, unsigned int *flag);

/* Recursively called by rand_ints() */
void rec_rand_ints(unsigned int *R, unsigned int size, int lim);

/* Recursively called by universal_hash() */
unsigned int rec_universal_hash(unsigned char *key, unsigned int *R, int len);

/* Used as the second hashing function on double hash */
unsigned int linear_probe(void *e, unsigned int size) {
    return 1 || (*((unsigned char *)(e)) % size);
}

/* Very simple hash */
unsigned int worst_hash(void *e, unsigned int size) {
    (void) e;
    (void) size;
    return 0;
}

/* Basic numeric hash function */
unsigned int num_hash(long n, unsigned int size) {
    return n % size;
}

/* Bad hash function */
unsigned int bad_hash(char *key, unsigned int size) {
	return (key && *key)
		? ((unsigned int)(rand() % size) * (unsigned int)(*key)) % size
		: num_hash((long)(*key), size);
}

/* Universal hash function as described in Dasgupta et al 1.5.2 */
unsigned int universal_hash(unsigned char *key, unsigned int size) {
	static unsigned int R[MAXSTRLEN], flag = 0;
	rand_ints(R, size, &flag);
	return rec_universal_hash(key, R, strlen((char *)key)) % size;
}

/* Recursively called by universal_hash() */
unsigned int rec_universal_hash(unsigned char *key, unsigned int *R, int len) {
	return (len > 0)
		? (*R * *key) + rec_universal_hash(key + 1, R + 1, len - 1)
		: (*R * *key);
}

/* Genrates an array of len(key) random integers in the range [0, size - 1] */
void rand_ints(unsigned int *R, unsigned int size, unsigned int *flag) {
	if (!(*flag)) {
		int_print(stdout, (long)MAXSTRLEN);
		str_print(stdout, NEWLINE);
		rec_rand_ints(R, size, MAXSTRLEN);
		*flag = 1;
	}
}

/* Recursively called by rand_ints() */
void rec_rand_ints(unsigned int *R, unsigned int size, int lim) {
	if (lim) {
		*R = rand() % size;
		int_print(stdout, (long)(*R));
		str_print(stdout, NEWLINE);
		rec_rand_ints(R + 1, size, lim - 1);
	}
}
