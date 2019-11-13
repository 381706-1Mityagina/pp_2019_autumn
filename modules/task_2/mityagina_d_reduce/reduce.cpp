// Copyright 2019 Mityagina Daria
#include "../../../modules/task_2/mityagina_d_reduce/reduce.h"
#include <math.h>
#include <ctime>
#include <random>
#include <algorithm>
#include <stdexcept>

int help_me_reduce(
  int rank_came, 
  int root,
  int num_of_proc, 
  void *sendbuf, 
  void *recvbuf, 
  int how_much, 
  MPI_Datatype datatype, 
  func op, 
  MPI_Comm comm) 
{
  MPI_Status status;
  int rank = rank_came, num = num_of_proc;
  int is_it_odd = 0, type_size, step = 1;

  while (num > 1) {
    if(rank < num) {
      is_it_odd = num % 2;
      if(rank % 2 != 0) {
        MPI_Send(sendbuf, how_much, datatype, (rank - 1) * step, 19, comm);
        rank *= num;
        break;
      } else {
        if(rank != (num - 1)) 
        {
          MPI_Recv(recvbuf, how_much, datatype, (rank + 1) * step, 19, comm, &status);
          op(recvbuf, sendbuf, how_much);
        }
        rank /= 2;
      }
      step *= 2;
    }
    num = num / 2 + is_it_odd;
  }
  if(rank_came == 0) {
    MPI_Type_size(datatype, &type_size);
    memcpy(recvbuf, sendbuf, how_much * type_size);
  }

  return 0;
}

int Reduce(
	void *where_to_send_from,
	void *where_to_send_to,
	int how_much,
	MPI_Datatype datatype,
	func op,
	int root,
	MPI_Comm comm
	) 
{
  int rank, num_of_proc; 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);
  help_me_reduce(rank, root, num_of_proc, where_to_send_from, where_to_send_to, how_much, datatype, op, comm);

  return 0;
}