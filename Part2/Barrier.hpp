//
// Created by Adina Katz on 05/01/2021.
//

#ifndef WET3_BARRIER_HPP
#define WET3_BARRIER_HPP

#include <semaphore.h>
#include "../Part1/Semaphore.hpp"

class Barrier {
private:
    uint num_of_threads;
    volatile int counter; // can be negative
    pthread_mutex_t mut_lock;
    pthread_cond_t cond_var;
    void init_locks();
public:
    Barrier(unsigned int num_of_threads);
    void wait();
    void increase();
    void decrease();
    ~Barrier();
};

#endif //WET3_BARRIER_HPP
