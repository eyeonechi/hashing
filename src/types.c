/*
 * COMP20007 Design of Algorithms
 * Semester 1 2016
 * Assignment 2 - Hashing
 *
 * Clement Poh (cpoh@unimelb.edu.au)
 * Ivan Ken Weng Chee (ichee@student.unimelb.edu.au) (736901)
 *
 * This module provides fundamental functionality used to
 * interpret generic pointers into typed data.
 *
*/
#include "types.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define NEWLINE  '\n'
#define NULLBYTE '\0'

/* Recursively called by str_copy() */
char *rec_str_copy(char *src, char *dest);

/* Returns whether n1 is equal to n2 */
bool int_eq(long e1, long e2) {
    return e1 == e2;
}

/* Prints n to file */
void int_print(FILE *file, long e) {
    fprintf(file, " %ld", e);
}

/* Returns whether str1 is equal to str2 */
bool str_eq(char *str1, char *str2) {
    return (!strcmp(str1, str2)) ? true : false;
}

/* Returns a copy of src */
char *str_copy(char *src) {
	char *dest = (char *)calloc(strlen(src), sizeof(dest));
	return rec_str_copy(src, dest);
}

/* Recursively called by str_copy() */
char *rec_str_copy(char *src, char *dest) {
	*dest = (*src == NEWLINE) ? NULLBYTE : *src;
	return (*src) ? rec_str_copy(src + 1, dest + 1) - 1 : dest;
}

/* Prints str to file */
void str_print(FILE *file, char *str) {
	fprintf(file, " %s", str);
}
