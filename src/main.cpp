#include "../include/Data.h"
#include "../include/random.h"
#include <time.h>

#include "../include/Solution.h"
#include "../include/LocalSearch.h"
#include "../include/Ruin.h"

int main(int argc, char** argv) {

    unsigned int seed = time(NULL);
    Random::randomize(seed);

    auto data = Data(argc, argv[1]);
    data.read_instance();

    Solution sol = Construction(data);

    sol = ruin(sol, data);

    // cout << "Ausentes: ";
    // for (int i = 0; i < sol.abs_costumers.size(); i++) {
    //     cout << sol.abs_costumers[i] << " ";
    // }
    // cout << endl;

    // cout << "Veiculos e suas informacoes: " << endl;
    // for (int i = 0; i < sol.vehicles.size(); i++) {
    //     cout << "Veiculo " << i+1 << ": ";
    //     for (int j = 0; j < sol.vehicles[i].route.size(); j++) {
    //         cout << sol.vehicles[i].route[j] << " -> ";
    //     }
    //     cout << endl;
    // }

    cout << endl << endl << "Custo de cada veiculo: "<< endl;
    for (int i = 0; i < sol.vehicles.size(); i++) {
        cout << "Veiculo " << i+1 << ": " << endl;
        cout << "Custo: " << sol.vehicles[i].cost << endl;
        cout << "Custo que era pra ser: " << calc_cost_vehicle(sol.vehicles[i], data) << endl << endl;
    }

    return 0;
}
