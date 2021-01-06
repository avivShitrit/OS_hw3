#ifndef _GLOBAL_H
#define _GLOBAL_H
/*--------------------------------------------------------------------------------
										Libraries
--------------------------------------------------------------------------------*/
// Data Structures: 
#include <vector>
#include <string>
#include <queue>
#include <list>
#include <iterator>
#include <map>

// Streams  & Filesystem:
#include <fstream>
#include <iostream>
#include <sstream>

// Utility:
#include <cmath>
#include <cassert>
#include <chrono>
#include <algorithm>
#include <numeric>  
#include <unistd.h>

// Threads & Synchronization:
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "PCQueue.hpp"
#include "Semaphore.hpp"
#include "../Part2/Barrier.hpp"
/*--------------------------------------------------------------------------------
									   Typedefs
--------------------------------------------------------------------------------*/
using std::string;
using std::vector;
using std::queue;
using std::map;

using std::ostream;
using std::ifstream;
using std::fstream;
using std::istringstream;
using std::chrono::time_point;
using std::cout;
using std::cerr;
using std::endl;

typedef enum {PHASE1, PHASE2, DONE} Phase;
typedef unsigned int uint;
typedef vector<vector<uint>> int_mat; // Use this as your Game Field
typedef struct {
    Phase phase;
    uint start_row;
    uint end_row;
} Job;
/*--------------------------------------------------------------------------------
								  Defines & Macros
--------------------------------------------------------------------------------*/
#define PRINT_BOARD true // Boolean flag - Responsible for controling whether the board is printed or not
#define GEN_SLEEP_USEC 300000 // Default : 300000. The approximate time the board is displayed each generation in micro-seconds
#define DEF_MAT_DELIMITER ' ' // The seperator betweens 0s and 1s in your matrix input file 
#define DEF_RESULTS_FILE_NAME "results.csv" // The filename of the results 

// Macros - For your comfort
#define DEBUG 1
#define DEBUG_MES(mes) if(DEBUG) cout << mes << endl;
#define user_error(mes,exp) if(!exp){cerr << "Fatal: " <<  mes << endl; exit(1);}


#endif

