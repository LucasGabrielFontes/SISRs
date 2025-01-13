#include "../include/Data.h"

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read_instance();
    size_t n = data.dimension;

    cout << "Nome do problema: " << data.instance_name << endl;
    cout << "Tipo: " << data.problem_type << endl;
    cout << "Dimensao do problema: " << data.dimension << endl;
    cout << "Edge weight type: " << data.edge_weight_type << endl;
    cout << "Capacidade: " << data.capacity << endl;

    return 0;
}