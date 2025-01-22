#include "../include/FleetMinimization.h"
#include "../include/Solution.h"
#include <climits>
#include "../include/LocalSearch.h"

Solution fleet_minimization(Solution& sol, const Data& data) {

    int dimension = data.get_dimension();
    long long ITS = calculateIterations(dimension)/10;
    Solution sBest = sol;

    for (int i = 2; i <= dimension; i++) {
        sBest.absC[i-1] = 0;
    }

    for (long long i = 0; i < ITS; i++) {

        Solution sStar = ruin_recreate(sol, data);
        update_absC(sStar);

        if (sStar.abs_costumers.size() < sol.abs_costumers.size() || sumAbs(sStar) < sumAbs(sol)) {
            sol = sStar;
        }

        if (sStar.abs_costumers.size() == 0) {
            sBest = sStar;
            int tuor = -1;
            int sum = INT_MAX;
            for (int i = 0; i < sStar.vehicles.size(); i++) {
                int sum_act = sumAbsTuor(sStar, i);
                if (sum_act < sum) {
                    sum = sum_act;
                    tuor = i;
                }
            }
            for (int i = 1; i < sStar.vehicles[tuor].route.size() - 1; i++) {
                sStar.abs_costumers.push_back(sStar.vehicles[tuor].route[i]);
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

    return sol;
}

int sumAbsTuor(const Solution& sol, int tour) {

    int cont = 0;
    for (int i = 0; i < sol.vehicles[tour].route.size(); i++)
        cont += sol.absC[sol.vehicles[tour].route[i]];

    return cont;
}

int sumAbs(const Solution& sol) {

    int cont = 0;
    for (int i = 0; i < sol.absC.size(); i++)
        cont += sol.absC[i];

    return cont;
}