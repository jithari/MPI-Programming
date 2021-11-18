#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#define N 3
int main(int argc, char** argv) {
	int i, j, k;
	int tag = 99, rank, size, root,blksz,sum=0;
	int A[N][N] = {
		{2,6, 5},
		{1,8, 7},
		{6,2, 1}
	};
	int B[N][N] = {
		{2,1, 6},
		{1,2, 7},
		{3,1, 2}
	};
	int C[N][N];
	int AA[N], CC[N];
	
    MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	MPI_Scatter(A, N * N / size, MPI_INT, AA, N * N / size, MPI_INT,0, MPI_COMM_WORLD);
	MPI_Bcast(B, N*N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
				sum = sum+  AA[j] * B[j][i];
		 
	    }  //end for j
		CC[i] = sum;
		sum = 0;
    }
	MPI_Gather(CC, N * N / size, MPI_INT, C, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		printf("\n~^~^~^~^~Matrix A~^~^~^~^~\n");

		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				printf("%10d", A[i][j]);
			}
			printf("\n");
		}
		
		printf("\n~^~^~^~^~Matrix b~^~^~^~^~\n");

		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				printf("%10d", B[i][j]);
			}
			printf("\n");
		}

		printf("\n~^~^~^~^~Matrix C~^~^~^~^~\n");

		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				printf("%10d", C[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Finalize();
}