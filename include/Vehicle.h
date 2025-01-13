#ifndef VEHICLE_H
#define VEHICLE_H

#include "Data.h"
#include <vector>

struct Vehicle {
    int capacity_used = 0;        // Capacidade total utilizada pelo veiculo atualmente (a capacidade maxima eh a mesma para todos os veiculos e pode ser acessada a partir da instancia da classe Data com a funcao get_capacity())
    std::vector<int> route = {}; // Rota de nos a ser visitados pelo veiculo
    double cost = 0.0;           // Custo total do veiculo (distancia total percorrida) 
};

int calc_capacity(const struct Vehicle& , const Data& );
double calc_cost_vehicle(const struct Vehicle& , const Data & );

#endif