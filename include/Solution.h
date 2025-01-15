#ifndef SOLUTION_H
#define SOLUTION_H

#include "Vehicle.h"

struct Solution {
    std::vector<Vehicle> vehicles = {};        // Lista de veiculos de toda rota
    std::vector<int> abs_costumers = {};       // Lista de clientes ausentes (tem que ser vazia em uma solucao viavel)
    std::vector<int> costumer_to_vehicle = {}; // Cada posicao i indica em qual veiculo o pedido do cliente i+1 esta
    double cost = 0.0;                         // Custo total da solucao
};

double calc_cost_solution(const struct Solution&, const Data &);
Solution Solution_Sisrs(Data& );
Solution Construction(Data& );

#endif