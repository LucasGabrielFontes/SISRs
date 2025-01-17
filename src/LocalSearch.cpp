#include "../include/LocalSearch.h"
#include "../include/random.h"
#include "../include/Solution.h"
#include "../include/Ruin.h"
#include "../include/Recreate.h"
#include <cmath>
#include <algorithm>

#define EP 1e-9
#define T0 100
#define Tf 1

Solution LocalSearch(Solution& solution, Data& data) { // struct

    int F = 10000; //it_v(data.get_dimension());

    Solution best_sol = solution;
    double T = T0;
    for (int i = 0; i < F; i++) {

        Solution solution_star = ruin_recreate(solution, data);
        if (solution_star.cost < solution.cost - T*log(Random::getReal(0+EP, 1-EP))) {
            solution = solution_star;
        }
        if (solution_star.cost < best_sol.cost) {
            best_sol = solution_star;
        }

        double c = pow((Tf/T0), (1/F));

        T = c*T;
    }
    return best_sol;
}

int it_v(int v) {
    return static_cast<int>(std::round(3e7 + ((v - 100) * 2.7e8 / 900)));
}