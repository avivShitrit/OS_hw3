//
// Created by Aviv on 04/01/2021.
//

#include "Part1/Headers.hpp"
#include "Part2/utils.hpp"

int main(){
    string file_name = "/mnt/c/Users/Aviv/Documents/operating_systems_git/OS_hw3/tiny.txt";
    cout<<(8/3)<<endl;
    vector<vector<string>> data;
    for(auto line : utils::read_lines(file_name)){
        if(line != "\r") {
            data.push_back(utils::split(line, ' '));
        }
    }
    auto data_ptr = &data;
    int rows = data.size();
    int columns = data[0].size();
    auto curr = new int_mat();
    cout<<"rows = "<<rows<<", columns = "<<columns<<endl;
    vector<uint> tmp;
    for(auto line : *data_ptr){
        for(int i=0; i<columns; i++) {
            string word = line[i];
            tmp.push_back(std::stoi(word, nullptr, 10));
        }
        curr->push_back(tmp);
        tmp.clear();
    }
    for(auto line : *curr){
        for(auto word : line){
            cout<<word<<",";
        }
        cout<<endl;
    }
    cout<<"rows = "<<rows<<", columns = "<<columns<<endl;
}
