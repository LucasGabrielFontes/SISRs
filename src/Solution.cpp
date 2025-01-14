#include "../include/Solution.h"
#include "../include/LocalSearch.h"

Solution Solution_Sisrs(Data& data) {
    Solution best_sol = Construction(data);

    LocalSearch(best_sol, data);

    return best_sol;
}

Solution Construction(Data& data) { // Cria uma solucao inicial com um veiculo para cada cliente
    Solution sol;
    sol.costumer_to_vehicle = vector<int>(data.get_dimension(), -1);

    for (int i = 2; i <= data.get_dimension(); i++) {
        Vehicle vehicle;
        vehicle.route = {};

        if (data.get_capacity() < data.get_demand(i)) {
            cout << "A demanda de um cliente eh maior que a capacidade dos veiculos" << endl;
            exit(1);
        }

        vehicle.route = {data.get_depot(), i, data.get_depot()};
        vehicle.cost = data.get_distance(data.get_depot(), i) + data.get_distance(i, data.get_depot());
        vehicle.capacity_used = data.get_demand(i);

        sol.vehicles.emplace_back(vehicle);
        sol.costumer_to_vehicle[i-1] = i-1; // Adiciona o cliente i ao veiculo i-1
        sol.cost += vehicle.cost; 
    }

    return sol;
}

double calc_cost_solution(const struct Solution& solution, const Data &data) {

    double cost = 0.0;

    for (int i = 0; i < solution.vehicles.size(); i++)
        cost += calc_cost_vehicle(solution.vehicles[i], data);

    return cost;
}