#include "../include/Data.h"

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read_instance();

    return 0;
}