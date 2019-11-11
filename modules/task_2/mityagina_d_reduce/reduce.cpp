// Copyright 2019 Mityagina Daria
#include "../../../modules/task_2/mityagina_d_reduce/reduce.h"
#include <math.h>
#include <ctime>
#include <random>
#include <algorithm>
#include <stdexcept>

void help_me_reduce(
  const int rank_in, 
  const int root,
  const int np, 
  void *sendbuf, 
  void *recvbuf, 
  int cnt, 
  MPI_Datatype datatype, 
  union_func op, // возможно стоит заменить на строку, и в зависимости от значения 
                 // (MPI_SUM, MPI_MAX, ..) производить к-л дейтвия с пришедшими данными
  MPI_Comm comm) 
{
  int rank = rank_in;
  MPI_Status status;
  if (np > 1) {
    if (rank != root)
    {
      MPI_Send(sendbuf, cnt, datatype, root, 0, comm);
    }
    else
    {
      MPI_Recv(recvbuf, cnt, datatype, MPI_ANY_SOURCE, 0, comm, &status);
      op(recvbuf, sendbuf, cnt);
    }
  }
  if(rank == root) {
    int typesize;
    MPI_Type_size(datatype, &typesize);
    memcpy(recvbuf, sendbuf, cnt * typesize);
  }
}


void Reduce(
	void *where_to_send_from,
	void *where_to_send_to,
	int how_much,
	MPI_Datatype datatype,
	union_func op,
	int root,
	MPI_Comm comm
	) 
{
  int rank, np; 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  help_me_reduce(rank, root, np, where_to_send_from, where_to_send_to, how_much, datatype, op, comm);
}
