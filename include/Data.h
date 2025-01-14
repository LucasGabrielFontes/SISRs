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
    void print_matrix_dist();

    inline int get_dimension() const {return dimension;};
    inline int get_capacity() {return capacity;};
    inline int get_depot() {return depot;};

    inline string get_instance_name() {return instance_name;};
    inline string get_problem_type() {return problem_type;};
    inline string get_edge_weight_type() {return edge_weight_type;};

    inline double get_distance(int i, int j) const {return matrix_dist[i-1][j-1];};
    inline double get_x(int i) {return x[i-1];};
    inline double get_y(int i) {return y[i-1];};
    inline double get_demand(int i) const {return demand[i-1];};

private:
    int dimension;            // Dimensao da instancia
    int capacity;             // Capacidade homogenea dos veiculos
    int depot;                // No de deposito (1)

    string instance_name;     // Nome da instancia
    string problem_type;      // Tipo do problema (CVRP)
    string edge_weight_type;  // EUC_2D

    double **matrix_dist;     // Matriz que armazena a distancia de um no a outro
    double *x, *y;            // Vetores que armazenam as coordenadas no eixo x e y de cada ponto no, respectivamente
    int *demand;              // Vetor que armazena a demanda de cada no

    static double calc_dist (double *, double *, int , int ); // Calcula a distancia entre dois nos
};

#endif