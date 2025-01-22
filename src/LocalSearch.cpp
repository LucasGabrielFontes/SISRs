#include "../include/LocalSearch.h"
#include "../include/random.h"
#include "../include/Solution.h"
#include "../include/Ruin.h"
#include "../include/Recreate.h"
#include <cmath>
#include <algorithm>

Solution LocalSearch(Solution& solution, Data& data) { // struct

    int dimension = data.get_dimension();

    double T0 = 100.0;
    double Tf = 1.0;
    double EP = 1e-6;

    long long F = calculateIterations(dimension);
    double c = pow((Tf/T0), (1.0/(double)F)); // O erro tava aqui, rapaz, imoral

    c -= EP;

    Solution best_sol = solution;
    double T = T0;
    for (long long i = 0; i < F; i++) {

        Solution solution_star = ruin_recreate(solution, data);
        if (solution_star.cost < solution.cost - T*log(Random::getReal(0, 1))) {
            solution = solution_star;
        }
        if (solution_star.cost < best_sol.cost) {
            best_sol = solution_star;
        }

        T *= c;
    }
    return best_sol;
}

long long calculateIterations(int v) {
    // Valores conhecidos
    const long long it100 = 3 * static_cast<long long>(pow(10, 7));  // it(100) = 3 x 10^7
    const long long it1000 = 3 * static_cast<long long>(pow(10, 8)); // it(1000) = 3 x 10^8

    // Verificação dos limites
    if (v < 100) {
        throw std::invalid_argument("O tamanho do problema deve ser pelo menos 100.");
    }
    if (v > 1000) {
        throw std::invalid_argument("O tamanho do problema deve ser no máximo 1000.");
    }

    // Interpolação
    long long iterations = it100 + static_cast<long long>((v - 100) / 900.0 * (it1000 - it100));
    // return iterations;
    return 1e6;
}