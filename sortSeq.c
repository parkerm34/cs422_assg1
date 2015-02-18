/* Author: Parker Mathewson
 * Date: 2/14/15
 * SID: 23118888
 * File: sortSeq.c
 *
 * Purpose: This file is to implement quicksort in sequential programming
 *            with using strcmp() as the comparator for sorting.
 *            This file will sort an input from the command line (a file)
 *            that has at maximum of 500,000 lines and will sort each line
 *            and output a sorted file without adding anything or missing
 *            anything in the sorting algorithm.
 *
 *
 * INPUT: a file to be sorted, 'any' format
 *
 * OUTPUT: a sorted list to stdout containing the same contents as the input file
 *
 * DEPENDENCIES: sortSeq.h
 *               stdio.h
 *               stdlib.h
 *               time.h
 *               sys/time.h
 */

#include "sortSeq.h"

void usage()
{
        printf("\nSORTSEQ USAGE:\n\n");
        printf("sortSeq <fileName>\n\n");
        printf("fileName is the exact file you want to sort.\n");
        printf("This sortSeq will sort the input and print them to stdout and print the time\n");
        printf("it took to sort to stderr.\n");
}


void print( char* lines[], int low, int high)
{
	int i = low;
	for(;i < high; i++)
		printf("%s", lines[i]);
}

void swap( char* lines[], int x, int y )
{
	char* temp = lines[x];
	lines[x] = lines[y];
	lines[y] = temp;
}


void exchange(char* lines[], int low, int high)
{
	char* temp = lines[low];
	lines[low] = lines[high];
	lines[high] = temp;
}

int partition(char* lines[], int low, int high)
{
	char* temp = lines[high];
	int i = low - 1;
	for(int j = low; j < high; j++)
	{
		if(strcmp(lines[j], temp) <= 0)
		{
			i++;
			exchange(lines, i, j);
		}
	}
	exchange(lines, i + 1, high);
	return i + 1;
}

char* median_of_3(char* lines[], int low, int high)
{
	int median = (low + high) / 2;

	if(strcmp(lines[low], lines[high]) > 0)
		exchange(lines, low, high);
	if(strcmp(lines[low], lines[median]) > 0)
		exchange(lines, low, median);
	if(strcmp(lines[high], lines[median]) > 0)
		exchange(lines, high, median);

	return lines[high];
}

void quicksort(char* lines[], int low, int high)
{
	if(low < high)
	{
		median_of_3(lines, low, high);

		int q = partition(lines, low, high);
		quicksort(lines, low, q - 1);
		quicksort(lines, q + 1, high);
	}
}

int main( int argc, char *argv[] )
{
	char *lines[MAXLINES];
	char *fileLine = malloc(sizeof(char)*MAXCHARS);
	int count = 0;
	struct timeval tv, end;

	if(argv[1] == NULL)
	{
		usage();
		exit(1);
	}
	inputFile = fopen(argv[1], "r");

	while( fgets(fileLine, MAXCHARS, inputFile) != NULL)
	{
		lines[count] = fileLine;
		count++;
		fileLine = malloc(sizeof(char)*MAXCHARS);
	}

	fclose(inputFile);

	gettimeofday(&tv, NULL);

	quicksort(lines, 0, count-1);

	gettimeofday(&end, NULL);	

	fprintf(stderr, "TIME TO SORT: %ld seconds %ld microseconds\n", end.tv_sec - tv.tv_sec, end.tv_usec - tv.tv_usec);

	print(lines, 0, count);

	return 0;
}
