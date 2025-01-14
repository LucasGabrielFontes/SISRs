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

    vector<int> costumers_not_abs = get_customers_not_abs(data, sol);
    int ind = Random::getInt(0, costumers_not_abs.size()-1);
    int csSeed = costumers_not_abs[ind]; // Cliente aleatorio

    vector<int> adjC = costumers_not_abs; // Lista de adjacencia que sera ordenada com base na distancia ate o cliente aleatorio
    sort(adjC.begin(), adjC.end(), [data, csSeed](int a, int b) {
        return data.get_distance(a, csSeed) < data.get_distance(b, csSeed);
    });

    vector<int> R = {}; // Conjunto de tour arruinados

    for (int i = 0; i < adjC.size() && R.size() < ks; i++) {

        if (find(sol.abs_costumers.begin(), sol.abs_costumers.end(), adjC[i]) == sol.abs_costumers.end()
        && find(R.begin(), R.end(), sol.costumer_to_vehicle[adjC[i]]) == R.end()) {

            int ct_star = adjC[i];

            double ltMax = min(static_cast<double>(sol.vehicles[sol.costumer_to_vehicle[adjC[i]] - 1].route.size()), lsMax); // Cardinalidade maxima da string a ser removida no tour/veiculo t

            int lt = floor(Random::getReal(1 + EP, ltMax + 1 - EP)); // Cardinalidade da string a ser removida no tuor t

            int rd = Random::getInt(0, 1);

            // Remove as strings. O conjunto de absent costumers ja eh atualizado nas funcoes
            if (rd == 0) {
                remove_string(sol, data, sol.costumer_to_vehicle[adjC[i]], lt, ct_star);
            } else {
                remove_split_string(sol, data, sol.costumer_to_vehicle[adjC[i]], lt, ct_star);
            }

            R.push_back(sol.costumer_to_vehicle[adjC[i]]);
        }
    }

    return sol;
}

vector<int> get_customers_not_abs(Data& data, Solution& sol) {

    std::vector<int> all_customers = {};

    for (int i = 2; i <= data.get_dimension(); i++)
        all_customers.push_back(i);

    // Remove os clientes ausentes
    for (int customer: sol.abs_costumers)
        all_customers.erase(std::remove(all_customers.begin(), all_customers.end(), customer), all_customers.end());

    return all_customers;
}