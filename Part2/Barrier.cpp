//
// Created by Adina Katz on 05/01/2021.
//

#include "Barrier.hpp"
#include <pthread.h>

Barrier::Barrier(unsigned int num_of_threads) : num_of_threads(num_of_threads) {
    this->counter = 0;
    this->init_locks();
}


void Barrier::increase() {
    DEBUG_MES("increase Locking mutex")
    pthread_mutex_lock(&this->mut_lock);
    DEBUG_MES("increase Counter++")
    this->counter++;
    pthread_mutex_unlock(&this->mut_lock);
    DEBUG_MES("increase Mutex unlocked")
}

void Barrier::decrease(){
    DEBUG_MES("decrease Locking mutex")
    pthread_mutex_lock(&this->mut_lock);
    DEBUG_MES("increase Counter--")
    this->counter--;
    pthread_mutex_unlock(&this->mut_lock);
    DEBUG_MES("decrease Mutex unlocked")
    DEBUG_MES("decrease Signaling to condition")
    pthread_cond_broadcast(&this->cond_var);
}

void Barrier::wait() {
    pthread_mutex_lock(&this->mut_lock);
    DEBUG_MES("wait check counter")
    while(this->counter > 0) {
        DEBUG_MES("wait counter > 0")
        pthread_cond_wait(&this->cond_var,&this->mut_lock);
        DEBUG_MES("wait got signal")
    }
    DEBUG_MES("wait unlocking mutex counter")
    pthread_mutex_unlock(&this->mut_lock);
}


Barrier::~Barrier() {
    pthread_mutex_destroy(&this->mut_lock);
    pthread_cond_destroy(&this->cond_var);
}

void Barrier::init_locks() {
    this->mut_lock = PTHREAD_MUTEX_INITIALIZER;
    this->cond_var = PTHREAD_COND_INITIALIZER;
    pthread_mutex_init(&this->mut_lock, nullptr);
    pthread_cond_init(&this->cond_var, nullptr);
}
