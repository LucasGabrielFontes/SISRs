#include "../include/Data.h"
#include "../include/random.h"
#include <time.h>

int main(int argc, char** argv) {

    unsigned int seed = time(NULL);
    Random::randomize(seed);

    auto data = Data(argc, argv[1]);
    data.read_instance();

    int L_max = 10;

    return 0;
}