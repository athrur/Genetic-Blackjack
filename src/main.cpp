#include "blackjack.h"
#include "genetic.h"



int main()
{
    int population_size = 800;
    int generations = 50000;
    int simulations_per = 200000;
    
    int hard_population[population_size][17][10];
    int soft_population[population_size][9][10];
    
    double fitness[population_size];
    double mean_fitness[generations];
    
    // Initialise random population
    for (int i = 0; i < population_size; i++) {
        randomise_strategy(hard_population[i], soft_population[i]);
    }
    
    // Calculate fitness
    for (int i = 0; i < population_size; i++) {
        fitness[i] = calculate_fitness(hard_population[i], soft_population[i], simulations_per);
        mean_fitness[0] += fitness[i];
    }
    
    std::cout << "0: Mean fitness: " << mean_fitness[0] / population_size << std::endl;
    
    
    for (int i = 1; i <= generations; i++) {
        // reproduction here. transforms population into new population
        auto start = std::chrono::high_resolution_clock::now();
        reproduce(hard_population, soft_population, fitness, population_size);
        
        for (int j = 0; j < population_size; j++) {
            fitness[j] = calculate_fitness(hard_population[j], soft_population[j], simulations_per);
            mean_fitness[i] += fitness[j];
        }
        // get start time
        std::cout << i << ": Mean fitness: " << mean_fitness[i] / population_size << " Change: " << (mean_fitness[i] - mean_fitness[i - 1]) / population_size << " Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;
        // print best strategy
        
        int best = 0;
        for (int i = 1; i < population_size; i++) {
            if (fitness[i] > fitness[best]) {
                best = i;
            }
        }
        
        // now save the best strategy
        FILE *fp;
        fp = fopen("best_strategy.txt", "w");
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 10; j++) {
                fprintf(fp, "%d ", hard_population[best][i][j]);
            }
            fprintf(fp, "\n");
        }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 10; j++) {
                fprintf(fp, "%d ", soft_population[best][i][j]);
            }
            fprintf(fp, "\n");
        }
        fclose(fp);
        
    }
    
    return 0;
};