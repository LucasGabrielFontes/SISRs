#include "../include/Ruin.h"
#include "../include/random.h"
#include <algorithm>

#define C_ 10
#define L_MAX 10

#define ALPHA 0.01

/*

lsMax: cardinalidade maxima de cada string removida (todas sao limitadas a isso)

ksMax: quantidade maxima de strings a serem removidas

ks: quantidade de strings que serao removidas

csSeed: cliente aleatorio

R: conjunto de tour arruinados

ltMax: cardinalidade maxima da string a ser removida no tour/veiculo t

lt: cardinalidade da string a ser removida no tuor t

*/

Solution ruin(Solution sol, Data& data) {

    double lsMax = min(static_cast<double>(L_MAX), static_cast<double>(data.get_dimension())/sol.vehicles.size()); 

    double ksMax = ((4*C_)/(1+lsMax))-1;

    int ks = floor(Random::getReal(1, ksMax + 1));

    int csSeed = Random::getInt(2, data.get_dimension()); // O proprio cliente, nao o indice

    // cout << "Adjacencia de " << csSeed << ": " << endl;

    // for (int i = 1; i < data.get_dimension(); i++) {
    //     cout << data.get_adj(csSeed, i) << endl;
    // }

    // cout << "\nDistancias: " << endl;
    // for (int i = 1; i < data.get_dimension(); i++) {
    //     cout << data.get_distance(csSeed, data.get_adj(csSeed, i)) << endl;
    // }

    vector<int> R = {};

    for (int i = 1; i <= data.get_dimension() && R.size() < ks; i++) { // get_adj necessita assim

        if (!belongsTo(data.get_adj(csSeed, i), sol.abs_costumers)
            && !belongsTo(sol.costumer_to_vehicle[data.get_adj(csSeed, i)], R)) {

            int ct_star = data.get_adj(csSeed, i); // ct_star eh o proprio cliente, nao o indice

            double ltMax = min(static_cast<double>(sol.vehicles[sol.costumer_to_vehicle[ct_star]].route.size()), lsMax);

            int lt = floor(Random::getReal(1, ltMax + 1));

            int rd = Random::getInt(0, 0);

            if (rd == 0)
                remove_string(sol, data, sol.costumer_to_vehicle[ct_star-1], lt, ct_star); // costumer_to_vehicle funciona assim
            // else 
            //     remove_split_string(sol, data, sol.costumer_to_vehicle[ct_star-1], lt, ct_star);

            R.push_back(sol.costumer_to_vehicle[ct_star]);
        }
    }

    return sol;
}

void remove_string(Solution &sol, Data& data, int tour, int size_string, int costumer_remove) {

    auto& route = sol.vehicles[tour].route;

    auto it = find(route.begin(), route.end(), costumer_remove); // Iterador para o local onde o costumer_remove esta
    int ind = (it != route.end()) ? std::distance(route.begin(), it) : -1; // Indice do costumer_remove em sua rota

    if (ind == -1) {
        cout << "Erro na funcao remove_string" << endl;
        exit(1);
    }

    int size_block1;
    int size_block2;

    // Tamanho do bloco que vem antes do costumer_remove
    if (ind - size_string > 0) {
        size_block1 = Random::getInt(0, size_string - 1);
    } else {
        size_block1 = Random::getInt(0, ind - 1);
    }

    size_block2 = size_string - size_block1 - 1; // Quando size_block1 = 0, tem um pequeno detalhe

    if (ind + size_block2 >= route.size()-1) { // Rearranja os tamanhos
        while ((ind + size_block2 >= route.size()-1) && (ind - size_block1 - 1 > 0)) {
            size_block1++;
            size_block2--;
        }
    }

    if (ind + size_block2 >= route.size()-1) { // Tamanho do bloco 2 tem que ser 0
        size_block2 = 0;
    }

    const int b = ind - size_block1;
    const int e = ind + size_block2;

    // Atualiza a lista de clientes ausentes da solucao
    // Atualiza a lista costumer_to_vehicle da solucao
    // Atualiza a capacidade usada de cada veiculo que teve sua rota modificada
    auto& vehicle = sol.vehicles[tour];
    for (int i = b; i <= e; i++) {
        int costumer = route[i];
        sol.abs_costumers.push_back(costumer);
        sol.costumer_to_vehicle[costumer-1] = -1;
        vehicle.capacity_used -= data.get_demand(costumer);
    }

    // Atualiza os custos da solucao e dos veiculos que tiveram suas rotas modificadas
    for (int i = b-1; i < e+1; i++) {
        int costumer = route[i];
        int costumer_next = route[i+1];
        double removal_cost = data.get_distance(costumer, costumer_next);
        sol.cost -= removal_cost;
        vehicle.cost -= removal_cost;
    }
    sol.cost += data.get_distance(route[b-1], route[e+1]);
    vehicle.cost += data.get_distance(route[b-1], route[e+1]);

    route.erase(sol.vehicles[tour].route.begin() + b, route.begin() + e + 1);

    if (route.size() == 2) { // Do deposito para o deposito: 1 -> 1
        vehicle.cost = 0;
    }

    // cout << "Rota: ";
    // for (int i = 0; i < route.size()-1; i++) {
    //     cout << route[i] << " -> ";
    // }
    // cout << route[route.size()-1] << endl;

    // cout << "Cliente que tem que ser removido: " << costumer_remove << endl;
    // cout << "Indice do cliente a ser removido: " << ind << endl;
    // cout << "Tamanho da string: " << size_string << endl;
    // cout << "Tamanho do primeiro bloco: " << size_block1 << endl;
    // cout << "Tamanho do segundo bloco: " << size_block2 << endl << endl;
}

bool belongsTo(int obj, vector<int> strc) {
    return (
        !(find(strc.begin(), strc.end(), obj) == strc.end())
    );
}