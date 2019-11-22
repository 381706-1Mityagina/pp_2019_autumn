// Copyright 2019 Mityagina Daria

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <random>
#include "../../../modules/task_3/mityagina_d_quick_sort/quick_sort.h"

void testing_lab(int size) {
    int rank;
    std::vector<int> _vector, result_my, result;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        _vector = getRandomVector(size);
    }
    if (rank == 0) {
      result = q_sort_not_parallel(_vector, 0, size);
    }
    result_my = q_sort_parallel(_vector); 

    if (rank == 0) {
        ASSERT_EQ(result, result_my);
    }
}

TEST(Quick_Sort_MPI, Test_On_Size_1) {
    int size = 1;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_2) {
    int size = 2;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_3) {
    int size = 3;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_9) {
    int size = 9;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_91) {
    int size = 91;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_5000) {
    int size = 5000;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Test_On_Size_1000000) {
    int size = 4095;
    testing_lab(size);
}

TEST(Quick_Sort_MPI, Negative_matrix_size) {
    int size = -10;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(std::vector<int> _vector(size, 0));
    }
}

TEST(Quick_Sort_MPI, Empty_matrix) {
    int size = 0;
    int rank;
    std::vector<int> _vector(size, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_ANY_THROW(q_sort_parallel(_vector));
    }
}

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
