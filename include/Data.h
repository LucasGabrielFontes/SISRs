#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <fstream>

using namespace std;

class Data{
public:
    Data(int, char *);
    ~Data();

    void read_instance();
    // ************** colocar como privado
    int dimension; // Dimensao da instancia
    string instance_name; // Nome da instancia
    string problem_type;
    string edge_weight_type;

    double **matrix_dist; // Matriz que armazena a distancia de um no a outro
    double *x, *y; // Vetores que armazenam as coordenadas no eixo x e y de cada ponto no, respectivamente
    double *demand; // Vetor que armazena a demanda de cada no
    int capacity; // Capacidade homogenea dos veiculos

    int depot; // No de deposito

private:

    
};

#endif