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
};

int main(){
    string file_name = "/Users/adinakatz/Documents/OneDrive - Technion/semester_5/OS/hw3/wet3/Tools/example_IO_files/tiny.txt";
    game_params params = {1, 1, file_name, true, true};
    GameTester game_tester(params);
    game_tester.test_one_round();
}