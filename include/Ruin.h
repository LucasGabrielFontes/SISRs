#ifndef RUIN_H
#define RUIN_H

#include "Solution.h"

Solution ruin(Solution , Data& );
void remove_split_string(Solution & , Data& , int , int , int );
void remove_string(Solution & , Data& , int , int , int );
bool belongsTo(int , vector<int>);
void definesBlockSize(int , int , int& , int &, vector<int>& );
void details_string(vector<int>& , int , int , int , int , int );
void details_split_string(vector<int>& , int , int , int , int , int , int , int , int , int , int , int );

void details_solution2(Solution & , Data & );

#endif