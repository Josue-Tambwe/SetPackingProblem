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

        //printHeaderGRASP(params, instance);

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

        while(!stoppingCriteriaGeneticAlgorithm(timer.getElapsedTime(), current_generation, params)){

            current_generation += 1;

            Solution local_best_individual = findBestIndividual(current_population, instance);
            std::int64_t local_best_individual_objective_value = local_best_individual.getObjectiveValue(instance);

            // update of the best solution before crossover
            if(local_best_individual_objective_value > best_individual_objective_value){

                best_individual_objective_value = local_best_individual_objective_value;
                best_individual = local_best_individual;
            }


            std::vector<double> population_fitness = computePopulationFitness(current_population,
                                                                              params,
                                                                              instance);


            std::vector<Solution> children = performCrossover(current_population,
                                                              population_fitness,
                                                              params,
                                                              instance);

        
            Solution best_child = findBestIndividual(children, instance);
            std::int64_t best_child_objective_value = best_child.getObjectiveValue(instance);

            // update of the best solution before mutation
            if(best_child_objective_value > best_individual_objective_value){

                best_individual_objective_value = best_child_objective_value;
                best_individual = best_child;
            }

            
            // when the crossover is not childless
            if(children.size() > 0){

                std::vector<size_t> children_to_mutate_indexes = selectIndexesToMutate(children.size(), params);

                performMutation(children,
                                children_to_mutate_indexes,
                                params,
                                instance);
            }

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

        log.info("Genetic Algorithm completed. Final best known solution :");


        best_individual.print(instance);

        std::cout << "\n time : " << timer.getElapsedTime() << " (s)"
                << " status : "  << best_individual.getStatus() 
                << " nb generations : " << current_generation << " \n\n";


    }
}
