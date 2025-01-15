#ifndef RECREATE_H
#define RECREATE_H

#include <random>
#include "Solution.h"

Solution recreate (Solution , Data& );
void sort_random(vector<int>& , mt19937& );
void sort_demand(vector<int>& , Data& );
void sort_far(vector<int>& , Data& );
void sort_close(vector<int>& , Data& );
double calc_cost(Solution& , Data& , int , int , int );

#endif