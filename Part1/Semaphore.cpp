//
// Created by Aviv on 03/01/2021.
//

#include "Headers.hpp"
#include "Semaphore.hpp"

void Semaphore::init_locks(){

    this->mut_lock = PTHREAD_MUTEX_INITIALIZER;
    this->cond_var = PTHREAD_COND_INITIALIZER;
    pthread_mutex_init(&this->mut_lock, nullptr);
    pthread_cond_init(&this->cond_var, nullptr);
}
Semaphore::Semaphore() {
    DEBUG_MES("Semaphore Started creating semaphore")
    this->counter = 0;
    init_locks();
    DEBUG_MES("Semaphore Done creating semaphore")
}

Semaphore::Semaphore(unsigned val){
    DEBUG_MES("Semaphore Started creating semaphore")
    this->counter = val;
    init_locks();
    DEBUG_MES("Semaphore Done creating semaphore")
}
Semaphore::~Semaphore(){
    DEBUG_MES("~Semaphore Destroying semaphore")
    pthread_mutex_destroy(&this->mut_lock);
    pthread_cond_destroy(&this->cond_var);
}

void Semaphore::down() {
    DEBUG_MES("down Locking mutex")
    pthread_mutex_lock(&this->mut_lock);
    DEBUG_MES("down Mutex locked")
    bool once = true;
    while(this->counter == 0) {
        if(once) {//only for debugging
            DEBUG_MES("down Waiting for condition")
            once = false;
        }
        pthread_cond_wait(&this->cond_var,&this->mut_lock);
    }
    --(this->counter);
    DEBUG_MES("down Unlocking mutex")
    pthread_mutex_unlock(&this->mut_lock);
    DEBUG_MES("down Mutex unlocked")
}

void Semaphore::up() {
    DEBUG_MES("up Locking mutex")
    pthread_mutex_lock(&this->mut_lock);
    DEBUG_MES("up Mutex locked")
    ++(this->counter);

    DEBUG_MES("up Unlocking mutex")
    pthread_mutex_unlock(&this->mut_lock);
    DEBUG_MES("up Mutex unlocked")
    DEBUG_MES("up Signaling to condition")
    pthread_cond_signal(&this->cond_var);
}

void Semaphore::count(unsigned int val) {
    pthread_mutex_lock(&this->mut_lock);
    while(this->counter < val) {
        pthread_cond_wait(&this->cond_var,&this->mut_lock);
    }
    pthread_mutex_unlock(&this->mut_lock);
    pthread_cond_signal(&this->cond_var);
}
