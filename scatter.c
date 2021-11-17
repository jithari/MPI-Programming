#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv)
 {
 int buff[100];
 int recvbuff[2];
 int numprocs;
 int myid;
 int i, k;
 int mysum;

 MPI_Status stat;

 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
 MPI_Comm_rank(MPI_COMM_WORLD,&myid);

 if ( myid == 0 )
 {
 cout << "WE have " << numprocs << " processors" << endl;
// -----------------------------------------------
 // Node 0 prepare 2 number for each processor
 // [1][2] [3][4] [5][6] .... etc
 // -----------------------------------------------
 k = 1;
 for ( i = 0; i < 2*numprocs; i += 2 )
 {
 buff[i] = k++;
buff[i+1] = k++;
 }
 }

 // ------------------------------------------
 // Node 0 scatter the array to the processors:
 // ------------------------------------------

 MPI_Scatter (buff, 2, MPI_INT, recvbuff, 2, MPI_INT, 0, MPI_COMM_WORLD);
 ^^^ !!! ^^^ !!!

 if ( myid == 0 )
 {
 mysum = recvbuff[0] + recvbuff[1];
 cout << "Processor " << myid << ": sum = " << mysum << endl;

 for( i = 1; i < numprocs; i++)
 {
 MPI_Recv(&mysum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
 cout << "Processor " << i << ": sum = " << mysum << endl;
 }
 }
 else
 {
 mysum = recvbuff[0] + recvbuff[1];
 MPI_Send(&mysum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
 }

 MPI_Finalize();
}