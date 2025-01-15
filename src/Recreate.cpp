#include "../include/Recreate.h"
#include "../include/random.h"
#include <algorithm>

#define EP 1e-9
#define BETA 0.01

Solution recreate (Solution sol, Data& data) {

    random_device rd;   // Gerador de números aleatórios
    mt19937 gen(rd());  // Mersenne Twister com seed aleatória

    // Distribuição DISCRETA com pesos: A=4, B=4, C=2, D=1
    std::discrete_distribution<> dist({4, 4, 2, 1});

    // Retorna um tipo de sort com base nos pesos determinados previamente
    int st = dist(gen);

    switch (st) {
        case 0: 
            sort_random(sol.abs_costumers, gen);
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

    int cont = 0;

    for (size_t i = 0; i < sol.abs_costumers.size(); ) { // Nao precisa incrementar i por causa do erase
        int c = sol.abs_costumers[i];

        pair<int, int> P = {-1, -1}; 
        pair<int, int> comp = {-1, -1};

        // P.first : veículo em que c será inserido
        // P.second : posição em que c será inserido na rota de P.first

        vector<int> vehicle_indx(sol.vehicles.size());
        for (int i = 0; i < sol.vehicles.size(); i++) {
            vehicle_indx[i] = i;
        }
        shuffle(vehicle_indx.begin(), vehicle_indx.end(), gen);  // Embaralha os índices para o proximo ser ser aleatorio

        for (int t : vehicle_indx) {
            if (sol.vehicles[t].capacity_used + data.get_demand(c) <= data.get_capacity()) {
                for (int pos = 0; pos < sol.vehicles[t].route.size() - 1; pos++) {
                    if (Random::getReal(0 + EP, 1 - EP) < 1 - BETA) {
                        if (P == comp || 
                            (calc_cost(sol, data, t, pos, c) < calc_cost(sol, data, P.first, P.second, c))) {
                            P = {t, pos};
                        }
                    }
                }
            }
        }

        // Atualiza as informações da solucao e dos veiculos
        if (P == comp) {  // Não encontrou um ponto para inserir
            Vehicle v;
            v.capacity_used += data.get_demand(c);
            v.cost += data.get_distance(data.get_depot(), c) + data.get_distance(c, data.get_depot());
            v.route = {data.get_depot(), c, data.get_depot()};
            sol.cost += v.cost;
            sol.vehicles.push_back(v);
            sol.costumer_to_vehicle[c - 1] = sol.vehicles.size()-1;  // Associar o cliente ao novo veículo
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

double calc_cost(Solution& sol, Data& data, int vehicle_index, int pos, int customer) {
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

void sort_random(vector<int>& abs, mt19937& gen) {
    shuffle(abs.begin(), abs.end(), gen);
}

void sort_demand(vector<int>& abs, Data& data) {
    sort(abs.begin(), abs.end(), [&data](int a, int b){
        return (data.get_demand(a) > data.get_demand(b));
    });
}

void sort_far(vector<int>& abs, Data& data) {
    sort(abs.begin(), abs.end(), [&data](int a, int b){
        return (data.get_distance(a, data.get_depot()) > data.get_distance(b, data.get_depot()));
    });
}

void sort_close(vector<int>& abs, Data& data) {
    sort(abs.begin(), abs.end(), [&data](int a, int b){
        return (data.get_distance(a, data.get_depot()) < data.get_distance(b, data.get_depot()));
    });
}
