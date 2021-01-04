//
// Created by Aviv on 04/01/2021.
//

#include "Part1/Headers.hpp"
#include "Part2/utils.hpp"

int main(){
    string file_name = "/mnt/c/Users/Aviv/Documents/operating_systems_git/OS_hw3/tiny.txt";
    vector<vector<string>> data;
    for(auto line : utils::read_lines(file_name)){
        data.push_back(utils::split(line, ' '));
    }
    auto data_ptr = &data;
    int rows = data.size();
    int columns = data[0].size();
    auto curr = new int_mat;
    cout<<"rows = "<<rows<<", columns = "<<columns<<endl;
    for(int row_idx =0; row_idx<this->num_of_rows; row_idx++){
        for(int column_idx =0; column_idx<this->num_of_columns; column_idx++){
            string tmp = (*data_ptr)[row_idx][column_idx];
            (*curr)[row_idx][column_idx] = std::stoi(tmp);
        }
    }
    cout<<"rows = "<<rows<<", columns = "<<columns<<endl;
}
