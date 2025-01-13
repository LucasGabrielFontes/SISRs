#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <fstream>

#define INFINITE 0

using namespace std;

class Data{
public:
    Data(int, char *);
    ~Data();

    void read_instance();

    // ************** colocar como privado
    int dimension; // Dimensao da instancia
    string instance_name; // Nome da instancia
    string problem_type; // Tipo do problema (CVRP)
    string edge_weight_type; // EUC_2D

    double **matrix_dist; // Matriz que armazena a distancia de um no a outro
    double *x, *y; // Vetores que armazenam as coordenadas no eixo x e y de cada ponto no, respectivamente
    double *demand; // Vetor que armazena a demanda de cada no
    int capacity; // Capacidade homogenea dos veiculos

    int depot; // No de deposito

    inline int get_dimension() {return dimension;};

    static double calc_dist (double *, double *, int , int );
    void print_matrix_dist();

private:

    
};

#endif