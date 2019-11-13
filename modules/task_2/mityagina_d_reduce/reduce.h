// Copyright 2019 Mityagina Daria
#ifndef MODULES_TASK_2_MITYAGINA_D_REDUCE_H_
#define MODULES_TASK_2_MITYAGINA_D_REDUCE_H_
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

typedef void (*func)(void *a, void *b, int);

int help_me_reduce(int rank_came, 
	int num_of_proc, 
	void *sendbuf, 
	void *recvbuf, 
	int how_much, 
	MPI_Datatype datatype, 
	func op, MPI_Comm comm);

// основная работа
int Reduce(
	void *where_to_send_from,
	void *where_to_send_to,
	int how_much,
	MPI_Datatype type,
	func op,
	int root,
	MPI_Comm comm
	); 

#endif  // MODULES_TASK_2_MITYAGINA_D_REDUCE_H_

