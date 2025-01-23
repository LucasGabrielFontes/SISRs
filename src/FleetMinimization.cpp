#include <climits>
#include "../include/FleetMinimization.h"
#include "../include/Solution.h"
#include "../include/LocalSearch.h"

Solution fleet_minimization(Solution& sol, const Data& data) {

    int dimension = data.get_dimension();
    long long ITS = calculateIterations(dimension)/10;
    Solution sBest = sol;

    for (int i = 2; i <= dimension; i++)
        sBest.absC[i-1] = 0;

    for (long long i = 0; i < ITS; i++) {

        Solution sStar = ruin_recreate(sol, data);
        update_absC(sStar);

        int sStar_abs_c_size = sStar.abs_costumers.size();

        if (sStar_abs_c_size < sol.abs_costumers.size() || sumAbs(sStar) < sumAbs(sol))
            sol = sStar;

        if (sStar_abs_c_size == 0) {
            sBest = sStar;
            int tuor = -1;
            int sum = INT_MAX;
            int vehicles_size = sStar.vehicles.size();
            for (int i = 0; i < vehicles_size; i++) {
                int sum_act = sumAbsTuor(sStar, i);
                if (sum_act < sum) {
                    sum = sum_act;
                    tuor = i;
                }
            }
            int route_size = sStar.vehicles[tuor].route.size();
            vector<int>& route = sStar.vehicles[tuor].route;
            for (int i = 1; i < route_size - 1; i++) {
                sStar.abs_costumers.push_back(route[i]);
            }
            sStar.cost -= sStar.vehicles[tuor].cost;
            sStar.vehicles.erase(sStar.vehicles.begin() + tuor);
            for (int i = 0; i < dimension; i++) {
                if (sStar.costumer_to_vehicle[i] > tuor) {
                    sStar.costumer_to_vehicle[i]--;
                }
            }
        }

        update_absC(sStar);
    }

    return sBest;
}

int sumAbsTuor(const Solution& sol, int tour) {

    int cont = 0;
    int size = sol.vehicles[tour].route.size();
    auto& sol_absC = sol.absC;
    auto& sol_vehi_tour = sol.vehicles[tour];
    for (int i = 0; i < size; i++)
        cont += sol_absC[sol_vehi_tour.route[i]];

    return cont;
}

int sumAbs(const Solution& sol) {

    int cont = 0;
    int size = sol.absC.size();
    for (int i = 0; i < size; i++)
        cont += sol.absC[i];

    return cont;
}