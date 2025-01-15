#include "../include/Solution.h"
#include "../include/LocalSearch.h"

#include "../include/Recreate.h"

Solution Solution_Sisrs(Data& data) {
    Solution best_sol = Construction(data);

    LocalSearch(best_sol, data);

    return best_sol;
}

Solution Construction(Data& data) { // Cria uma solucao inicial com um veiculo para cada cliente
    Solution sol;
    sol.costumer_to_vehicle = vector<int>(data.get_dimension(), -1);

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