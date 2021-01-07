//
// Created by Adina Katz on 05/01/2021.
//
#include "Thread.hpp"

#define DEAD 0

bool isInBounds(int rows, int columns, int i, int j) {
    if (i < 0 || i >= rows || j < 0 || j >= columns) {
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
    while (true) {
        DEBUG_MES("thread_workload #"+std::to_string(this->m_thread_id)+ ": Wating for job")
        curr_job = this->jobs_queue->pop();
        this->barrier->increase();
        DEBUG_MES("thread_workload #"+std::to_string(this->m_thread_id)+ ": Got new Job! " + std::to_string(curr_job.phase))
        if (curr_job.phase == PHASE1) {
            phase1ExecuteJob(curr_job);
        }
        else if (curr_job.phase == PHASE2) {
            phase2ExecuteJob(curr_job);
        }
        DEBUG_MES("thread_workload #"+std::to_string(this->m_thread_id)+ ": decrease()")
        this->barrier->decrease();
        DEBUG_MES("thread_workload #"+std::to_string(this->m_thread_id)+ ": decrease() Done")
        if (curr_job.phase == DONE) {
            DEBUG_MES("thread_workload #"+std::to_string(this->m_thread_id)+ ": Breaking loop")
            return;
        }
    }
}

void GameThread::phase1ExecuteJob(Job job) {
    int_mat field = **this->curr;
    int n = field[0].size();
    DEBUG_MES("phase1 Execute: starting")
    for (uint i = job.start_row; i < job.end_row; ++i) {
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
    int_mat field = **this->curr;
    int n = field[0].size();
    for (uint i = job.start_row; i < job.end_row; ++i) {
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
    int num_of_neighbors = 0;
    for (auto n : neighbours) {
        num_of_neighbors += n.second;
    }
    if (num_of_neighbors == 3) {
        return true;
    }
    return false;
}

bool GameThread::doesCellStayAlive(map<int, int> &neighbours) {
    int num_of_neighbors = 0;
    for (auto n : neighbours) {
        num_of_neighbors += n.second;
    }
    if (num_of_neighbors == 2 || num_of_neighbors == 3) {
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
    (**this->next)[i][j] = dominant_specie;
}

void GameThread::setCellNewSpecie(int i, int j, map<int, int> &neighbours) {
    double sum_species = (**this->curr)[i][j];
    int total_num_neighbours = 1;
    for (auto species : neighbours) {
        sum_species += (species.first * species.second);
        total_num_neighbours += species.second;
    }
    double new_specie = std::round(sum_species / total_num_neighbours);
    (**this->next)[i][j] = new_specie;
}

void GameThread::setCellDead(int i, int j) {
    (**this->next)[i][j] = DEAD;
}

void GameThread::getCellNeighbours(int row, int col, map<int, int> &neighbours, Job &job) {
    for (int i = row - 1; i <= row + 1; ++i) {
        for (int j = col - 1; j <= col + 1; ++j) {
            if (isInBounds((**this->curr).size(), (**this->curr)[0].size(), i, j)) {
                int specie = (**this->curr)[i][j];
                if (specie == DEAD || (i == row && j == col)) continue;
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
    (**this->next)[i][j] = (**this->curr)[i][j];
}



