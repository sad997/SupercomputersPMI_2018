#include "stdafx.h"
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	struct triple_struct {
		int p1, p2;
		double p3;
	} triple;

	int blocklen[3] = { 1, 1, 1 };
	MPI_Datatype constypes[3] = { MPI_INT, MPI_INT, MPI_DOUBLE };
	MPI_Aint displ[3];
	displ[0] = (MPI_Aint)0;
	MPI_Aint int_extent;
	MPI_Type_extent(MPI_INT, &int_extent);
	displ[1] = int_extent;
	displ[2] = int_extent + int_extent;

	MPI_Datatype triple_type;
	MPI_Type_create_struct(3, blocklen, displ, constypes, &triple_type);
	MPI_Type_commit(&triple_type);

	int i;
	triple_struct* buf = new triple_struct[rank];
	if (rank > 0) {
		for (i = 0; i < rank; i++) {
                        std::cin >> buf[i].p1 >> buf[i].p2 >> buf[i].p3;
		}
		MPI_Send(buf, rank, triple_type, 0, 0, MPI_COMM_WORLD);
	}
	else {
		triple_struct* recvbuf = new triple_struct[size];
		for (i = 1; i < size; i++) {
			MPI_Recv(recvbuf, i, triple_type, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for (int j = 0; j < i; j++) {
                                std::cout << recvbuf[j].p1 << recvbuf[j].p2 << recvbuf[j].p3;
			}
		}
	}



	delete buf;

        MPI_Finalize();
        return 0;
    }
