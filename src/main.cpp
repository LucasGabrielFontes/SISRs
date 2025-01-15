#include "../include/Data.h"
#include "../include/random.h"
#include <time.h>

#include "../include/Solution.h"
#include "../include/LocalSearch.h"

int main(int argc, char** argv) {

    unsigned int seed = time(NULL);
    Random::randomize(seed);

    auto data = Data(argc, argv[1]);
    data.read_instance();

    Solution sol = Construction(data);

    cout << "Veiculos e suas rotas: " << endl;

    for (int i = 0; i < sol.vehicles.size(); i++) {
        cout << "Veiculo " << i+1 << ": ";
        for (int j = 0; j < sol.vehicles[i].route.size() - 1; j++) {
            cout << sol.vehicles[i].route[j] << " -> ";
        }
        cout << sol.vehicles[i].route[sol.vehicles[i].route.size() - 1] << endl;
    }

    return 0;
}
