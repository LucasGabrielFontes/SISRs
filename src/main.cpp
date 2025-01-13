#include "../include/Data.h"

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read_instance();

    cout << "Dimension: " << data.get_dimension() << endl;

    return 0;
}