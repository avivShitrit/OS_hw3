//
// Created by Aviv on 03/01/2021.
//

#include "Semaphore.hpp"

static const int N = 40;
static const int Z = 2000000;
static int COUNTER = 0;


void *thread_func(void *semaphore) {
    auto sem = (Semaphore*)semaphore;

    sem->down();
    for(int i = 0; i < Z; i++) {
        ++COUNTER;
    }
    sem->up();
    return nullptr;
}


int main(){
    Semaphore sem(1);
    pthread_t threads[N];
    for(int i = 0; i < N; i++) {
        pthread_create(&threads[i], nullptr,thread_func,&sem);
    }
    sem.count(1);
    if(COUNTER < N*Z){
        cout<<"Wrong!, counter = "<<COUNTER<<endl;
    }
    cout<<"pass count condition";
    for(int i = 0; i < N; i++) {
        pthread_join(threads[i], nullptr);
    }
    assert(COUNTER == Z*N && "COUNTER is not equal Z*N");
    fprintf(stderr,"%d\n", COUNTER);
    return EXIT_SUCCESS;
}

