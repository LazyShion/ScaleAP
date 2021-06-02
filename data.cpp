#include "data.hpp"

using namespace std;

Data::Data(){}

Data::Data(char *simfile, double pref){
  read_sim_vector(simfile);
  init(pref);
}

void
Data::read_sim_vector(char *simfile){
  ifstream finput;
  finput.open(simfile, fstream::in | fstream::binary);

  finput.read((char *)(&N), 4);
  size = N*N;

  sim_vector.resize(size);
  finput.read((char *)(&sim_vector[0]), size*8);
 
  finput.close();
}

void
Data::init(double pref){
  delta_sim_vector.resize(size, 0);
  min_index.resize(N, 0);
  vector<double>::iterator it, start = sim_vector.begin();

  for(unsigned int i=0; i<N; ++i){
    // init preference
    sim_vector[N*i + i] = pref;

    // get min sim for i
    it = min_sim_index(i);
    min_index[i] = (it - start)%N;

    // store delta similarity
    for(unsigned int j=0; j<N; ++j){
      delta_sim_vector[N*i + j] = sim_vector[N*i +j] - *it;
    }
  }

}
