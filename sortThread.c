/* Author: Parker Mathewson
 * Date: 2/14/15
 * SID: 23118888
 * File: sortThread.c
 *
 * Purpose: This file is to implement quicksort using threads
 *            with using strcmp() as the comparator for sorting.
 *            This file will sort an input from the command line (a file)
 *            that has at maximum of 500,000 lines and will sort each line
 *            and output the sorted to stdout without adding anything or missing
 *            anything in the sorting algorithm. The program will time how long
 *            the sorting took by using gettimeofday().
 *
 *
 * INPUT: Number of threads, must be power of 2, max of 16
 *        a file to be sorted, 'any' format
 *
 * OUTPUT: a sorted list to std containing the same contents as the input file
 *         time it took to sort
 *
 * DEPENDENCIES: sortThread.h
 *               stdio.h
 *               stdlib.h
 *               time.h
 *               sys/time.h
 */

#include "sortThread.h"

void swap( char* lines[], int x, int y )
{
        char* temp = lines[x];
        lines[x] = lines[y];
        lines[y] = temp;
}

void usage()
{
        printf("\nSORTPROCESS USAGE:\n\n");
        printf("sortProcess <numProcesses> <fileName>\n\n");
        printf("numProcesses is the number of processes to use at most, must be power of 2.\n");
        printf("fileName is the exact file you want to sort.\n");
        printf("This sortProcess will sort the input and print them to stdout and print the time\n");
        printf("it took to sort to stderr.\n");
}

void print( char* lines[], int low, int high)
{
        int i = low;
        for(;i < high; i++)
                printf("%s", lines[i]);
}


int partition(char* lines[], int low, int high, int pivot)
{
    char* pivotValue = lines[pivot];
    swap(lines, pivot, high); 
    int storeIndex = low;
    for (int i=low ; i<high ; i++)
    {
        if (strcmp(lines[i], pivotValue) < 0)
        {
            swap(lines, i, storeIndex); 
            storeIndex++;
        }
    }
    swap(lines, storeIndex, high); 
    return storeIndex;
}

char* median_of_3(char* lines[], int low, int high)
{
        int median = (low + high) / 2;

        if(strcmp(lines[low], lines[high]) > 0)
                swap(lines, low, high);
        if(strcmp(lines[low], lines[median]) > 0)
                swap(lines, low, median);
        if(strcmp(lines[high], lines[median]) > 0)
                swap(lines, high, median);

        return lines[high];
}


void quicksort(char* lines[], int low, int high)
{
     if (low < high)
     {
	median_of_3(lines, low, high);

        int piv = low + (high - low)/2;
        piv = partition(lines, low, high, piv);
        quicksort(lines, low, piv - 1);
        quicksort(lines, piv + 1, high);
     }
}

void* quicksort_thread(void *init)
{
    struct qsort_starter *start = init;
    parallel_quicksort(start->lines, start->low, start->high, start->depth);
    return NULL;
}

void parallel_quicksort(char* lines[], int low, int high, int depth)
{
    if (high > low)
    {
        int piv = low + (high - low)/2;
        piv = partition(lines, low, high, piv);
        if (depth-- > 0)
        {
            struct qsort_starter arg = {lines, low, piv - 1, depth};
            pthread_t thread;
            int ret = pthread_create(&thread, NULL, quicksort_thread, &arg);
            parallel_quicksort(lines, piv + 1, high, depth);
            pthread_join(thread, NULL);
        }
        else
        {
            quicksort(lines, low, piv - 1);
            quicksort(lines, piv + 1, high);
        }
    }
}

int main(int argc, char **argv)
{
	int count = 0, depth = 0;
	char** lines;
	char *fileLine = malloc(sizeof(char)*MAXCHARS);
	char** memoryBuffer;
	struct timeval tv, end;

	FILESIZE = MAXCHARS * sizeof(char) * MAXLINES;

	lines = malloc(sizeof(char)*FILESIZE);

	memoryBuffer = (char**)mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

	if( memoryBuffer == MAP_FAILED )
        {
                fprintf(stderr, "errno is %d\n", errno);
                perror("memoryBuffer is MAP_FAILED ");
                exit(1);
        }

	if(argv[1] == NULL || argv[2] == NULL)
        {
                usage();
                exit(1);
        }

	depth = atoi(argv[1]);
        inputFile = fopen(argv[2], "r");

	if(depth == 16)
		depth = 4;
	else if(depth == 8)
		depth = 3;
	else if(depth == 4)
		depth = 2;
	else if(depth == 2)
		depth = 2;

	if(inputFile == NULL)
        {
                printf("input file could not be opened\n");
                exit(1);
        }

	while( fgets(fileLine, MAXCHARS, inputFile) != NULL)
        {
                lines[count] = fileLine;
                memoryBuffer[count] = fileLine;
                count++;
                fileLine = malloc(sizeof(char)*MAXCHARS);
        }

	fclose(inputFile);

	gettimeofday(&tv, NULL);
	parallel_quicksort(lines, 0, count - 1, depth);
	gettimeofday(&end, NULL);

	fprintf(stderr, "TIME TO SORT: %ld seconds %ld microseconds\n", end.tv_sec - tv.tv_sec, end.tv_usec - tv.tv_usec);
	print(lines, 0, count);

	return 0;
}
