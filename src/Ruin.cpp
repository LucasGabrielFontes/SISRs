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

    double lsMax = min(static_cast<double>(L_MAX), static_cast<double>(data.get_dimension()-1)/sol.vehicles.size()); 

    double ksMax = ((4*C_)/(1+lsMax))-1;

    int ks = floor(Random::getReal(1, ksMax + 1));

    int csSeed = Random::getInt(2, data.get_dimension()); // O proprio cliente, nao o indice

    vector<bool> tourRuined(sol.vehicles.size(), false);

    int cont = 1;
    int strings = 0;
    while (cont <= data.get_dimension() && strings < ks) {

        int ct_star = data.get_adj(csSeed, cont);
        int indTour = sol.costumer_to_vehicle[ct_star-1];
        if (sol.costumer_to_vehicle[ct_star-1] != -1 && tourRuined[indTour] == false) {

            double ltMax = min(static_cast<double>(sol.vehicles[indTour].route.size()), lsMax);

            int lt = floor(Random::getReal(1, ltMax + 1));

            int rd = Random::getInt(0, 1);

            if (rd == 0 || lt == 2)
                remove_string(sol, data, indTour, lt, ct_star); // costumer_to_vehicle funciona assim
            else 
                remove_split_string(sol, data, indTour, lt, ct_star);
        
            strings++;
            tourRuined[indTour] = true;
        }

        cont++;
    }

    for (int i = sol.vehicles.size()-1; i >= 0; i--) {

        if (sol.vehicles[i].route.size() == 2) { // Do deposito para o deposito: 1 -> 1
            // Retira o veiculo
            sol.vehicles.erase(sol.vehicles.begin() + i);
            for (int j = 0; j < data.get_dimension(); j++) {
                if (sol.costumer_to_vehicle[j] > i) {
                    sol.costumer_to_vehicle[j]--;
                }
            }
        }
    }

    return sol;
}

void remove_split_string(Solution &sol, Data& data, int tour, int size_string, int costumer_remove) {

    auto& route = sol.vehicles[tour].route;

    int m = 1;
    int rd = Random::getReal(0, 1);
    while (!(rd < ALPHA || m == sol.vehicles[tour].route.size() - size_string)) {
        m += 1;
        rd = Random::getReal(0, 1);
    }
    size_string += m;

    auto it = find(route.begin(), route.end(), costumer_remove); // Iterador para o local onde o costumer_remove esta
    int ind = (it != route.end()) ? std::distance(route.begin(), it) : -1; // Indice do costumer_remove em sua rota

    if (ind == -1) {
        cout << "Erro na funcao remove_string" << endl;
        exit(1);
    }
    int size_block1;
    int size_block2;
    definesBlockSize(ind, size_string, size_block1, size_block2, route);

    int comp1 = ind - size_block1 + 1;
    int comp2 = ind + size_block2 - m;
    int ind_stay = -1;

    if (comp2 < comp1)
        ind_stay = ind;
    else 
        ind_stay = Random::getInt(comp1, comp2); // Take care

    int b1 = ind - size_block1;
    int e1 = ind_stay - 1;
    int b2 = ind_stay + m;
    int e2 = ind + size_block2; 

    if (b1 > e1) {
        b1 = -1;
        e1 = -1;
    }

    if (b2 > e2) {
        b2 = -1;
        e2 = -1;
    }   

    // Atualiza a lista de clientes ausentes da solucao
    // Atualiza a lista costumer_to_vehicle da solucao
    // Atualiza a capacidade usada de cada veiculo que teve sua rota modificada
    auto& vehicle = sol.vehicles[tour];
    for (int i = b1; i <= e1 && i >= 0; i++) {
        int costumer = route[i];
        sol.abs_costumers.push_back(costumer);
        sol.costumer_to_vehicle[costumer-1] = -1;
        vehicle.capacity_used -= data.get_demand(costumer);
    }
    for (int i = b2; i <= e2 && i >= 0; i++) {
        int costumer = route[i];
        sol.abs_costumers.push_back(costumer);
        sol.costumer_to_vehicle[costumer-1] = -1;
        vehicle.capacity_used -= data.get_demand(costumer);
    }

    // Atualiza os custos da solucao e dos veiculos que tiveram suas rotas modificadas
    for (int i = b1-1; i < e1+1 && i >= 0; i++) {
        int costumer = route[i];
        int costumer_next = route[i+1];
        double removal_cost = data.get_distance(costumer, costumer_next);
        sol.cost -= removal_cost;
        vehicle.cost -= removal_cost;
    }
    if (b1 > 0 && e1 > 0) {
        sol.cost += data.get_distance(route[b1-1], route[e1+1]);
        vehicle.cost += data.get_distance(route[b1-1], route[e1+1]);
    }

    for (int i = b2-1; i < e2+1 && i >= 0; i++) {
        int costumer = route[i];
        int costumer_next = route[i+1];
        double removal_cost = data.get_distance(costumer, costumer_next);
        sol.cost -= removal_cost;
        vehicle.cost -= removal_cost;
    }
    if (b2 > 0 && e2 > 0) {
        sol.cost += data.get_distance(route[b2-1], route[e2+1]);
        vehicle.cost += data.get_distance(route[b2-1], route[e2+1]);
    }

    route.erase(route.begin() + ind_stay + m, route.begin() + ind + size_block2 + 1);
    route.erase(route.begin() + ind - size_block1, route.begin() + ind_stay);
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

    definesBlockSize(ind, size_string, size_block1, size_block2, route);

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
}

void definesBlockSize(int ind, int size_string, int& size_block1, int &size_block2, vector<int>& route) {
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
}

void details_split_string(vector<int>& route, int ind, int costumer_remove, int size_string, int size_block1, int size_block2, int ind_stay, int b1, int e1, int b2, int e2, int m) {

    cout << "Rota: ";
    for (int i = 0; i < route.size()-1; i++) {
        cout << route[i] << " -> ";
    }
    cout << route[route.size()-1] << endl;

    cout << "Cliente que tem que ser removido: " << costumer_remove << endl;
    cout << "Indice do cliente a ser removido: " << ind << endl;
    cout << "Tamanho da string: " << size_string << endl;
    cout << "Tamanho do m: " << m << endl;
    cout << "ind_stay: " << ind_stay << endl;
    cout << "Tamanho do primeiro bloco: " << size_block1 << endl;
    cout << "Tamanho do segundo bloco: " << size_block2 << endl;
    cout << b1 << " " << e1 << "  " << b2 << " " << e2 << endl << endl;
}

void details_string(vector<int>& route, int ind, int costumer_remove, int size_string, int size_block1, int size_block2) {

    cout << "Rota: ";
    for (int i = 0; i < route.size()-1; i++) {
        cout << route[i] << " -> ";
    }
    cout << route[route.size()-1] << endl;

    cout << "Cliente que tem que ser removido: " << costumer_remove << endl;
    cout << "Indice do cliente a ser removido: " << ind << endl;
    cout << "Tamanho da string: " << size_string << endl;
    cout << "Tamanho do primeiro bloco: " << size_block1 << endl;
    cout << "Tamanho do segundo bloco: " << size_block2 << endl << endl;
}
