#include "Game.hpp"

static const char *colors[7] = {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
/*--------------------------------------------------------------------------------
								
--------------------------------------------------------------------------------*/
void Game::run() {
	_init_game(); // Starts the threads and all other variables you need
	print_board("Initial Board");
	for (uint i = 0; i < m_gen_num; ++i) {
		auto gen_start = std::chrono::system_clock::now();
		_step(i); // Iterates a single generation 
		auto gen_end = std::chrono::system_clock::now();
		m_gen_hist.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
		print_board(nullptr); //todo: need to pass board
	} // generation loop
	print_board("Final Board");
	_destroy_game();
}

void Game::_init_game() {
	// Create game fields - Consider using utils:read_file, utils::split
    vector<vector<string>> data;
	for(auto line : utils::read_lines(this->init_params.filename)){
	    if(line != "\r")
        data.push_back(utils::split(line, ' '));
	}
	this->num_of_rows = data.size();
	this->num_of_columns = data[0].size();
	this->curr = new int_mat;
    this->next = new int_mat(this->num_of_rows, vector<uint>(this->num_of_columns, 0));
    _fill_curr_board(&data);

    //Create Game structures
    this->m_thread_num = num_of_rows<this->init_params.n_thread ? num_of_rows : this->init_params.n_thread;
    this->jobs_pcq = new PCQueue<Job>();
    this->barrier = new Barrier(this->m_thread_num);

    //Create and start Threads
    for(uint i=0; i < this->m_thread_num; i++){
        GameThread* tmp_ptr = new GameThread(i, &this->curr, &this->next, this->jobs_pcq, this->barrier);
        bool result = tmp_ptr->start();
        if(!result){
            cerr<<"Problem in Thread->start() number:"<<i<<" Shutting done...";
            _destroy_game();

            exit(-1);
        }
        m_threadpool.push_back(tmp_ptr);
    }
}

void Game::_step(uint curr_gen) {
    /** 1. init phase1 **/
    uint job_size = this->num_of_rows / this->m_thread_num;
    vector<Job> jobs;
    uint i=0;
    for(; i<num_of_rows-job_size; i+=job_size){
        jobs.push_back({PHASE1, i, i+job_size});
    }
    jobs.push_back({PHASE1, i, this->num_of_rows});

    DEBUG_MES("_step: PHASE1 - Starting to push jobs to Queue")
    for(auto job : jobs){
        this->jobs_pcq->push(job);
    }
    while(!jobs_pcq->empty()){}
    this->barrier->wait();
    DEBUG_MES("_step: PHASE1 - Queue is empty and all jobs finished, moving to Phase 2")

    /** 2. init phase2 **/
    std::swap(this->curr, this->next);
    print_board("end phase 1");
    this->next->assign(this->num_of_rows, vector<uint>(this->num_of_columns, 0));
    jobs.clear();
    i=0;
    for(; i < this->num_of_rows - job_size; i += job_size){
        jobs.push_back({PHASE2, i, i+job_size});
    }
    jobs.push_back({PHASE2, i, this->num_of_rows});

    DEBUG_MES("_step: PHASE2 - Starting to push jobs to Queue")
    for(auto job : jobs){
        jobs_pcq->push(job);
    }
    while(!this->jobs_pcq->empty()){}
    this->barrier->wait();
    DEBUG_MES("_step: PHASE2 - Queue is empty and all jobs finished This generation is over")

    std::swap(this->curr, this->next);
    this->next->assign(this->num_of_rows, vector<uint>(this->num_of_columns, 0));
}

void Game::_destroy_game(){
	// Destroys board and frees all threads and resources
	DEBUG_MES("_destroy_game: Sending Done to all threads")
	for(uint i=0; i<this->m_thread_num; i++){
        this->jobs_pcq->push({DONE, 0,0});
	}
	DEBUG_MES("_destroy_game: Wating for all threads to join")
    for(auto thread : this->m_threadpool){
        thread->join();
    }
    DEBUG_MES("_destroy_game: All Threads joined successfully")
    delete this->curr;
    delete this->next;
    this->curr = nullptr;
    this->next = nullptr;
	for(auto thread : this->m_threadpool){
	    delete thread;
	}
	this->m_threadpool.clear();
    delete this->jobs_pcq;
	delete this->barrier;
}


void Game::_fill_curr_board(vector<vector<string>>* data) {
    vector<uint> tmp;
    for(auto line : *data){
        for(uint i = 0; i < this->num_of_columns; i++) {
            string word = line[i];
            tmp.push_back(std::stoi(word, nullptr, 10));

        }
        curr->push_back(tmp);
        tmp.clear();
    }
}

/*--------------------------------------------------------------------------------
								
--------------------------------------------------------------------------------*/
inline void Game::print_board(const char* header) {

	if(this->print_on){

		// Clear the screen, to create a running animation 
		if(this->interactive_on)
			system("clear");

		// Print small header if needed
		if (header != nullptr)
			cout << "<------------" << header << "------------>" << endl;

        cout << u8"╔" << string(u8"═") * this->num_of_columns << u8"╗" << endl;
        for (uint i = 0; i < this->num_of_rows; ++i) {
            cout << u8"║";
            for (uint j = 0; j < this->num_of_columns; ++j) {
                uint val = (*this->curr)[i][j];
                if (val > 0)
                    cout << colors[val % 7] << u8"█" << RESET;
                else
                    cout << u8"░";
            }
            cout << u8"║" << endl;
        }
        cout << u8"╚" << string(u8"═") * this->num_of_columns << u8"╝" << endl;

		// Display for GEN_SLEEP_USEC micro-seconds on screen 
		if(this->interactive_on)
			usleep(GEN_SLEEP_USEC);
	}

}

Game::Game(game_params params){
    this->init_params = params;
    this->m_gen_num = params.n_gen;
    this->interactive_on = params.interactive_on;
    this->print_on = params.print_on;
    this->m_thread_num = 0;
    this->jobs_pcq = nullptr;
    this->curr = nullptr;
    this->next = nullptr;
    this->num_of_rows = -1;
    this->num_of_columns = -1;
    this->barrier = nullptr;
}

Game::~Game() {
}

uint Game::thread_num() const {
    return this->m_thread_num;
}


/* Function sketch to use for printing the board. You will need to decide its placement and how exactly 
	to bring in the field's parameters. 

		cout << u8"╔" << string(u8"═") * field_width << u8"╗" << endl;
		for (uint i = 0; i < field_height ++i) {
			cout << u8"║";
			for (uint j = 0; j < field_width; ++j) {
                if (field[i][j] > 0)
                    cout << colors[field[i][j] % 7] << u8"█" << RESET;
                else
                    cout << u8"░";
			}
			cout << u8"║" << endl;
		}
		cout << u8"╚" << string(u8"═") * field_width << u8"╝" << endl;
*/ 



