//
// Created by Adina Katz on 05/01/2021.
//

#ifndef WET3_BARRIER_HPP
#define WET3_BARRIER_HPP

#include <semaphore.h>

class Barrier {
private:
    unsigned int N; // number of threads
    unsigned int counter;
    sem_t  barrier;
    sem_t checkpoint;
    pthread_mutex_t lock;
public:
    Barrier(unsigned int num_of_threads);
    void wait();
    ~Barrier();
};

#endif //WET3_BARRIER_HPP
