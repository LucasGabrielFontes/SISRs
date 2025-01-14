#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "Solution.h"

void LocalSearch(Solution& , const Data& );
Solution ruin(Solution , Data& );
vector<int> get_customers_not_abs(Data& , Solution& );

#endif