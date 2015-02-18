/* Author: Parker Mathewson
 * Date: 2/14/15
 * SID: 23118888
 * File: sortProcess.c
 *
 * Purpose: This file is to implement quicksort using processes
 *            with using strcmp() as the comparator for sorting.
 *            This file will sort an input from the command line (a file)
 *            that has at maximum of 500,000 lines and will sort each line
 *            and output the sorted to stdout without adding anything or missing
 *            anything in the sorting algorithm. The program will time how long
 *            the sorting took by using gettimeofday().
 *
 *
 * INPUT: Number of processes, must be power of 2, max of 16
 *        a file to be sorted, 'any' format
 *
 * OUTPUT: a sorted list to std containing the same contents as the input file
 *         time it took to sort
 *
 * DEPENDENCIES: sortProcess.h
 *               stdio.h
 *               stdlib.h
 *               time.h
 *               sys/time.h
 */

#include "sortProcess.h"

int debug = 0;

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
	printf("in quicksort\n");
	if(low < high)
	{
		printf("in low < high quicksort\n");
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
	int count = 0, help = 0, setter = 0;
	int* nums;
	struct timeval tv, end;
	char** memoryBuffer;
	char** sections[16];

	FILESIZE = MAXCHARS * sizeof(char) * MAXLINES;

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

	PROCESSES = atoi(argv[1]);
	inputFile = fopen(argv[2], "r");

	for(int agh = 0; agh < 16; agh++)
		sections[agh] = malloc(sizeof(char*)*FILESIZE);

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

	int count_help = 0;
	int count_help2 = 0;
	int loo = 0;
	nums = malloc(sizeof(int) * PROCESSES);

	for(int j = 0; j < PROCESSES; j++)
	{
		if(count%PROCESSES > j || j == 0)
		{
			if(debug)
				printf("bucket %d: %d to %d\n", j, count_help, count_help + (count/PROCESSES));
			count_help = (count/PROCESSES) + 1;
			for( loo = 0; loo < count_help; loo++)
			{
				if(debug)
					printf("if sections[%d][%d] = lines[%d]\n", j, loo, count_help2);
				sections[j][loo] = lines[count_help2];
				count_help2++;
			}
		}
		else
		{
			if(debug)
				printf("bucket %d: %d to %d\n", j, count_help, count_help + (count/PROCESSES)-1);
			count_help = (count/PROCESSES);
			for( loo = 0; loo < count_help; loo++)
                        {
				if(debug)
                                	printf("else sections[%d][%d] = lines[%d]\n", j, loo, count_help2);
                              	sections[j][loo] = lines[count_help2];
				count_help2++;
                        }
		}
		nums[j] = loo;
		if(debug)
			printf("nums[%d] = %d\n", j, loo);
	}
	fclose(inputFile);

	if(debug)
		printf("%d %d\n", (int)getpid()%PROCESSES, (int)getpid());
	pid_t kidpid;
	int poop = 0;
	quicksort(sections[(int)getpid()%PROCESSES], 0, nums[(int)getpid()%PROCESSES]);

	for(int j = 0; j < 4; j++)
	{
		kidpid = fork();
		if(kidpid < 0)
		{
			printf("fork failed!\n");
			exit(1);
		}
		if(kidpid == 0)
		{
			if(debug)
			{
				printf(" %d %d \n", (int)getpid()%PROCESSES, (int)getpid());
				printf(" sections[%d][0] = %s  nums[%d] = %d\n", (int)getpid()%PROCESSES, sections[(int)getpid()%PROCESSES][0], (int)getpid()%PROCESSES, nums[(int)getpid()%PROCESSES]);
			}
			printf("quicksort start\n");
			quicksort(sections[(int)getpid()%PROCESSES], 0, nums[(int)getpid()%PROCESSES]);
			printf("quicksort end\n");
		}
		else
		{
			;
		}
	}
/*
	gettimeofday(&tv, NULL);

	quicksort(lines, 0, count-1);

	gettimeofday(&end, NULL);	

	fprintf(stderr, "TIME TO SORT: %ld microseconds\n", end.tv_usec - tv.tv_usec);

	print(lines, 0, count);
*/

	print(sections[(int)getpid()%PROCESSES], 0, nums[(int)getpid()%PROCESSES]);
	return 0;
}
