#ifndef __THREAD_H
#define __THREAD_H

#include "../Part1/Headers.hpp"

class Thread
{
public:
	Thread(uint thread_id) 
	{
		// Only places thread_id
		m_thread_id = thread_id;
	} 
	virtual ~Thread() {} // Does nothing 

	/** Returns true if the thread was successfully started, false if there was an error starting the thread */
	// Creates the internal thread via pthread_create 
	bool start()
	{
        int res = pthread_create(&m_thread, nullptr, entry_func, this);
        if (res != 0) return false;

        return true;
	}

	/** Will not return until the internal thread has exited. */
	void join()
	{
        pthread_join(m_thread, nullptr);  // is this enough?!
    }

	/** Returns the thread_id **/
	uint thread_id()
	{
		return m_thread_id;
	}
protected:
	// Implement this method in your subclass with the code you want your thread to run. 
	virtual void thread_workload() = 0;
	uint m_thread_id; // A number from 0 -> Number of threads initialized, providing a simple numbering for you to use

private:
	static void * entry_func(void * thread) { ((Thread *)thread)->thread_workload(); return NULL; }
	pthread_t m_thread;
};

class GameThread : public Thread
{
public:
    GameThread(uint thread_id, int_mat **curr, int_mat **next, PCQueue<Job> *jobs, Barrier *barrier);
    ~GameThread() override = default;
    void thread_workload() override;

private:
    int_mat **curr;
    int_mat **next;
    PCQueue<Job> *jobs_queue;
    Barrier *barrier;

    void phase1ExecuteJob(Job job);
    void phase2ExecuteJob(Job job);

    void getCellNeighbours(int row, int col, map<int, int> &neighbours, Job &job);
    bool isCellBorne(map<int, int> &neighbours);
    bool doesCellStayAlive(map<int, int> &neighbours);
    void setCellSpecie(int i, int j, map<int, int> &neighbours);
    void setCellNewSpecie(int i, int j, map<int, int> &neighbours);
    void setCellAlive(int i, int j);
    void setCellDead(int i, int j);
};


#endif
