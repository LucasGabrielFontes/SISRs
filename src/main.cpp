#include "../include/Data.h"
#include "../include/random.h"
#include <time.h>

#include "../include/Solution.h"

int main(int argc, char** argv) {

    unsigned int seed = time(NULL);
    Random::randomize(seed);

    auto data = Data(argc, argv[1]);
    data.read_instance();

    Solution sol = Construction(data);
    
    return 0;
}