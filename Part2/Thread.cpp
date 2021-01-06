//
// Created by Adina Katz on 05/01/2021.
//
#include "Thread.hpp"

#define DEAD 0

bool isInBounds(int start_row, int end_row, int columns, int i, int j) {
    if (i < 0 || i < start_row || i >= end_row || j < 0 || j >= columns) {
        return false;
    }
    return true;
}


GameThread::GameThread(uint thread_id, int_mat **curr, int_mat **next, PCQueue<Job> *jobs, Barrier *barrier) :
        Thread(thread_id),
        curr(curr),
        next(next),
        jobs_queue(jobs),
        barrier(barrier){}


void GameThread::thread_workload() {
    Job curr_job;
    bool is_done = false;
    while (!is_done) {
        curr_job = jobs_queue->pop();
        barrier->increase();
        if (curr_job.phase == PHASE1) {
            phase1ExecuteJob(curr_job);
        } else if (curr_job.phase == PHASE2) {
            phase2ExecuteJob(curr_job);
        }
        barrier->decrease();
        if (curr_job.phase == DONE) {
            is_done = true;
        }
    }
}

void GameThread::phase1ExecuteJob(Job job) {
    int_mat field = **curr;
    int n = field[0].size();
    for (int i = job.start_row; i < job.end_row; ++i) {
        for (int j = 0; j < n; ++j) {
            map<int, int> neighbours; // safe, threads don't share stack
            getCellNeighbours(i, j, neighbours, job);
            if (field[i][j] == DEAD) {
                if (isCellBorne(neighbours)) {
                    setCellSpecie(i, j, neighbours);
                } else {
                    setCellDead(i, j);
                }
            } else {
                if (doesCellStayAlive(neighbours)) {
                    setCellAlive(i, j);
                } else {
                    setCellDead(i, j);
                }
            }
        }
    }
}

void GameThread::phase2ExecuteJob(Job job) {
    int_mat field = **curr;
    int n = field[0].size();
    for (int i = job.start_row; i < job.end_row; ++i) {
        for (int j = 0; j < n; ++j) {
            map<int, int> neighbours;
            getCellNeighbours(i, j, neighbours, job);
            if (field[i][j] != DEAD) {
                setCellNewSpecie(i, j, neighbours);
            }
        }
    }
}

bool GameThread::isCellBorne(map<int, int> &neighbours) {
    if (neighbours.size() == 3) {
        return true;
    }
    return false;
}

bool GameThread::doesCellStayAlive(map<int, int> &neighbours) {
    if (neighbours.size() == 2 || neighbours.size() == 3) {
        return true;
    }
    return false;
}

void GameThread::setCellSpecie(int i, int j, map<int, int> &neighbours) {
    int dominant_specie = 0;
    int num_of_neighbours = 0;
    for (auto species : neighbours) {
        if (species.second > num_of_neighbours) {
            num_of_neighbours = species.second;
            dominant_specie = species.first;
        }
    }
    (**next)[i][j] = dominant_specie;
}

void GameThread::setCellNewSpecie(int i, int j, map<int, int> &neighbours) {
    int new_specie = (**curr)[i][j];
    int total_num_neighbours = 1;
    for (auto species : neighbours) {
        new_specie += (species.first * species.second);
        total_num_neighbours += species.second;
    }
    new_specie = (new_specie / total_num_neighbours); //todo: round always down?
    (**next)[i][j] = new_specie;
}

void GameThread::setCellDead(int i, int j) {
    (**next)[i][j] = DEAD;
}

void GameThread::getCellNeighbours(int row, int col, map<int, int> &neighbours, Job &job) {
    for (int i = row - 1; i <= i + 1; ++i) {
        for (int j = col - 1; j <= j + 1; ++j) {
            if (isInBounds(job.start_row, job.end_row, (**curr)[0].size(), i, j)) {
                int specie = (**curr)[i][j];
                if (neighbours.count(specie) > 0) {
                    neighbours[specie] += 1;
                } else {
                    neighbours.insert({specie, 1});
                }
            }
        }
    }
}

void GameThread::setCellAlive(int i, int j) {
    (**next)[i][j] = (**curr)[i][j];
}



