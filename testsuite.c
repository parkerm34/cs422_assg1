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
 * OUTPUT: a sorted file containing the same contents as the input file
 *
 * DEPENDENCIES: stdio.h
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

FILE *inputFile, *ansFile;

void usage()
{
	printf("\nTESTSUITE USAGE:\n\n");
	printf("testsuit <numTestCases> <program>\n\n");
	printf("numTestCases is the number of test cases to run in the tests directory starting from 1.\n");
	printf("program is the exact program you want to run the test cases against. This testsuite\n");
	printf("was made for sortSeq, sortProccess, and sortThread, but will work on others given there\n");
	printf("is a tests directory within the containing directory of this file as well as the program.\n\n\n");
}

int main( int argc, char *argv[] )
{
	char command[50], run[50];
	int help = 0;
	if(argv[1] == NULL || argv[2] == NULL)
        {
                usage();
                exit(1);
        }

	char* num = argv[1];
	char* app = argv[2];
	int cases = atoi(num);
	int count = 1, passed = 0, failed = 0;


	while(count <= cases)
	{
		if(strcmp(app, "sortSeq") == 0)
			sprintf(run, "%s tests/test%d.txt > tests/test%d.out", app, count, count);
		else
			sprintf(run, "%s 16 tests/test%d.txt > tests/test%d.out", app, count, count);
		system(run);
		printf("\n----**** TEST CASE %d DIFF RESULTS ****----\n", count);
		sprintf(command, "diff tests/test%d.out tests/test%d.ans", count, count);
		help = system(command);
		if( help == 0 )
		{
			printf("\n++++++++ TEST CASE %d PASSED! ++++++++\n", count);
			passed++;
		}
		else
		{
			printf("\nXXXXXXXX TEST CASE %d FAILED! XXXXXXXX\n", count);
			failed++;
		}
		count++;
	}
	printf("\n======== RESULTS ========\n");
	printf("\tFAILED: %d\n", failed);
	printf("\tPASSED: %d\n\n", passed);
	return 0;
}
