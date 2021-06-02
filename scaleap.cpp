#include "scaleap.hpp"

using namespace std;

ScaleAP::ScaleAP(){}

ScaleAP::ScaleAP(Data *d, float l, unsigned int t, unsigned int s){
  data = d;
  lambda = l;
  max_itr = t;
  num_itr =0;
  stop = s;

  unsigned int size = data->get_size();
  unsigned int N = data->get_N();
  res1.resize(size, 0);
  res2.resize(size, 0);
  ava1.resize(size, 0);
  ava2.resize(size, 0);

  res_plus_ava.resize(size, 0);
  examplar.resize(N, INT_MAX);
  max_index.resize(N, 0);
  max_res.resize(N, -DBL_MAX);
  
  res_prev = &res1;
  res = &res2;  
  ava_prev = &ava1;
  ava = &ava2;

  for(unsigned int i=0; i<N; ++i){
    double tmp_max_sim = -DBL_MAX;
    double tmp_max_index = 0;
      
    for(unsigned int j=0; j<N; ++j){
      (*res_prev)[N*i+j] = data->sim(i, j) - data->max_sim(i, j);
      
      if((*res_prev)[N*i+j] > tmp_max_sim){
	tmp_max_sim = (*res_prev)[N*i+j];
	tmp_max_index = j;
      }
    }

    max_index[i] = tmp_max_index;
  }

}

void
ScaleAP::run(){
  unsigned int stable_count = 0;
  unsigned int prev_updates = 0;
  unsigned int updates = 0;
  
  for(num_itr=0; num_itr<max_itr; ++num_itr){    
    
    updates = one_aggregated_iteration();
    updates == prev_updates || updates == 0 ? ++stable_count : stable_count = 0;
    prev_updates = updates;
    
    if(stable_count >= stop){
      break;
    }
    
  }
}

unsigned int
ScaleAP::one_aggregated_iteration(){
  unsigned int N = data->get_N();
  unsigned int min_k = 0, max_k = 0;
  double min_r = 0, max_r = 0;
  
  // compute aggregated responsibilities
  for(unsigned int i=0; i<N; ++i){    

    // compute r_t(i, i)
    set_r(i,i, get_r(i,i));
    
    // compute r_t(i, min_k)
    min_k = data->min_index[i];
    min_r = update_r(i, min_k);

    // compute r_t(i, max_k)
    max_k = max_index[i];
    max_r = -DBL_MAX;
    if(max_k == i){
      max_r = get_r(i, i);
    }else if(max_k != min_k){
      max_r = update_r(i, max_k);
    }

    // exemplar computation
    for(unsigned int j=0; j<N; ++j){
      if(j != max_k && j != min_k && j != i){
	if(max_k != min_k){
	  set_r(i, j, min_r + (1-lambda)*(data->delta_sim_vector[N*i+j]) + lambda*(get_r(i,j) - get_r(i,min_k)));
	}else{
	  update_r(i,j);
	}
      }
      res_plus_ava[N*i+j] = (*res)[N*i+j];
    }

  }

  
  // compute availabilities
  for(unsigned int i=0; i<N; ++i){
    double tmp_max = -DBL_MAX;
    unsigned int tmp_max_index = 0;

    for(unsigned int j=0; j<N; ++j){
      double tmp_ava = 0;
      
      if(max_res[j]<0 && i!=j){
	tmp_ava = (1-pow(lambda, (num_itr+1)))*get_r(j,j);
	set_a(i, j, tmp_ava);
      }else{
	tmp_ava = update_a(i, j);
      }

      res_plus_ava[N*i+j] += tmp_ava;
      
      // obtain max similarity for responsibility
      if(data->sim(i,j)+tmp_ava > tmp_max){
	tmp_max = data->sim(i,j)+tmp_ava;
	tmp_max_index = j;
      }

    }
    
    max_index[i] = tmp_max_index;
  }


  // find examplars
  vector<double>::iterator it = res_plus_ava.begin();
  unsigned int updates = 0;
  
  for(unsigned int i=0; i<N; ++i){
    unsigned int object = max_element(it+N*i, it+N*(i+1)) - (it+N*i);
    if(examplar[i] != object){
      examplar[i] = object;
      ++updates;
    }
    max_res[i] = -DBL_MAX;
  }

  flip(&res_prev, &res);  
  flip(&ava_prev, &ava);
  
  return updates;
}
