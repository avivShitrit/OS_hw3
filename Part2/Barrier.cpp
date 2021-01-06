//
// Created by Adina Katz on 05/01/2021.
//

#include "Barrier.hpp"
#include <pthread.h>

Barrier::Barrier(unsigned int num_of_threads) : N(num_of_threads) {
    counter = 0;
    pthread_mutex_init(&lock, nullptr);
    sem_init(&barrier, 0, 0);
    sem_init(&checkpoint, 0, 1);
}

void Barrier::wait() {

}

Barrier::~Barrier() {
    pthread_mutex_destroy(&lock);
    sem_destroy(&barrier);
    sem_destroy(&checkpoint);
}