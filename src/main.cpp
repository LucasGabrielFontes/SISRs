#include "../include/Data.h"
#include "../include/random.h"
#include <time.h>

#include "../include/Solution.h"
#include "../include/LocalSearch.h"
#include "../include/Ruin.h"
#include "../include/Recreate.h"

#include "../include/FleetMinimization.h"

#define EP 1e-9

void details_solution(Solution & , Data & );

int main(int argc, char** argv) {

    unsigned int seed = time(NULL);
    Random::randomize(seed);

    auto data = Data(argc, argv[1]);
    data.read_instance();

    Solution sol = Construction(data);

    sol = ruin_recreate(sol, data);

    sol = fleet_minimization(sol, data);

    cout << "Solucao eita: " << endl << endl;
    details_solution(sol, data);

    return 0;
}

void details_solution(Solution &sol, Data &data) {

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
