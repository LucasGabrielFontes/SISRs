#ifndef RECREATE_H
#define RECREATE_H

#include <random>
#include "Solution.h"

Solution recreate (Solution , const Data& );
void sort_random(vector<int>& , mt19937_64& );
void sort_demand(vector<int>& , const Data& );
void sort_far(vector<int>& , const Data& );
void sort_close(vector<int>& , const Data& );
double calc_cost(Solution& , const Data& , int , int , int );

#endif