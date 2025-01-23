#ifndef RUIN_H
#define RUIN_H

#include "Solution.h"

Solution ruin(Solution , const Data& );
void remove_split_string(Solution & , const Data& , int , int , int );
void remove_string(Solution & , const Data& , int , int , int );
void definesBlockSize(int , int , int& , int &, vector<int>& );
void details_string(vector<int>& , int , int , int , int , int );
void details_split_string(vector<int>& , int , int , int , int , int , int , int , int , int , int , int );

#endif