/*------------------------------------------------------------------------------
 * SPP-Solver — A compact Set Packing solver blending MILP, heuristics,
 *              and efficient BitVector techniques.
 *------------------------------------------------------------------------------
 * Copyright (c) 2026 Josué Tambwe
 *               Nantes Université, France
 *------------------------------------------------------------------------------
 * This software is released under the MIT License.
 * See the LICENSE file for more details.
 *------------------------------------------------------------------------------
 */


/** 
 * @file RunGeneticAlgorithm.cpp
 * @author Josué Tambwe
 * @date 12 August 2026
 */

#include "algorithms/geneticAlgorithm/RunGeneticAlgorithm.hpp"


namespace spp{

    void runGeneticAlgorithm(const Params &params){

        printHeader();

        // building the instance
        const Instance instance(params);

        printHeaderGeneticAlgorithm(params, instance);
        

        // selection probabilities of low-level metaheuristics
        std::vector<std::array<double, 3>> all_proportions = {
            {0.33, 0.33, 0.33},
            {0.33, 0.33, 0.33},
            {0.33, 0.33, 0.33},
            {0.33, 0.33, 0.33},
            {0.33, 0.33, 0.33},
            {0.33, 0.33, 0.33},
            {0.33, 0.33, 0.33},
            {0.33, 0.33, 0.33},
            {0.33, 0.33, 0.33},
            {0.33, 0.33, 0.33}
        };

        size_t fixed_population_size = params.population_size;
        size_t nb_survivors = static_cast<size_t>(std::floor(fixed_population_size * params.survivor_rate));
        size_t nb_new_individuals_per_generation = fixed_population_size - nb_survivors;
        size_t nb_couples = static_cast<size_t>(std::floor(fixed_population_size * params.crossover_rate) / 2.0);


        Timer timer = Timer();
        Logger log;

        timer.start();

        // initialization of the population
        std::vector<Solution> current_population = generateNewIndividuals(fixed_population_size,
                                                                          all_proportions,
                                                                          params,
                                                                          instance);

        // initialization of the best solution
        Solution best_individual = findBestIndividual(current_population, instance);
        std::int64_t best_individual_objective_value = best_individual.getObjectiveValue(instance);

        size_t current_generation = 0;

        do{

            current_generation += 1;

            std::vector<double> population_fitness = computePopulationFitness(current_population,
                                                                              params,
                                                                              instance);

            double max_population_fitness = 0.0;
            double min_population_fitness = 0.0;
            double average_population_fitness = 0.0;

            computeMaximumMinimumAverageFitness(max_population_fitness,
                                                min_population_fitness,
                                                average_population_fitness,
                                                population_fitness);

            Solution local_best_individual = findBestIndividual(current_population, instance);
            std::int64_t local_best_individual_objective_value = local_best_individual.getObjectiveValue(instance);

            // update of the best solution before crossover
            if(local_best_individual_objective_value > best_individual_objective_value){

                best_individual_objective_value = local_best_individual_objective_value;
                best_individual = local_best_individual;
            }


            std::vector<Solution> children = performCrossover(current_population,
                                                              population_fitness,
                                                              params,
                                                              instance);

            // crossover statistics
            double max_children_fitness = 0.0;
            double min_children_fitness = 0.0;
            double average_children_fitness = 0.0;

            // mutation statistics
            double max_mutated_children_fitness = 0.0;
            double min_mutated_children_fitness = 0.0;
            double average_mutated_children_fitness = 0.0;

            size_t nb_mutated_children = 0;

            // when the crossover is not childless
            if(children.size() > 0){

                std::vector<double> children_fitness = computePopulationFitness(children,
                                                                                params,
                                                                                instance);

                computeMaximumMinimumAverageFitness(max_children_fitness,
                                                    min_children_fitness,
                                                    average_children_fitness,
                                                    children_fitness);


                Solution best_child = findBestIndividual(children, instance);
                std::int64_t best_child_objective_value = best_child.getObjectiveValue(instance);

                // update of the best solution before mutation
                if(best_child_objective_value > best_individual_objective_value){

                    best_individual_objective_value = best_child_objective_value;
                    best_individual = best_child;
                }

                // mutation

                std::vector<size_t> children_to_mutate_indexes = selectIndexesToMutate(children.size(), params);

                nb_mutated_children = children_to_mutate_indexes.size();

                if(children_to_mutate_indexes.size() > 0){

                    performMutation(children,
                                    children_to_mutate_indexes,
                                    params,
                                    instance);


                    computeMaximumMinimumAverageMutationFitness(max_mutated_children_fitness,
                                                                min_mutated_children_fitness,
                                                                average_mutated_children_fitness,
                                                                children_to_mutate_indexes,
                                                                children_fitness);

                }
                
            }


            printGeneticAlgorithmIteration(timer.getElapsedTime(), 
                                           current_generation,
                                           nb_couples,
                                           children.size(),
                                           nb_mutated_children,
                                           best_individual_objective_value,
                                           max_population_fitness,
                                           min_population_fitness,
                                           average_population_fitness,
                                           max_children_fitness,
                                           min_children_fitness,
                                           average_children_fitness,
                                           max_mutated_children_fitness,
                                           min_mutated_children_fitness,
                                           average_mutated_children_fitness,
                                           alpha_values_GA,
                                           all_proportions);


            // the unified population : old generation + new generation
            std::vector<Solution> unified_population = unifyPopulation(current_population,
                                                                       children,
                                                                       instance);

            // the top-scoring individual survivors
            std::vector<Solution> elite_survivors = computeEliteSurvivors(nb_survivors, 
                                                                          unified_population,
                                                                          params,
                                                                          instance);

            // population update : elite survivors + new generated individuals
            std::vector<Solution> new_generation = generateNewIndividuals(nb_new_individuals_per_generation,
                                                                          all_proportions,
                                                                          params,
                                                                          instance);

            current_population = unifyPopulation(elite_survivors,
                                                 new_generation,
                                                 instance);


            local_best_individual = findBestIndividual(current_population, instance);
            local_best_individual_objective_value = local_best_individual.getObjectiveValue(instance);

            // update of the best solution after population replacement
            if(local_best_individual_objective_value > best_individual_objective_value){

                best_individual_objective_value = local_best_individual_objective_value;
                best_individual = local_best_individual;
            }

        }
        while(!stoppingCriteriaGeneticAlgorithm(timer.getElapsedTime(), current_generation, params));

        log.info("Genetic Algorithm completed. Final best known solution :");


        best_individual.print(instance);

        printSummaryGeneticAlgorithm(timer.getElapsedTime(), 
                                     best_individual.getObjectiveValue(instance),
                                     current_generation,
                                     best_individual.getStatus());
    }
}
