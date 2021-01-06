#ifndef _QUEUEL_H
#define _QUEUEL_H
#include "Headers.hpp"
// Single Producer - Multiple Consumer queue
template <typename T>class PCQueue
{

public:
	// Blocks while queue is empty. When queue holds items, allows for a single
	// thread to enter and remove an item from the front of the queue and return it. 
	// Assumes multiple consumers.
	T pop(); 

	// Allows for producer to enter with *minimal delay* and push items to back of the queue.
	// Hint for *minimal delay* - Allow the consumers to delay the producer as little as possible.  
	// Assumes single producer 
	void push(const T& item);

	//not sync protected, simple if(items.size()>0) return false;
	bool empty();
    PCQueue();

private:
	// Add your class memebers here
    std::queue<T, std::deque<T>> items;

    pthread_mutex_t global_lock;
    pthread_cond_t producer_allowed;
    pthread_cond_t consumers_allowed;

    int consumers_inside;
    int producers_inside;
    int producers_waiting;

};
// Recommendation: Use the implementation of the std::queue for this exercise

template<typename T>
PCQueue<T>::PCQueue() :
    consumers_inside(0),
    producers_inside(0),
    producers_waiting(0) {
    pthread_cond_init(&producer_allowed, nullptr);
    pthread_cond_init(&consumers_allowed, nullptr);
    pthread_mutex_init(&global_lock, nullptr);
}

//consumers
template<typename T>
T PCQueue<T>::pop() {
    pthread_mutex_lock(&global_lock);
    while (items.empty() || (producers_inside > 0 || producers_waiting > 0)) {
        pthread_cond_wait(&consumers_allowed, &global_lock);
    }
    consumers_inside++;

    T item = items.front();
    items.pop();

    consumers_inside--;
    if (consumers_inside == 0) {
        pthread_cond_signal(&consumers_allowed);
    }
    pthread_mutex_unlock(&global_lock);

    return item;
}

template<typename T>
void PCQueue<T>::push(const T &item) {
    pthread_mutex_lock(&global_lock);
    producers_waiting++;
    while (producers_inside + consumers_inside > 0) {
        pthread_cond_wait(&producer_allowed, &global_lock);
    }
    producers_waiting--;
    producers_inside++;

    items.push(item);

    producers_inside--;
    if (producers_inside == 0) {
        pthread_cond_broadcast(&consumers_allowed);
        pthread_cond_signal(&producer_allowed);
    }
    pthread_mutex_unlock(&global_lock);
}

template<typename T>
bool PCQueue<T>::empty() {
    if(items.size()>0)
        return false;
    return true;
}

//template<typename T>
//void PCQueue<T>::consumerLock() {
//    pthread_mutex_lock(&global_lock);
//    while (producers_inside > 0 || producers_waiting > 0) {
//        pthread_cond_wait(&consumers_allowed, &global_lock);
//    }
//    consumers_inside++;
//    pthread_mutex_unlock(&global_lock);
//}
//
//template<typename T>
//void PCQueue<T>::consumerUnLock() {
//    pthread_mutex_lock(&global_lock);
//    consumers_inside--;
//    if (consumers_inside == 0) {
//        pthread_cond_signal(&consumers_allowed);
//    }
//    pthread_mutex_unlock(&global_lock);
//}

//template<typename T>
//void PCQueue<T>::producerLock() {
//    pthread_mutex_lock(&global_lock);
//    producers_waiting++;
//    while (producers_inside + consumers_inside > 0) {
//        pthread_cond_wait(&producer_allowed, &global_lock);
//    }
//    producers_inside++;
//    pthread_mutex_unlock(&global_lock);
//}
//
//template<typename T>
//void PCQueue<T>::producerUnLock() {
//    pthread_mutex_lock(&global_lock);
//    producers_inside--;
//    if (producers_inside == 0) {
//        pthread_cond_broadcast(&consumers_allowed);
//        pthread_cond_signal(&producer_allowed);
//    }
//    pthread_mutex_unlock(&global_lock);
//}


#endif