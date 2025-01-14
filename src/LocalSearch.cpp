#include "../include/LocalSearch.h"
#include "../include/random.h"
#include "../include/Solution.h"
#include <cmath>

#define E 1e-9

void LocalSearch(Solution& solution, const Data& data, const double T0, const double Tf, const int f) { // struct

    Solution best_sol = solution;
    double T = T0;
    for (int i = 0; i < f; i++) {

        Solution solution_star; // = ruin_and_recreate(solution);
        if (solution_star.cost < solution.cost - T*log(Random::getReal(0+E, 1-E))) {
            solution = solution_star;
        }
        if (solution_star.cost < best_sol.cost) {
            best_sol = solution_star;
        }

        double c = pow((Tf/T0), (1/f));

        T = c*T;
    }
}