/*
 * Scalable Affinity Propagation (ScaleAP)
 * Last update: June 22, 2020
 * Author: Hiroaki Shiokawa
 */
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstring>
#include <cassert>

#include "data.hpp"
#include "scaleap.hpp"
#include "util.hpp"

using namespace std;

char *simfile = NULL;
float lambda = 0.5;
unsigned int max_itr = 1000;
unsigned int stop_condition = 1000;
bool display = false;
bool result = false;
char *logfile = NULL;
double pref = 0.5;

void
usage(char *prog_name, const string more, bool help){
  if(!help){
    cerr << "[Error] " << more << endl;
  }else{
    cerr << "[Info] " << more << endl;    
  }
  cerr << "[Usage] " << prog_name << " -i <sim_file_name> [-l <lambda>] [-t <max_itr>] [-p <preference>] [-L <log_file_name>] [-D] [-R] [-H]" << endl;
  // Display help
  if(help){
    cerr << "\t-i <sim_file_name>: Set the output file name." << endl;
    cerr << "\t-l <lambda>: Set dumping factor." << endl;
    cerr << "\t-t <max_itr>: Set a maximum number of iterations." << endl;
    cerr << "\t-p <preference>: Set a preference value." << endl;
    cerr << "\t-D: Display statistics." << endl;
    cerr << "\t-R: Display clusteering results" << endl;
    cerr << "\t-H: Display help menu." << endl;
    cerr << "" << endl;
  }
  cerr << endl;
  exit(0);
}

void
check_args(int pos, int argc, char *prog_name, string more){
  if(pos >= argc){
    usage(prog_name, more, false);
  }
}

void
parse_args(int argc, char **argv) {
  if(argc <= 1){
    usage(argv[0], "Arguments are missing.", true);    
  }
  
  for(int i = 1; i < argc; i++){
    if(argv[i][0] == '-'){
      switch(argv[i][1]){
      case 'i':
	check_args(i+1, argc, argv[0], "Invalid arguments at "+string(argv[i]));
	simfile = argv[i+1];
	i+=1;
	break;
      case 'l':
	check_args(i+1, argc, argv[0], "Invalid arguments at "+string(argv[i]));
	lambda = atof(argv[i+1]);
	i+=1;
	break;
      case 't':
	check_args(i+1, argc, argv[0], "Invalid arguments at "+string(argv[i]));
	max_itr = atoi(argv[i+1]);
	i+=1;
	break;
      case 's':
	check_args(i+1, argc, argv[0], "Invalid arguments at "+string(argv[i]));
	stop_condition = atoi(argv[i+1]);
	i+=1;
	break;
      case 'p':
	check_args(i+1, argc, argv[0], "Invalid arguments at "+string(argv[i]));
	pref = atof(argv[i+1]);
	i+=1;
	break;
      case 'D':
	display = true;
	break;
      case 'R':
	result = true;
	break;
      case 'H':
	usage(argv[0], "Help menu", true);
	break;
      default:
	usage(argv[0], "Unknown option", false);
      }
    }
  }

  if(simfile == NULL){
    usage(argv[0], "<sim_file_name> is missing.", false);
  }

}


int
main(int argc, char **argv){
  parse_args(argc, argv);
  // -----------------------------------------------------------------------------------
  // Main procedures
  // -----------------------------------------------------------------------------------
  Data data(simfile, pref);
  ScaleAP scaleap(&data, lambda, max_itr, stop_condition);

  Chrono_t start_time = get_time(); // start time
  scaleap.run();
  Chrono_t end_time = get_time(); // end time
  
  // -----------------------------------------------------------------------------------
  // Execute option commands
  // -----------------------------------------------------------------------------------
  // Display statistics
  if(display){
    cout << "# of data objects: " << data.get_N() << endl;
    cout << "Time duration (sec): " << get_time_sec(&start_time, &end_time) << endl << endl;
  }

  if(result){
    unsigned int N = data.get_N();
    cout << "Object\tCluster" << endl;
    for(unsigned int i=0; i<N; ++i){
      cout << i << "\t" << scaleap.get_examplar(i) << endl;
    }
  }

}
