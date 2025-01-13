#include "../include/Data.h"

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

    input_CVRP >> problem_type;

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

    input_CVRP >> edge_weight_type;

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

    input_CVRP >> capacity;

    if (capacity <= 0) {
        cout << "CAPACITY " << dimension << " invalida!" << endl;
        exit(1);
    }

    x = new double [dimension];
    y = new double [dimension];
    matrix_dist = new double *[dimension];

    for (int i = 0; i < dimension; i++) {
        matrix_dist[i] = new double [dimension];
    }

    input_CVRP.close();
}