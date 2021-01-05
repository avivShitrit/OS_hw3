//
// Created by Adina Katz on 05/01/2021.
//
#include "Thread.hpp"

GameThread::GameThread(uint thread_id, int_mat **curr, int_mat **next, PCQueue<Job> &jobs) :
        Thread(thread_id),
        curr(curr),
        next(next),
        jobs_queue(jobs){}


void GameThread::thread_workload() {
    Job curr_job;
    while (1) {
        curr_job = jobs_queue.pop();
        // increase_barrier
        if (curr_job.phase == PHASE1) {
            phase1ExecuteJob(curr_job);
        } else if (curr_job.phase == PHASE2) {
            // if alive find species - calc species- round(max(average species))
        }
        // decrease_barrier
    }
}

void GameThread::phase1ExecuteJob(Job job) {
    // if dead - calc if alive - go over all neighbours
    // if alive - calc dominant species
    // if alive - calc if stays alive
    //else dead
    int n = (**curr)[0].size();
//    int_mat curr_field =
    for (int i = job.start_row; i < job.end_row; ++i) {
        for (int j = 0; j < n; ++j) {

        }
    }
}