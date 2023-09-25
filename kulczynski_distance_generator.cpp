#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

using namespace std;

#define NUM_KMERS 100000
#define NUM_RUNS 10000

int main() {
    int len_set_a = 50000;
    int min_len_set_b = 10000;
    int max_len_set_b = 90000;
    int seed = 0;
    double scale_factors[] = {0.0001, 0.001, 0.01, 0.1, 0.95, 1.0};
    srand(seed);

    default_random_engine generator;
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(min_len_set_b, max_len_set_b);
    for (double scale_factor : scale_factors) {
        // select sketch size
        bool random_indices[NUM_KMERS] = {0};
        int sketch_size = NUM_KMERS * scale_factor;
        bernoulli_distribution distribution(scale_factor);

        // generate random indices
        for (int i = 0; i < NUM_KMERS; i++) {
            random_indices[i] = distribution(generator);
        }

        for (int iter = 0; iter < NUM_RUNS; iter++) {
            // generate vector a and b randomly
            bool vector_a[NUM_KMERS] = {0};
            bool vector_b[NUM_KMERS] = {0};

            for (int i = 0; i < len_set_a; i++) vector_a[i] = 1;
            random_shuffle(std::begin(vector_a), std::end(vector_a));

            int len_set_b = uni(rng);
            for (int i = 0; i < len_set_b; i++) vector_b[i] = 1;
            random_shuffle(std::begin(vector_b), std::end(vector_b));

            // compute true
            double true_kulczynski = 0.0;
            for (int i = 0; i < NUM_KMERS; i++) true_kulczynski += min(vector_a[i], vector_b[i]);
            true_kulczynski = 1.0 - 0.5 * (0.0 + len_set_a + len_set_b) * true_kulczynski/(1.0*len_set_a*len_set_b);

            // take sketch, compute sketched cosine
            double sketched_kulczynski = 0.0;
            int len_sketch_of_a = 0;
            int len_sketch_of_b = 0;
            for (int i = 0; i < NUM_KMERS; i++) {
                sketched_kulczynski += min(vector_a[i], vector_b[i])*random_indices[i];
                len_sketch_of_a += vector_a[i]*random_indices[i];
                len_sketch_of_b += vector_b[i]*random_indices[i];
            }
            sketched_kulczynski = 1.0 - 0.5 * (0.0 + len_sketch_of_a + len_sketch_of_b) * sketched_kulczynski / (1.0*len_sketch_of_a*len_sketch_of_b);

            cout << scale_factor << ' ' << len_set_a << ' ' << len_set_b << ' ' << true_kulczynski << ' ' << sketched_kulczynski << endl;
        }
    }

    return 0;
}
