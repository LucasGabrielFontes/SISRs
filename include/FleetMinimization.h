#ifndef FLEET_MINIMIZATION_H
#define FLEET_MINIMIZATION_H

#include "Solution.h"

Solution fleet_minimization(Solution& , Data& );
int sumAbs(const Solution& );
int sumAbsTuor(const Solution& , int );
void update_absC(Solution& );

void details_solution1(Solution & , Data & );

#endif