#if 1
#include <iostream>
#include <omp.h>

int test_openmp_start() {

#pragma omp parallel
    {
        std::cout << "Thread = " << omp_get_thread_num() << std::endl;

        if (0 == omp_get_thread_num()) {
            std::cout << "Number of threads = " << omp_get_num_threads() << std::endl;
        }
    }

    return 0;
}

#endif
