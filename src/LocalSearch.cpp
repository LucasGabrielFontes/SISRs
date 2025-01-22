#include "../include/LocalSearch.h"
#include "../include/random.h"
#include "../include/Solution.h"
#include "../include/Ruin.h"
#include "../include/Recreate.h"
#include <cmath>
#include <algorithm>

Solution LocalSearch(Solution& solution, Data& data) { // struct

    double T0 = 100.0;
    double Tf = 1.0;
    double EP = 1e-6;

    int F = it_v(data.get_dimension());
    double c = pow((Tf/T0), (1.0/(double)F)); // O erro tava aqui, rapaz, imoral

    c -= EP;

    Solution best_sol = solution;
    double T = T0;
    for (int i = 0; i < F; i++) {

        Solution solution_star = ruin_recreate(solution, data);
        if (solution_star.cost < solution.cost - T*log(Random::getReal(0, 1))) {
            solution = solution_star;
        }
        if (solution_star.cost < best_sol.cost) {
            best_sol = solution_star;
        }

        T *= c;
    }
    return best_sol;
}

int it_v(int v) {
    return 1e6;
}