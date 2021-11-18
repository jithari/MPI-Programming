#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
void merge(int*, int*, int, int, int);
void mergeSort(int*, int*, int, int);
int main(int argc, char** argv) {
	double mytime;
	/*~~~~~~~~ Genratae the array ~~~~~~~~~~~``*/
	int n = atoi(argv[1]);
	int* original_array =(int*) malloc(n * sizeof(int));
	int c;
	srand(time(NULL));
	for (c = 0; c < n; c++) {
		original_array[c] = rand() % n;
	}
	
	int world_rank;
	int world_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	mytime = MPI_Wtime(); /*get time before merge-sort */
	
	int size = n / world_size;
	
	int* sub_array = (int*)malloc(size * sizeof(int));
	MPI_Scatter(original_array, size, MPI_INT, sub_array, size, MPI_INT, 0,
		MPI_COMM_WORLD);
	
	int* tmp_array = (int*)malloc(size * sizeof(int));
	mergeSort(sub_array, tmp_array, 0, (size - 1));
	
	int* sorted = NULL;
	if (world_rank == 0) {
		sorted = (int*)malloc(n * sizeof(int));
	}
	MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (world_rank == 0) {
		int* other_array = (int*)malloc(n * sizeof(int));
		mergeSort(sorted, other_array, 0, (n - 1));
		mytime = MPI_Wtime() - mytime; // time after completing merge-sort
		
		printf("Num of Processes: %d \n", world_size);
		printf("Size of the list: %d", n);
		/*for(c = 0; c < n; c++) {
		printf("%d ", sorted[c]);
		}*/
		printf("\n Execution time: %.2f \n", mytime);
		
		free(sorted);
		free(other_array);
	}
	
	free(original_array);
	free(sub_array);
	free(tmp_array);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}
/*~~~~~~~ Merge Function~~~~~~*/
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
/*~~~~~~~~~ Recursive Merge Function~~~~~~~~~*/
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
