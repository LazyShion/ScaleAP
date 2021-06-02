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

#include "util.hpp"

using namespace std;

char *input_file = NULL;
char *output_file = NULL;
unsigned int N = 0;

void
usage(char *prog_name, const string more, bool help){
  cerr << "[Error] " << more << endl;
  cerr << "[Usage] " << prog_name << " -i <input_file_name> -o <output_file_name> [-h]" << endl;
  // Display help
  if(help){
    cerr << "\t-h: Display help menu" << endl;
    cerr << "\t-i <input_file_name>: Set the input file name." << endl;
    cerr << "\t-o <output_file_name>: Set the output file name." << endl;
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
  for(int i = 1; i < argc; i++){
    if(argv[i][0] == '-'){
      switch(argv[i][1]){
      case 'i':
	check_args(i+1, argc, argv[0], "Invalid arguments at "+string(argv[i]));
	input_file = argv[i+1];
	i++;
	break;
      case 'o':
	check_args(i+1, argc, argv[0], "Invalid arguments at "+string(argv[i]));
	output_file = argv[i+1];
	i++;
	break;
      case 'h':
	usage(argv[0], "Help menu", true);
	break;
      default:
	usage(argv[0], "Unknown option", false);
      }
    }
  }

  if(input_file == NULL){
    usage(argv[0], "<input_file_name> is missing.", false);
  }
  
  if(output_file == NULL){
    usage(argv[0], "<output_file_name> is missing.", false);
  }
}

void
read_file(vector<double> *sim){
  ifstream finput;
  finput.open(input_file, fstream::in);
  
  finput >> N;
  (*sim).resize(N*N, 0);
  for(unsigned int i=0, N2=N*N; i<N2; ++i){
    finput >> (*sim)[i];
  }
  
  finput.close();
}

void
write_binary(vector<double> *sim){
  ofstream foutput;
  foutput.open(output_file, fstream::out | fstream::binary);

  foutput.write((char *)(&N), 4);

  for(unsigned int i=0, N2=N*N; i<N2; ++i){
    double s = (*sim)[i];
    foutput.write((char *)(&s), 8);
  }

  foutput.close();
}

int
main(int argc, char **argv){
  if(argc > 1){
    parse_args(argc, argv);
  }
  // -----------------------------------------------------------------------------------
  // Main procedures
  // -----------------------------------------------------------------------------------

  vector<double> sim_vector(0);
  
  // Read input file
  read_file(&sim_vector);
  
  // Write output file
  write_binary(&sim_vector);
  
}
