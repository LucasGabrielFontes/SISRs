#include "../include/LocalSearch.h"
#include "../include/random.h"
#include "../include/Solution.h"
#include <cmath>
#include <algorithm>

#define EP 1e-9
#define T0 100
#define Tf 1
#define F 10000

void LocalSearch(Solution& solution, Data& data) { // struct

    Solution best_sol = solution;
    double T = T0;
    for (int i = 0; i < F; i++) {

        Solution solution_star = ruin(solution, data);
        solution_star; //= recreate(solution_star);
        if (solution_star.cost < solution.cost - T*log(Random::getReal(0+EP, 1-EP))) {
            solution = solution_star;
        }
        if (solution_star.cost < best_sol.cost) {
            best_sol = solution_star;
        }

        double c = pow((Tf/T0), (1/F));

        T = c*T;
    }
}
