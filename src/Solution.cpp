#include "../include/Solution.h"
#include "../include/LocalSearch.h"

#include "../include/Ruin.h"
#include "../include/Recreate.h"
#include "../include/random.h"

#define T0 100
#define Tf 1
#define F 10000

Solution Solution_SA(Solution& sol, Data& data) {
    Solution best_sol = sol;

    int IterT = 0;
    double T = T0;

    while (T > Tf) {

        int SAmax = it_v(data.get_dimension());
        while (IterT < SAmax) {

            IterT++;

            Solution solution = LocalSearch(sol, data);

            double DELTA = solution.cost - sol.cost;

            if (DELTA < 0) {
                sol = solution;
                if (solution.cost < best_sol.cost) {
                    best_sol = solution;
                }
            } else {
                double x = Random::getReal(0, 1);
                if (x < exp(-DELTA/T)) {
                    sol = solution;
                }
            }
        }

        double c = pow((Tf/T0), (1/F));
        T *= c;
        IterT = 0;
    }

    return best_sol;
}

int it_v(int v) {
    return static_cast<int>(std::round(3e7 + ((v - 100) * 2.7e8 / 900)));;
}

Solution ruin_recreate(Solution& sol, Data& data) {

    Solution solution = ruin(sol, data);
    solution = recreate(solution, data);
    update_absC(solution);
    return solution;
}

void update_absC(Solution& sol) {
    for (int i = 0; i < sol.abs_costumers.size(); i++) {
        sol.absC[sol.abs_costumers[i]-1]++;
    }
}

Solution Construction(Data& data) { // Cria uma solucao inicial com um veiculo para cada cliente
    Solution sol;
    sol.costumer_to_vehicle = vector<int>(data.get_dimension(), -1);
    sol.absC = vector<int>(data.get_dimension(), 0);

    for (int i = 2; i <= data.get_dimension(); i++) {
        sol.abs_costumers.push_back(i);
    }

    sol = recreate(sol, data);

    return sol;
}

double calc_cost_solution(const struct Solution& solution, const Data &data) {

    double cost = 0.0;

    for (int i = 0; i < solution.vehicles.size(); i++)
        cost += calc_cost_vehicle(solution.vehicles[i], data);

    return cost;
}