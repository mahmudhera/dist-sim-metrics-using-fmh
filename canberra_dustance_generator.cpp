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
    double scale_factors[] = {0.0001, 0.001, 0.01, 0.1};
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
            double true_canberra = 0.0;
            int num_distinct_kmers = 0;
            for (int i = 0; i < NUM_KMERS; i++) {
                if (vector_a[i] + vector_b[i] == 0) continue;
                true_canberra += 1.0 * abs((double)vector_a[i] - (double)vector_b[i])/((double)vector_a[i] + (double)vector_b[i]);
                if (vector_a[i] > 0 || vector_b[i] > 0) num_distinct_kmers++;
            }
            true_canberra /= (1.0 * num_distinct_kmers);

            // take sketch, compute sketched cosine
            double sketched_canberra = 0.0;
            int num_distinct_kmers_in_sketches = 0;
            for (int i = 0; i < NUM_KMERS; i++) {
                if (random_indices[i] == 0) continue;
                if (vector_a[i] + vector_b[i] == 0) continue;
                sketched_canberra += 1.0 * abs((double)vector_a[i] - (double)vector_b[i])/((double)vector_a[i] + (double)vector_b[i]);
                if (vector_a[i] > 0 || vector_b[i] > 0) num_distinct_kmers_in_sketches++;
            }
            sketched_canberra /= (1.0 * num_distinct_kmers_in_sketches);

            cout << scale_factor << ' ' << len_set_a << ' ' << len_set_b << ' ' << true_canberra << ' ' << sketched_canberra << endl;
        }
    }

    return 0;
}
