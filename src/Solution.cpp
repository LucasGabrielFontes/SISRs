#include "../include/Solution.h"

double calc_cost_solution(const struct Solution& solution, const Data &data) {

    double cost = 0.0;

    for (int i = 0; i < solution.vehicles.size(); i++) {
        cost += calc_cost_vehicle(solution.vehicles[i], data);
    }

    return cost;
}