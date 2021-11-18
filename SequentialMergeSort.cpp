#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
void merge(int*, int*, int, int, int);
void mergeSort(int*, int*, int, int);
int main(int argc, char** argv) {
	double mytime;
	
	int n = atoi(argv[1]);
	int* original_array = (int*)malloc(n * sizeof(int));
	int c;
	for (c = 0; c < n; c++) {
		original_array[c] = rand() % n;
	}
	
	int world_rank;
	int world_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	mytime = MPI_Wtime(); /*get time before merge-sort */
	
	if (world_rank == 0) {
		int* temp = (int*)malloc(n * sizeof(int));
		mergeSort(original_array, temp, 0, (n - 1));
		mytime = MPI_Wtime() - mytime; // time after completing merge-sort
		
		printf("Size of the list: %d", n);
		printf("\n Execution time: %.4f \n", mytime);
	
		free(temp);
	}

	free(original_array);
	MPI_Finalize();
}
/* Merge Function */
void merge(int* a, int* b, int start, int middle, int end)
{
	int na1, na2, nb, i;
	na1 = start;
	nb = start;
	na2 = middle + 1;
	while ((na1 <= middle) && (na2 <= end)) {
		if (a[na1] <= a[na2]) {
			b[nb++] = a[na1++];
		}
		else {
			b[nb++] = a[na2++];
		}
	}
	if (na1 <= middle) {
		for (i = na1; i <= middle; i++) {
			b[nb++] = a[i];
		}
	}
	if (na2 <= end) {
		for (i = na2; i <= end; i++) {
			b[nb++] = a[i];
		}
	}
	for (i = start; i <= end; i++) {
		a[i] = b[i];
	}
}
/*Recursive Merge Function*/
void mergeSort(int* a, int* b, int start, int end)
{
	int middle;

	if (start < end) {
		middle = (start + end) / 2;
		mergeSort(a, b, start, middle);
		mergeSort(a, b, middle + 1, end);
		merge(a, b, start, middle, end);
	}
}