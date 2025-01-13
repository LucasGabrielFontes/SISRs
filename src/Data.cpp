#include "../include/Data.h"
#include <math.h>

Data::Data(int qt_params, char *instance):
x(NULL),
y(NULL),
demand(NULL),
matrix_dist(NULL) {

    if (qt_params != 2)  {
		cout << "Quantidade de parametros invalida!" << endl;
		cout << "Execucao correta: ./sisrs [Instance]"  << endl;
		exit(1);        
	}

    instance_name = instance;
    edge_weight_type = "";
    dimension = -1;
    capacity = -1;
    depot = -1;
}

Data::~Data(){

	delete [] x;
	delete [] y;

	for ( int i = 0; i < dimension; i++){
		delete [] matrix_dist[i];
	}
	delete [] matrix_dist;
}

void Data::read_instance(){

    ifstream input_CVRP(instance_name, ios::in);

    if (!input_CVRP) {
        cout << "Arquivo " << instance_name << " nao encontrado!" << endl;
        exit(1);
    }

    string file;

    while (file != "NAME:" && file != "NAME"){
        input_CVRP >> file;
    }

    if (file == "NAME") {
        input_CVRP >> file;
    }

    input_CVRP >> instance_name; // Resgata o nome exato do problema

    while (file != "TYPE:" && file != "TYPE"){
        input_CVRP >> file;
    }

    if (file == "TYPE") {
        input_CVRP >> file;
    }

    input_CVRP >> problem_type; // Resgata o tipo do problema

    if (problem_type != "CVRP") {
        cout << "Tipo " << problem_type << " nao e suportado!" << endl;
        exit(1);
    }

    while (file != "DIMENSION:" && file != "DIMENSION"){
        input_CVRP >> file;
    }

    if (file == "DIMENSION") {
        input_CVRP >> file;
    }

    input_CVRP >> dimension; // Resgata a dimensao do problema

    if (dimension <= 0) {
        cout << "DIMENSION " << dimension << " invalida!" << endl;
        exit(1);
    }

    while (file != "EDGE_WEIGHT_TYPE:" && file != "EDGE_WEIGHT_TYPE"){
        input_CVRP >> file;
    }

    if (file == "EDGE_WEIGHT_TYPE") {
        input_CVRP >> file;
    }

    input_CVRP >> edge_weight_type; // Resgata a regra de distancia do problema

    if (edge_weight_type != "EUC_2D") {
        cout << "Edge weight type " << edge_weight_type << " nao e suportado!" << endl;
        exit(1);
    }

    while (file != "CAPACITY:" && file != "CAPACITY"){
        input_CVRP >> file;
    }

    if (file == "CAPACITY") {
        input_CVRP >> file;
    }

    input_CVRP >> capacity; // Resgata a capacidade do problema

    if (capacity <= 0) {
        cout << "CAPACITY " << dimension << " invalida!" << endl;
        exit(1);
    }

    x = new double [dimension];
    y = new double [dimension];
    demand = new double [dimension];
    matrix_dist = new double *[dimension];

    for (int i = 0; i < dimension; i++) {
        matrix_dist[i] = new double [dimension];
    }

    while (file != "NODE_COORD_SECTION") {
        input_CVRP >> file;
    }

    int node;
    for (int i = 0; i < dimension; i++) {
        input_CVRP >> node >> x[i] >> y[i];
        if (x[i] < 0 || y[i] < 0) {
            cout << "Coordenada negativa encontrada no no " << i+1 << endl;
            exit(1);
        }
    }

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (i == j) {
                matrix_dist[i][j] = INFINITE;
                continue;
            }

            matrix_dist[i][j] = calc_dist(x, y, i, j);
        }
    }

    while (file != "DEMAND_SECTION") {
        input_CVRP >> file;
    }

    for (int i = 0; i < dimension; i++) {
        input_CVRP >> node >> demand[i];
        if (demand[i] < 0) {
            cout << "Demanda negativa encontrada no no " << i+1 << endl;
            exit(1);
        }
    }

    while (file != "DEPOT_SECTION") {
        input_CVRP >> file;
    }

    input_CVRP >> depot;

    if (depot < 1 || depot > dimension) {
        cout << "Deposito invalido!" << endl;
        exit(1);
    }

    input_CVRP >> file;

    if (file != "-1") {
        cout << "DEPOT_SECTION invalida!" << endl;
        exit(1);
    }

    input_CVRP.close();
}

double Data::calc_dist (double *x, double *y, int i, int j){
	return sqrt(pow(x[i] - x[j], 2) + pow (y[i] - y[j], 2));
}

void Data::print_matrix_dist() {
    for (int i = 0; i < get_dimension(); i++) {
        for (int j = 0; j < get_dimension(); j++) {
            cout << matrix_dist[i][j] << " ";
        }
        cout << endl;
    }
}