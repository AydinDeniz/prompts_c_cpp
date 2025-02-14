#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <functional>
#include <cmath>

// Define the Individual
class Individual {
public:
    std::vector<double> genes;
    double fitness;

    Individual(size_t num_genes) : genes(num_genes), fitness(0.0) {}

    void calculateFitness() {
        // Fitness function: sphere function for minimization (-x^2)
        fitness = 0.0;
        for (double gene : genes) {
            fitness += gene * gene;
        }
    }

    void print() const {
        for (double gene : genes) {
            std::cout << gene << " ";
        }
        std::cout << ": " << fitness << std::endl;
    }
};

// Define the Genetic Algorithm
class GeneticAlgorithm {
public:
    GeneticAlgorithm(size_t pop_size, size_t num_genes, size_t num_generations, double crossover_rate, double mutation_rate)
        : pop_size(pop_size),
          num_genes(num_genes),
          num_generations(num_generations),
          crossover_rate(crossover_rate),
          mutation_rate(mutation_rate) {
        rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
    }

    void run() {
        initializePopulation();
        evaluatePopulation();

        for (size_t generation = 0; generation < num_generations; ++generation) {
            std::cout << "Generation " << generation << " best fitness: " << getBestFitness() << "\n";
            population = createNextGeneration();
            evaluatePopulation();
        }

        std::cout << "Best solution: ";
        getBestIndividual().print();
    }

private:
    size_t pop_size;
    size_t num_genes;
    size_t num_generations;
    double crossover_rate;
    double mutation_rate;
    std::vector<Individual> population;
    std::mt19937 rng;

    void initializePopulation() {
        std::uniform_real_distribution<double> distribution(-5.0, 5.0);
        for (size_t i = 0; i < pop_size; ++i) {
            population.emplace_back(num_genes);
            for (size_t j = 0; j < num_genes; ++j) {
                population[i].genes[j] = distribution(rng);
            }
        }
    }

    void evaluatePopulation() {
        for (Individual& individual : population) {
            individual.calculateFitness();
        }
    }

    Individual selectParent() {
        std::uniform_int_distribution<size_t> distribution(0, pop_size - 1);
        return population[distribution(rng)];
    }

    void crossover(Individual& parent1, Individual& parent2, Individual& offspring1, Individual& offspring2) {
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        if (distribution(rng) < crossover_rate) {
            std::uniform_int_distribution<size_t> crossover_point(0, num_genes - 1);
            size_t point = crossover_point(rng);
            for (size_t i = 0; i < num_genes; ++i) {
                if (i <= point) {
                    offspring1.genes[i] = parent1.genes[i];
                    offspring2.genes[i] = parent2.genes[i];
                } else {
                    offspring1.genes[i] = parent2.genes[i];
                    offspring2.genes[i] = parent1.genes[i];
                }
            }
        } else {
            offspring1 = parent1;
            offspring2 = parent2;
        }
    }

    void mutate(Individual& individual) {
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        std::normal_distribution<double> mutation_distribution(0.0, 0.1);
        for (double& gene : individual.genes) {
            if (distribution(rng) < mutation_rate) {
                gene += mutation_distribution(rng);
            }
        }
    }

    std::vector<Individual> createNextGeneration() {
        std::vector<Individual> next_generation;
        while (next_generation.size() < pop_size) {
            Individual parent1 = selectParent();
            Individual parent2 = selectParent();
            Individual offspring1(num_genes), offspring2(num_genes);

            crossover(parent1, parent2, offspring1, offspring2);
            
            mutate(offspring1);
            mutate(offspring2);

            next_generation.push_back(offspring1);
            if (next_generation.size() < pop_size) {
                next_generation.push_back(offspring2);
            }
        }
        return next_generation;
    }

    double getBestFitness() const {
        return std::min_element(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
        })->fitness;
    }

    const Individual& getBestIndividual() const {
        return *std::min_element(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
        });
    }
};

int main() {
    GeneticAlgorithm ga(100, 10, 50, 0.7, 0.01);
    ga.run();
    return 0;
}