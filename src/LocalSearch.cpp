#include "../include/LocalSearch.h"
#include "../include/random.h"
#include "../include/Solution.h"
#include <cmath>
#include <algorithm>

#define EP 1e-9
#define T0 100
#define Tf 1
#define F 10000

#define C_ 10
#define L_MAX 10

#define ALPHA 0.01

void LocalSearch(Solution& solution, const Data& data) { // struct

    Solution best_sol = solution;
    double T = T0;
    for (int i = 0; i < F; i++) {

        Solution solution_star; //= ruin(solution);
        solution_star; //= recreate(solution_star);
        if (solution_star.cost < solution.cost - T*log(Random::getReal(0+EP, 1-EP))) {
            solution = solution_star;
        }
        if (solution_star.cost < best_sol.cost) {
            best_sol = solution_star;
        }

        double c = pow((Tf/T0), (1/F));

        T = c*T;
    }
}

Solution ruin(Solution sol, Data& data) {

    double lsMax = min(static_cast<double>(L_MAX), static_cast<double>(data.get_dimension())/sol.vehicles.size()); // Cardinalidade maxima de cada string removida (todas sao limitadas a isso)

    double ksMax = ((4*C_)/(1+lsMax))-1; // Quantidade maxima de strings a serem removidas

    int ks = floor(Random::getReal(1+EP, ksMax + 1 - EP)); // Quantidade de strings que serao removidas

    int csSeed = Random::getInt(2, data.get_dimension()); // Cliente aleatorio

    vector<int> R = {}; // Conjunto de tour arruinados

    for (int i = 1; i <= data.get_dimension() && R.size() < ks; i++) {

        if (find(sol.abs_costumers.begin(), sol.abs_costumers.end(), data.get_adj(csSeed, i)) == sol.abs_costumers.end()
        && find(R.begin(), R.end(), sol.costumer_to_vehicle[data.get_adj(csSeed, i)]) == R.end()) {

            int ct_star = data.get_adj(csSeed, i);

            double ltMax = min(static_cast<double>(sol.vehicles[sol.costumer_to_vehicle[data.get_adj(csSeed, i)] - 1].route.size()), lsMax); // Cardinalidade maxima da string a ser removida no tour/veiculo t

            int lt = floor(Random::getReal(1 + EP, ltMax + 1 - EP)); // Cardinalidade da string a ser removida no tuor t

            int rd = Random::getInt(0, 1);

            // Remove as strings. O conjunto de absent costumers ja eh atualizado nas funcoes
            if (rd == 0) {
                remove_string(sol, data, sol.costumer_to_vehicle[data.get_adj(csSeed, i)], lt, ct_star);
            } else {
                remove_split_string(sol, data, sol.costumer_to_vehicle[data.get_adj(csSeed, i)], lt, ct_star);
            }

            R.push_back(sol.costumer_to_vehicle[data.get_adj(csSeed, i)]);
        }
    }

    return sol;
}

void remove_split_string(Solution &sol, Data& data, int tour, int size_string, int costumer_remove) {

    int size_block1 = Random::getInt(0, size_string);
    int size_block2 = size_string - size_block1 - 1;

    int m = 1;

    int rd = Random::getInt(0+EP, 1-EP);

    while (!(rd < ALPHA || m == sol.vehicles[tour].route.size() - size_string)) {
        m += 1;
        rd = Random::getInt(0+EP, 1-EP);
    }

    size_string += m;

    int size_block1 = Random::getInt(0, size_string);
    int size_block2 = size_string - size_block1 - 1;

    auto it = find(sol.vehicles[tour].route.begin(), sol.vehicles[tour].route.end(), costumer_remove);
    int ind = (it != sol.vehicles[tour].route.end()) ? std::distance(sol.vehicles[tour].route.begin(), it) : -1;

    if (ind == -1) {
        cout << "Erro na funcao remove_string" << endl;
        exit(1);
    }

    if (ind - size_block1 < 0 || ind + size_block2 >= sol.vehicles[tour].route.size()) {
        cout << "Bloco fora dos limites da rota na funcao remove_string." << endl;
        exit(1);
    }

    int ind_main = Random::getInt(ind, ind + size_string - 1 - m); // Indice que indica o inicio da string que permanecera na rota

    for (int i = ind - size_block1; i < ind_main; i++) {
        sol.abs_costumers.push_back(sol.vehicles[tour].route[i]);
    }

    for (int i = ind_main + m; i <= ind + size_block2; i++) {
        sol.abs_costumers.push_back(sol.vehicles[tour].route[i]);
    }

    sol.vehicles[tour].route.erase(sol.vehicles[tour].route.begin() + ind - size_block1, sol.vehicles[tour].route.begin() + ind_main + 1); 

    sol.vehicles[tour].route.erase(sol.vehicles[tour].route.begin() + ind_main + m - 1, sol.vehicles[tour].route.begin() + ind + size_block2 + 1);     

}

void remove_string(Solution &sol, Data& data, int tour, int size_string, int costumer_remove) {

    int size_block1 = Random::getInt(0, size_string);
    int size_block2 = size_string - size_block1 - 1;

    auto it = find(sol.vehicles[tour].route.begin(), sol.vehicles[tour].route.end(), costumer_remove);
    int ind = (it != sol.vehicles[tour].route.end()) ? std::distance(sol.vehicles[tour].route.begin(), it) : -1;

    if (ind == -1) {
        cout << "Erro na funcao remove_string" << endl;
        exit(1);
    }

    if (ind - size_block1 < 0 || ind + size_block2 >= sol.vehicles[tour].route.size()) {
        cout << "Bloco fora dos limites da rota na funcao remove_string." << endl;
        exit(1);
    }

    for (int i = ind - size_block1; i <= ind + size_block2; i++) {
        sol.abs_costumers.push_back(sol.vehicles[tour].route[i]);
    }

    sol.vehicles[tour].route.erase(sol.vehicles[tour].route.begin() + ind - size_block1, sol.vehicles[tour].route.begin() + size_block2 + 1);    
}
