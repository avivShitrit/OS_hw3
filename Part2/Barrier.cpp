//
// Created by Adina Katz on 05/01/2021.
//

#include "Barrier.hpp"
#include <pthread.h>

Barrier::Barrier(unsigned int num_of_threads) : N(num_of_threads) {
    counter = 0;
    pthread_mutex_init(&lock, nullptr);
    sem_init(&barrier, 0, 0);
    sem_init(&checkpoint, 0, N);
}

void Barrier::increase() {
    sem_wait(&checkpoint);
    pthread_mutex_lock(&lock);
    counter += 1;
    pthread_mutex_unlock(&lock);
}

void Barrier::decrease() {
    pthread_mutex_lock(&lock);
    counter -= 1;
    pthread_mutex_unlock(&lock);
    sem_post(&barrier);
}

void Barrier::wait() {
    // todo: finish
    while (counter != 0) {
        sem_wait(&barrier);
        sem_post(&checkpoint);
    }
    sem_post(&barrier);


//    pthread_mutex_lock(&lock);
//    counter += 1;
//    if (counter == N) { //maybe while ?
//        sem_wait(&checkpoint);
//        sem_wait(&barrier);
//    }
//    pthread_mutex_unlock(&lock);
//    sem_wait(&barrier);
//    sem_post(&checkpoint);
//
//    pthread_mutex_lock(&lock);
//    counter -= 1;
//    if (counter == 0) {
//        sem_wait(&barrier);
//        sem_post(&checkpoint);
//    }
//    pthread_mutex_unlock(&lock);
//
//    sem_wait(&checkpoint);
//    sem_post(&checkpoint);
}

Barrier::~Barrier() {
    pthread_mutex_destroy(&lock);
    sem_destroy(&barrier);
    sem_destroy(&checkpoint);
}