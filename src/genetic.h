#ifndef GENETIC_H
#define GENETIC_H

void randomise_strategy(int hard[17][10], int soft[9][10]);
double calculate_fitness(int hard[17][10], int soft[9][10], int simulations);
void select_parents(int hard_population[][17][10], int soft_population[][9][10], double fitness[], int population_size);
void tournament_select_parents(int hard_population[][17][10], int soft_population[][9][10], double fitness[], int population_size, int tournament_size);
void reproduce(int hard_population[][17][10], int soft_population[][9][10], double fitness[], int population_size);

#endif
