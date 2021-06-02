#ifndef SCALEAP_H
#define SCALEAP_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <climits>
#include <cassert>

#include "data.hpp"
#include "util.hpp"

using namespace std;

class ScaleAP{
private:
  // ---------------------
  // Members
  // ---------------------
  
  Data *data;
  float lambda;
  unsigned int max_itr;
  unsigned int num_itr;
  unsigned int stop;
  vector<double> *res, *res_prev;
  vector<double> res1, res2;
  vector<double> *ava, *ava_prev;
  vector<double> ava1, ava2;
  vector<double> res_plus_ava;
  vector<unsigned int> examplar;
  vector<unsigned int> max_index;
  vector<double> max_res;
  
  // ---------------------
  // Functions
  // ---------------------

  unsigned int one_aggregated_iteration();

public:
  // ---------------------
  // Constructor
  // ---------------------

  ScaleAP();
  ScaleAP(Data *d, float l, unsigned int t, unsigned int s);

  // ---------------------
  // Functions
  // ---------------------

  void run();

  
  // ---------------------
  // Inlines 
  // ---------------------

  // Return responsibility r(i,j)
  inline double
  get_r(unsigned int i, unsigned int j){
    return (*res_prev)[data->get_N()*i + j];
  }

  // Set responsibility as r(i,j) = r
  inline void
  set_r(unsigned int i, unsigned int j, double r){
    (*res)[data->get_N()*i + j] = r;

    if(r > max_res[j]){
      max_res[j] = r;
    }
  }

  // Update responsibility r(i,j)
  inline double
  update_r(unsigned int i, unsigned int j){
    double new_r = 0.0;
    double rho = data->sim(i,j);

    if(i == j){
      rho -= data->max_sim(i,j);
    }else{
      double val = -DBL_MAX;
      for(unsigned int k=0; k<data->get_N(); ++k){
	if(k != j){
	  val = max(val, data->sim(i,k)+get_a(i,k));
	}
      }
      rho -= val;
    }

    new_r = lambda*get_r(i,j) + (1-lambda)*rho;
    set_r(i, j, new_r);
    
    return new_r;
  }

  // Return availability a(i,j)
  inline double
  get_a(unsigned int i, unsigned int j){
    return (*ava_prev)[data->get_N()*i + j];
  }

  // Set availability as a(i,j) = a
  inline void
  set_a(unsigned int i, unsigned int j, double a){
    (*ava)[data->get_N()*i + j] = a;
  }

  // Update availability a(i,j)
  inline double
  update_a(unsigned int i, unsigned int j){
    double new_a = 0;
    double sum = 0;
    
    unsigned int N = data->get_N();
    for(unsigned int k=0; k<N; ++k){
      if(k!=i && k!=j){
	sum += max(0.0, get_r(k, j));
      }
    }

    double alpha = 0.0;
    if(i == j){
      alpha = sum;
    }else{
      alpha = min(0.0, get_r(j, j)+sum);
    }

    new_a = lambda*get_a(i,j) + (1-lambda)*alpha;
    set_a(i, j, new_a);
    return new_a;
  }

  // Flip vector pointers 
  inline void
  flip(vector<double> **a, vector<double> **b){
    vector<double> *tmp = *b;
    *b = *a;
    *a = tmp;
  }

  // Return examplar
  inline unsigned int
  get_examplar(unsigned int i){
    return examplar[i];
  }

  inline void
  view_matrix(vector<double> *v, unsigned int N, string name){
    cout << name << endl;
    for(unsigned int i=0; i<N; ++i){
      for(unsigned int j=0; j<N; ++j){
	cout <<(*v)[N*i+j] << "\t";
      }
      cout << endl;
    }
    cout << endl;
  }

  
};

#endif // SCALEAP_H
