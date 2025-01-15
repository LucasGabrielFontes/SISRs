#include "../include/Ruin.h"
#include "../include/random.h"
#include <algorithm>

#define EP 1e-9
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

    int ks = floor(Random::getReal(1+EP, ksMax + 1 - EP));

    int csSeed = Random::getInt(2, data.get_dimension());

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

        if (find(sol.abs_costumers.begin(), sol.abs_costumers.end(), data.get_adj(csSeed, i)) == sol.abs_costumers.end()
        && find(R.begin(), R.end(), sol.costumer_to_vehicle[data.get_adj(csSeed, i)]) == R.end()) {

            int ct_star = data.get_adj(csSeed, i); // ct_star eh o proprio cliente, nao o indice

            double ltMax = min(static_cast<double>(sol.vehicles[sol.costumer_to_vehicle[ct_star]].route.size()), lsMax);

            int lt = floor(Random::getReal(1 + EP, ltMax + 1 - EP));

            // int rd = Random::getInt(0, 0);

            // if (rd == 0)
                remove_string(sol, data, sol.costumer_to_vehicle[ct_star-1], lt, ct_star); // costumer_to_vehicle funciona assim
            // else 
            //     remove_split_string(sol, data, sol.costumer_to_vehicle[ct_star-1], lt, ct_star);

            R.push_back(sol.costumer_to_vehicle[ct_star]);
        }
    }

    return sol;
}

void remove_string(Solution &sol, Data& data, int tour, int size_string, int costumer_remove) {

    cout << "Costumer remove: " << costumer_remove << endl;
    cout << "Rota do veiculo: ";
    for (int i = 0; i < sol.vehicles[tour].route.size(); i++) {
        cout << sol.vehicles[tour].route[i] << " ";
    }
    cout << endl;

    int size_block1 = Random::getInt(0, size_string - 1); // Tamanho do bloco que vem antes do costumer_remove
    int size_block2 = size_string - size_block1 - 1; // Tamanho do bloco que vem depois do costumer_remove

    auto it = find(sol.vehicles[tour].route.begin(), sol.vehicles[tour].route.end(), costumer_remove); // Iterador para o local onde o costumer_remove esta
    int ind = (it != sol.vehicles[tour].route.end()) ? std::distance(sol.vehicles[tour].route.begin(), it) : -1; // Indice do costumer_remove em sua rota

    if (ind == -1) {
        cout << "Erro na funcao remove_string" << endl;
        exit(1);
    }

    // Talvez nao seja necessario
    if (ind - size_block1 < 0 || ind + size_block2 >= sol.vehicles[tour].route.size()) {
        cout << "Bloco fora dos limites da rota na funcao remove_string." << endl;
        exit(1);
    }

    // Atualiza a lista de veiculos ausentes
    // Atualiza a lista costumer_to_vehicle
    for (int i = ind - size_block1; i <= ind + size_block2; i++) {
        int costumer = sol.vehicles[tour].route[i];
        sol.abs_costumers.push_back(costumer);
        sol.costumer_to_vehicle[costumer-1] = -1;
    }

    int i = ind - size_block1 > 0 ? ind - size_block1 - 1 : 0;
    int f = ind + size_block2 < sol.vehicles[tour].route.size() - 1 ? ind + size_block2 + 1 : ind + size_block2;

    for (int i1 = i+1; i1 < f; i1++) {
        int costumer = sol.vehicles[tour].route[i1];
        sol.abs_costumers.push_back(costumer);
        sol.vehicles[tour].capacity_used -= data.get_demand(costumer);
    }

    // Atualiza o custo da solucao
    // Atualiza o custo individual de cada veiculo
    // Atualiza a demanda utilizada de cada veiculo
    for (; i < f; i++) {
        int costumer = sol.vehicles[tour].route[i];
        sol.cost -= data.get_distance(costumer, costumer+1);
        sol.vehicles[tour].cost -= data.get_distance(costumer, costumer+1);
    }
    sol.cost += data.get_distance(sol.vehicles[tour].route[i],sol.vehicles[tour].route[f]);
    sol.vehicles[tour].cost += data.get_distance(sol.vehicles[tour].route[i],sol.vehicles[tour].route[f]);

    sol.vehicles[tour].route.erase(sol.vehicles[tour].route.begin() + ind - size_block1, sol.vehicles[tour].route.begin() + ind + size_block2 + 1);
}

void remove_split_string(Solution &sol, Data& data, int tour, int size_string, int costumer_remove) {

    cout << "Costumer remove: " << costumer_remove << endl;
    cout << "Rota do veiculo: ";
    for (int i = 0; i < sol.vehicles[tour].route.size(); i++) {
        cout << sol.vehicles[tour].route[i] << " ";
    }
    cout << endl;

    int m = 1;

    int rd = Random::getInt(0+EP, 1-EP);

    while (!(rd < ALPHA || m == sol.vehicles[tour].route.size() - size_string)) {
        m += 1;
        rd = Random::getInt(0+EP, 1-EP);
    }

    size_string += m;

    int size_block1 = Random::getInt(1, size_string - 1); // Tamanho do bloco total que vem antes do cliente central
    int size_block2 = size_string - size_block1 - 1; // Tamanho do bloco total que vem depois do cliente central

    auto it = find(sol.vehicles[tour].route.begin(), sol.vehicles[tour].route.end(), costumer_remove);
    int ind = (it != sol.vehicles[tour].route.end()) ? std::distance(sol.vehicles[tour].route.begin(), it) : -1;

    if (ind == -1) {
        cout << "Erro na funcao remove_split_string" << endl;
        exit(1);
    }

    if (ind - size_block1 < 0) {
        size_block1 = ind;  // Ajusta o bloco anterior para o início da rota
    }

    if (ind + size_block2 >= sol.vehicles[tour].route.size()) {
        size_block2 = sol.vehicles[tour].route.size() - 1 - ind;  // Ajusta o bloco posterior para o final da rota
    }

    int ind_main = Random::getInt(ind - size_block1 + 1, ind + size_block2 - 1); // Indice que indica o inicio da string que permanecera na rota (tamanho m definido acima)
    ind_main = max(ind - size_block1 + 1, min(ind_main, ind + size_block2 - 1));  // Garante que ind_main está no intervalo válido

    // Atualiza a lista de clientes ausentes com base na primeira parte da split string
    // Atualiza a lista costumer_to_vehicle com base na primeira parte da split string
    for (int i = ind - size_block1; i < ind_main; i++) {
        int costumer = sol.vehicles[tour].route[i];
        sol.abs_costumers.push_back(costumer);
        sol.costumer_to_vehicle[costumer-1] = -1;
    }

    int i1 = ind - size_block1 > 0 ? ind - size_block1 - 1 : 0;
    int f1 = ind_main;

    // Atualiza o custo da solucao com base na primeira parte da split string
    // Atualiza o custo individual de cada veiculo com base na primeira parte da split string
    // Atualiza a demanda utilizada de cada veiculo com base na primeira parte da split string
    for (; i1 < f1; i1++) {
        int costumer = sol.vehicles[tour].route[i1];
        sol.cost -= data.get_distance(costumer, costumer+1);
        sol.vehicles[tour].cost -= data.get_distance(costumer, costumer+1);
        sol.vehicles[tour].capacity_used -= data.get_demand(costumer);
    }

    // Atualiza a lista de clientes ausentes com base na segunda parte da split string
    // Atualiza a lista costumer_to_vehicle com base na segunda parte da split string
    for (int i = ind_main + m; i <= ind + size_block2; i++) {
        int costumer = sol.vehicles[tour].route[i];
        sol.abs_costumers.push_back(costumer);
        sol.costumer_to_vehicle[costumer-1] = -1;
    }

    int i2 = ind_main;
    int f2 = ind_main + size_block2 < sol.vehicles[tour].route.size() - 1 ? ind_main + size_block2 + 1 : ind_main + size_block2;

    // Atualiza o custo da solucao com base na segunda parte da split string
    // Atualiza o custo individual de cada veiculo com base na segunda parte da split string
    // Atualiza a demanda utilizada de cada veiculo com base na segunda parte da split string
    for (; i2 < f2; i2++) {
        int costumer = sol.vehicles[tour].route[i2];
        sol.cost -= data.get_distance(costumer, costumer+1);
        sol.vehicles[tour].cost -= data.get_distance(costumer, costumer+1);
        sol.vehicles[tour].capacity_used -= data.get_demand(costumer);
    }
    sol.vehicles[i2].capacity_used -= data.get_demand(sol.vehicles[tour].route[i2]);

    sol.vehicles[tour].route.erase(sol.vehicles[tour].route.begin() + ind_main + 1, sol.vehicles[tour].route.begin() + ind + size_block2 + 1);

    sol.vehicles[tour].route.erase(sol.vehicles[tour].route.begin() + ind - size_block1, sol.vehicles[tour].route.begin() + ind_main); 
}
