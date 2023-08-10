#include "blackjack.h"
#include <iostream>
#include <random>

void randomise_strategy(int hard[17][10], int soft[9][10])
{
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            hard[i][j] = rand() % 2;
        }
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            soft[i][j] = rand() % 2;
        }
    }
}

double calculate_fitness(int hard[17][10], int soft[9][10], int simulations)
{
    Game game;
    game.set_strategy(hard, soft);
    double fitness = 0;
    for (int i = 0; i < simulations; i++)
    {
        fitness += game.play_game();
    }
    return fitness / simulations;
}

void select_parents(int hard_population[][17][10], int soft_population[][9][10], double fitness[], int population_size)
{
    // delete the last 50% of the population

    for (int i = 0; i < population_size / 2; i++)
    {
        int parent1 = rand() % population_size;
        int parent2 = rand() % population_size;

        // select the fittest parent
        if (fitness[parent1] > fitness[parent2])
        {
            memcpy(hard_population[i], hard_population[parent1], sizeof(hard_population[i]));
            memcpy(soft_population[i], soft_population[parent1], sizeof(soft_population[i]));
        }
        else
        {
            memcpy(hard_population[i], hard_population[parent2], sizeof(hard_population[i]));
            memcpy(soft_population[i], soft_population[parent2], sizeof(soft_population[i]));
        }
    }
}

void tournament_select_parents(int hard_population[][17][10], int soft_population[][9][10], double fitness[], int population_size, int tournament_size)
{
    for (int i = 0; i < population_size / 2; i++)
    {
        int parent1 = -1;
        int parent2 = -1;

        // Conduct the tournament to select the first parent
        for (int j = 0; j < tournament_size; j++)
        {
            int candidate = rand() % population_size;
            if (parent1 == -1 || fitness[candidate] > fitness[parent1])
            {
                parent1 = candidate;
            }
        }

        // Conduct another tournament to select the second parent
        for (int j = 0; j < tournament_size; j++)
        {
            int candidate = rand() % population_size;
            if (parent2 == -1 || fitness[candidate] > fitness[parent2])
            {
                parent2 = candidate;
            }
        }

        // Select the fittest parent
        if (fitness[parent1] > fitness[parent2])
        {
            memcpy(hard_population[i], hard_population[parent1], sizeof(hard_population[i]));
            memcpy(soft_population[i], soft_population[parent1], sizeof(soft_population[i]));
        }
        else
        {
            memcpy(hard_population[i], hard_population[parent2], sizeof(hard_population[i]));
            memcpy(soft_population[i], soft_population[parent2], sizeof(soft_population[i]));
        }
    }
}

// basic reproduce function. atm just return back the same population
void reproduce(int hard_population[][17][10], int soft_population[][9][10], double fitness[], int population_size)
{
    tournament_select_parents(hard_population, soft_population, fitness, population_size, 4);
    // now the first half of the population is the parents
    // we want to crossover uniformly between the parents

    // e.g. population1 and population2 breed to create population(n+1) and population(n+2) where n is len population / 2
    // then 3 and 4 breed etc

    for (int i = 0; i < population_size / 2; i += 2)
    {
        // we want to uniformly crossover between the parents
        // e.g. 50% of the time we take the first parent's strategy and 50% of the time we take the second parent's strategy

        for (int j = 0; j < 17; j++)
        {
            for (int k = 0; k < 10; k++)
            {
                if (rand() % 2 == 0)
                {
                    hard_population[i + population_size / 2][j][k] = hard_population[i][j][k];
                    hard_population[i + population_size / 2 + 1][j][k] = hard_population[i + 1][j][k];
                }
                else
                {
                    hard_population[i + population_size / 2][j][k] = hard_population[i + 1][j][k];
                    hard_population[i + population_size / 2 + 1][j][k] = hard_population[i][j][k];
                }
            }
        }
        for (int j = 0; j < 9; j++)
        {
            for (int k = 0; k < 10; k++)
            {
                if (rand() % 2 == 0)
                {
                    soft_population[i + population_size / 2][j][k] = soft_population[i][j][k];
                    soft_population[i + population_size / 2 + 1][j][k] = soft_population[i + 1][j][k];
                }
                else
                {
                    soft_population[i + population_size / 2][j][k] = soft_population[i + 1][j][k];
                    soft_population[i + population_size / 2 + 1][j][k] = soft_population[i][j][k];
                }
            }
        }
    }

    // mutate 10% of the population
    for (int i = 0; i < population_size / 10; i++)
    {
        int individual = rand() % population_size;
        int strategy = rand() % 2;
        int row = rand() % 17;
        int col = rand() % 10;
        if (strategy == 0)
        {
            hard_population[individual][row][col] = rand() % 2;
        }
        else
        {
            soft_population[individual][row][col] = rand() % 2;
        }
    }
}