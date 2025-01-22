#include <algorithm>
#include "../include/Recreate.h"
#include "../include/random.h"

#define BETA 0.01

Solution recreate (Solution sol, const Data& data) {

    std::mt19937_64& generator = Random::gen();

    // Distribuição DISCRETA com pesos: 4, 4, 2, 1, com descrito no artigo
    std::discrete_distribution<> dist({4, 4, 2, 1});

    // Determina o tipo de sort aleatorio a ser realizado com base nos pesos determinados previamente
    int st = dist(generator);

    switch (st) {
        case 0: 
            sort_random(sol.abs_costumers, generator);
            break;
        case 1: 
            sort_demand(sol.abs_costumers, data);
            break;
        case 2: 
            sort_far(sol.abs_costumers, data);
            break;
        case 3: 
            sort_close(sol.abs_costumers, data);
            break;
    }

    for (size_t i = 0; i < sol.abs_costumers.size(); ) { // Nao precisa incrementar i por causa do erase
        int c = sol.abs_costumers[i];

        pair<int, int> P = {-1, -1}; 
        double insert_costP = std::numeric_limits<double>::infinity();
        bool found_place = false;

        int vehicles_size = sol.vehicles.size();
        for (int t = 0; t < vehicles_size; t++) {
            auto& vehicle = sol.vehicles[t];
            if (vehicle.capacity_used + data.get_demand(c) <= data.get_capacity()) {
                int route_size = vehicle.route.size();
                for (int pos = 0; pos < route_size - 1; pos++) {
                    if (Random::getReal(0, 1) < 1 - BETA) {
                        double insert_costPos = calc_cost(sol, data, t, pos, c);
                        if (!found_place || 
                            (insert_costPos < insert_costP)) {
                            P = {t, pos};
                            insert_costP = insert_costPos;
                            found_place = true;
                        }
                    }
                }
            }
        }

        // Atualiza as informações da solucao e dos veiculos
        if (!found_place) {  // Não encontrou um ponto para inserir
            Vehicle v;
            v.capacity_used += data.get_demand(c);
            v.cost += 2*data.get_distance(data.get_depot(), c);
            v.route = {data.get_depot(), c, data.get_depot()};
            sol.cost += v.cost;
            sol.vehicles.push_back(v);
            sol.costumer_to_vehicle[c - 1] = sol.vehicles.size()-1;  // Associar o cliente ao novo veículo (retorna o indice)
        } else {
            sol.cost += calc_cost(sol, data, P.first, P.second, c);
            sol.costumer_to_vehicle[c - 1] = P.first;
            sol.vehicles[P.first].cost += calc_cost(sol, data, P.first, P.second, c);
            sol.vehicles[P.first].route.insert(sol.vehicles[P.first].route.begin() + P.second + 1, c);
            sol.vehicles[P.first].capacity_used += data.get_demand(c);
        }

        // Remove o cliente c da lista abs_costumers
        sol.abs_costumers.erase(sol.abs_costumers.begin() + i);
    }

    return sol;
}

double calc_cost(Solution& sol, const Data& data, int vehicle_index, int pos, int customer) {
    const auto& route = sol.vehicles[vehicle_index].route;

    if (pos < 0 || pos >= route.size() - 1) {
        cout << "Posição de inserção inválida!" << endl;
        return numeric_limits<double>::infinity();  // Retorna um custo infinito se a posição for invalida
    }

    int prev_customer = route[pos];  // Cliente anterior ou deposito
    int next_customer = route[pos+1];  // Cliente posterior ou deposito

    double insertion_cost = data.get_distance(prev_customer, customer) +
                            data.get_distance(customer, next_customer) -
                            data.get_distance(prev_customer, next_customer);

    return insertion_cost;
}

void sort_random(vector<int>& abs, mt19937_64& gen) {
    shuffle(abs.begin(), abs.end(), gen);
}

void sort_demand(vector<int>& abs, const Data& data) {
    sort(abs.begin(), abs.end(), [&data](int a, int b){
        return (data.get_demand(a) > data.get_demand(b));
    });
}

void sort_far(vector<int>& abs, const Data& data) {
    sort(abs.begin(), abs.end(), [&data](int a, int b){
        return (data.get_distance(a, data.get_depot()) > data.get_distance(b, data.get_depot()));
    });
}

void sort_close(vector<int>& abs, const Data& data) {
    sort(abs.begin(), abs.end(), [&data](int a, int b){
        return (data.get_distance(a, data.get_depot()) < data.get_distance(b, data.get_depot()));
    });
}