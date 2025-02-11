#include <time.h>
#include <chrono>
#include "../include/Data.h"
#include "../include/random.h"
#include "../include/Solution.h"

#define EP 1e-6

void details_solution(Solution & , Data & );

int main(int argc, char** argv) {

    unsigned int seed = time(NULL);
    Random::randomize(seed);

    auto data = Data(argc, argv[1]);
    data.read_instance();

    auto begin = chrono::high_resolution_clock::now();
    Solution sol = Solution_SA(data);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time = end - begin;

    cout << "Solucao: " << endl << endl;
    details_solution(sol, data);
    cout << "\nTempo de execucao: " << time.count() << endl;

    return 0;
}

void details_solution(Solution &sol, Data &data) {

    cout << "Ausentes: ";
    if (sol.abs_costumers.size() == 0)
        cout << "Nao ha clientes ausentes na solucao." << endl << endl;
    else {
        for (int i = 0; i < sol.abs_costumers.size(); i++)
            cout << sol.abs_costumers[i] << " ";
        cout << endl << endl;
    }

    int vehicles_size = sol.vehicles.size();
    cout << "Veiculos e suas informacoes: " << endl << endl;
    for (int i = 0; i < vehicles_size; i++) {
        cout << "Veiculo " << i+1 << ": " << endl;
        cout << "Rota: ";
        vector<int>& route = sol.vehicles[i].route;
        int route_size = route.size();
        for (int j = 0; j < route_size - 1; j++) {
            cout << route[j] << " -> ";
        }
        cout << route[route_size-1] << endl;
        double custoTrue = sol.vehicles[i].cost;
        double custo = calc_cost_vehicle(sol.vehicles[i], data);
        cout << "Custo: " << custoTrue << endl;
        cout << "Custo correto: " << custo << endl;
        if (fabs(custoTrue - custo) < EP) {
            cout << "Custos iguais" << endl;
        } else {
            cout << "Custo diferentes. AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl;
        }
        cout << "Capacidade usada: " << sol.vehicles[i].capacity_used << endl;
        cout << "Capacidade maxima: " << data.get_capacity() << endl << endl;
    }

    double custoTrue = sol.cost;
    double custo = calc_cost_solution(sol, data);
    cout << "Custo da solucao: " << custoTrue << endl;
    cout << "Custo correto: " << custo << endl;
    if (fabs(custoTrue - custo) < EP) {
        cout << "Custos iguais" << endl;
    } else {
        cout << "Custo diferentes. AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << endl << endl;
    }

    cout << "Quantidade de veiculos: " << sol.vehicles.size() << endl << endl;
}
