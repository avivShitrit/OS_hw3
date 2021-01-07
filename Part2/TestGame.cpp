//
// Created by Aviv on 06/01/2021.
//

#include "Game.hpp"
#include "../Part1/Headers.hpp"





class GameTester : public Game{
public:
    GameTester(game_params params): Game(params){}
    void test_init_destroy(){
        this->_init_game();
        this->_destroy_game();
    }
    void test_print_board(){
        this->_init_game();
        char header[8] = "Testing";
        this->print_board(header);
        this->_destroy_game();
    }
    void test_one_round(){
        this->_init_game();
        this->_step(1);
        char header[8] = "Testing";
        this->print_board(header);
        this->_destroy_game();
    }
    void test_n_rounds(int n) {
        this->_init_game();
        for (int i = 0; i < n; ++i) {
            this->_step(1);
            string header = "Round" + std::to_string(i);
            this->print_board(header.c_str());
        }
        this->_destroy_game();
    }
};

int main(){
    string file_name = "/home/student/CLionProjects/OS_hw3/Tools/example_IO_files/my_test.txt";
    game_params params = {1, 10, file_name, true, true};
    GameTester game_tester(params);
//    game_tester.test_one_round();
    game_tester.test_n_rounds(10);
}