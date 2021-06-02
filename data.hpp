#ifndef DATA_H
#define DATA_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

#include "util.hpp"

using namespace std;

class Data{
private:
  // ---------------------
  // Members
  // ---------------------

  unsigned int N; // # of data objects
  unsigned int size; // size of sim_vector
  vector<double> sim_vector; // vector of similarities 

public:
  // ---------------------
  // Members
  // ---------------------

  vector<double> delta_sim_vector; // vector of delta similarities
  vector<unsigned int> min_index; // vector of min similarity index
  
  // ---------------------
  // Constructor
  // ---------------------

  Data();
  Data(char *simfile, double pref);
  
  // ---------------------
  // Functions
  // ---------------------

  void read_sim_vector(char *simfile);
  void init(double pref);

  // ---------------------
  // Inlines 
  // ---------------------

  // Return # of data objects
  inline unsigned int get_N(){
    return N;
  }

  // Return size
  inline unsigned int get_size(){
    return size;
  }
  
  // Return a similarity from i to j
  inline double
  sim(unsigned int i, unsigned int j){
    //assert(i >=0 && i < N);
    //assert(j >=0 && j < N);
    return sim_vector[N*i + j];
  }

  // Return a maximum similarity of object i except for sim(i,j)
  inline double
  max_sim(unsigned int i, unsigned int j){
    //assert(i >=0 && i < N);
    //assert(j >=0 && j < N);
    
    vector<double>::iterator it = sim_vector.begin()+(N*i);

    if(j==0){
      return *max_element(it+1, it+N);
    }else if(j == N-1){
      return *max_element(it, it+N-1);
    }else{
      return max(*max_element(it, it+j), *max_element(it+j+1, it+N));
    }
  }

  // Return an index of minimum similarity of object i except for sim(i,i)
  inline vector<double>::iterator
  min_sim_index(unsigned int i){
    //assert(i >=0 && i < N);    
    vector<double>::iterator it = sim_vector.begin()+(N*i);

    if(i==0){
      return min_element(it+1, it+N);
    }else if(i == N-1){
      return min_element(it, it+N-1);
    }else{
      vector<double>::iterator min1 = min_element(it, it+i);
      vector<double>::iterator min2 = min_element(it+i+1, it+N);
      if(*min1 < *min2){
	return min1;
      }else{
	return min2;
      }
    }
  }

  // Return an index of maximum similarity of object i
  inline unsigned int
  max_sim_index(unsigned int i){
    return (max_element(sim_vector.begin()+N*i, sim_vector.begin()+N*(i+1)) - sim_vector.begin())%N;
  }
  
};

#endif //DATA_H
