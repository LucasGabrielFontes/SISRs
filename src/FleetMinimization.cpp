#include "../include/FleetMinimization.h"
#include "../include/Solution.h"
#include <climits>
#include "../include/LocalSearch.h"

//////
#define EP 1e-9
#include <math.h>
///////

Solution fleet_minimization(Solution& sol, Data& data) {

    int ITS = it_v(data.get_dimension())/10;
    Solution sBest = sol;

    for (int i = 2; i <= data.get_dimension(); i++) {
        sBest.absC[i-1] = 0;
    }

    for (int i = 0; i < ITS; i++) {

        Solution sStar = sol;
        for (int i = 2; i <= data.get_dimension(); i++) {
            sStar.absC[i-1] = 0;
        }

        sStar = ruin_recreate(sStar, data);

        if (sStar.abs_costumers.size() < sol.abs_costumers.size() || sumAbs(sStar) < sumAbs(sol)) {
            sol = sStar;
        }

        if (sStar.abs_costumers.empty()) {
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
            for (int i = 0; i < data.get_dimension(); i++) {
                if (sStar.costumer_to_vehicle[i] > tuor) {
                    sStar.costumer_to_vehicle[i]--;
                }
            }
        }

        update_absC(sStar);

        // details_solution1(sStar, data);
    }

    return sol;
}

void details_solution1(Solution &sol, Data &data) {

    cout << "Ausentes: ";
    for (int i = 0; i < sol.abs_costumers.size(); i++) {
        cout << sol.abs_costumers[i] << " ";
    }
    cout << endl;

    cout << "Veiculos e suas informacoes: " << endl;
    for (int i = 0; i < sol.vehicles.size(); i++) {
        cout << "Veiculo " << i+1 << ": ";
        for (int j = 0; j < sol.vehicles[i].route.size()-1; j++) {
            cout << sol.vehicles[i].route[j] << " -> ";
        }
        cout << sol.vehicles[i].route[sol.vehicles[i].route.size()-1] << endl;
    }

    cout << endl << endl << "Custo de cada veiculo: "<< endl;
    for (int i = 0; i < sol.vehicles.size(); i++) {
        cout << "Veiculo " << i+1 << ": " << endl;
        double custoTrue = sol.vehicles[i].cost;
        double custo = calc_cost_vehicle(sol.vehicles[i], data);
        if (fabs(custoTrue - custo) < EP) {
            cout << "Custos iguais" << endl;
        } else {
            cout << "Custo diferentes. Ai eh triste AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
        }
        cout << "Custo: " << custo << endl;
        cout << "Custo que era pra ser: " << custo << endl << endl;
    }
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