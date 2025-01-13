#include "../include/Vehicle.h"

int calc_capacity(const struct Vehicle& vehicle, const Data& data) {

    int cap = 0;

    for (int i = 0; i < vehicle.route.size(); i++)
        cap += data.get_demand(vehicle.route[i]);

    return cap;
}

double calc_cost_vehicle(const struct Vehicle& vehicle, const Data& data) {

    double cost = 0.0;

    for (int i = 0; i < vehicle.route.size()-1; i++)
        cost += data.get_distance(vehicle.route[i], vehicle.route[i+1]);

    return cost;
}