#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "Solution.h"

void LocalSearch(Solution& , const Data& );
Solution ruin(Solution , Data& );
void remove_string(Solution & , Data& , int , int , int );
void remove_split_string(Solution & , Data& , int , int , int );

#endif