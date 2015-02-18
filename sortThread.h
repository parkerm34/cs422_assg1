/* Author: Parker Mathewson
 * Date: 2/14/15
 * SID: 23118888
 * File: sortSeq.h
 *
 * Purpose: This file is the header file to implement quicksort in sequential programming
 *            with using strcmp() as the comparator for sorting.
 *            This file will sort an input from the command line (a file)
 *            that has at maximum of 500,000 lines and will sort each line
 *            and output a sorted file without adding anything or missing
 *            anything in the sorting algorithm.
 *
 *
 *
 * DEPENDENCIES: stdio.h
 *               stdlib.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>

FILE *inputFile;
int PROCESSES;
int FILESIZE;
int MAXLINES = 500000;
int MAXCHARS = 200;

struct qsort_starter
{
    char** lines;
    int low;
    int high;
    int depth;
};

void swap( char* lines[], int x, int y );

void usage();

void print( char* lines[], int low, int high);

int partition(char* lines[], int low, int high, int pivot);

char* median_of_3(char* lines[], int low, int high);

void quicksort(char* lines[], int low, int high);

void parallel_quicksort(char* lines[], int low, int high, int depth);

void* quicksort_thread(void *init);


