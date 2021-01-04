#include "Game.hpp"
#include "utils.hpp"

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
		print_board(nullptr);
	} // generation loop
	print_board("Final Board");
	_destroy_game();
}

void Game::_init_game() {
	// Create game fields - Consider using utils:read_file, utils::split
    vector<vector<string>> data;
	for(auto line : utils::read_lines(this->init_params.filename)){
        data.push_back(utils::split(line, ' '));
	}
	this->num_of_rows = data.size();
	this->num_of_columns = data[0].size();
	this->curr = new int_mat;
    this->next = new int_mat;
    _fill_curr_board(&data);
	// Create & Start threads
	// Testing of your implementation will presume all threads are started here
}

void Game::_step(uint curr_gen) {
	// Push jobs to queue
	// Wait for the workers to finish calculating 
	// Swap pointers between current and next field 
	// NOTE: Threads must not be started here - doing so will lead to a heavy penalty in your grade 
}

void Game::_destroy_game(){
	// Destroys board and frees all threads and resources
    delete this->curr;
    delete this->next;
    this->curr = nullptr;
    this->next = nullptr;
	// Not implemented in the Game's destructor for testing purposes. 
	// All threads must be joined here
	for (uint i = 0; i < m_thread_num; ++i) {
        m_threadpool[i]->join();
    }
}


void Game::_fill_curr_board(vector<vector<string>>* data) {
    for(int row_idx =0; row_idx<this->num_of_rows; row_idx++){
        for(int column_idx =0; column_idx<this->num_of_columns; column_idx++){
            string tmp = (*data)[row_idx][column_idx];
            (*this->curr)[row_idx][column_idx] = std::atoi(tmp);
        }
    }
}

/*--------------------------------------------------------------------------------
								
--------------------------------------------------------------------------------*/
inline void Game::print_board(const char* header) {

	if(print_on){ 

		// Clear the screen, to create a running animation 
		if(interactive_on)
			system("clear");

		// Print small header if needed
		if (header != nullptr)
			cout << "<------------" << header << "------------>" << endl;
		
		// TODO: Print the board 

		// Display for GEN_SLEEP_USEC micro-seconds on screen 
		if(interactive_on)
			usleep(GEN_SLEEP_USEC);
	}

}

Game::Game(game_params params)  {
    this->init_params = params;
    this->m_gen_num = params.n_gen;
    this->interactive_on = params.interactive_on;
    this->print_on = params.print_on;
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



