// Copyright 2019 Mityagina Daria
#include "../../../modules/task_1/mityagina_d_sum_of_matrix_elements/sum_of_matrix_elements.h"

#include <iostream>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <mpi.h>

void testing_lab (int size)
{
    int answer, result = 0;
    int rank;
    std::vector<int> matrix(size, 0);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) 
    {
        matrix = getMatrix(size);
    }

    answer = Work(size, matrix);

    if (rank == 0) 
    {
        for (int i = 0; i < size; ++i)
        {
            result += matrix[i];
        }
        ASSERT_EQ(result, answer);
    }
    
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_91) 
{
    int size = 91;

    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_100) 
{
    int size = 100;

    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_5000) 
{
    int size = 5000;

    testing_lab(size);
}

TEST(Sum_of_matrix_elements_MPI, Test_On_Size_1000000) 
{
    int size = 1000000;

    testing_lab(size);
}

using namespace std;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}