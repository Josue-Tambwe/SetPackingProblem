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
 * @file GeneticAlgorithmUtils.cpp
 * @author Josué Tambwe
 * @date 6 August 2026
 */

#include "algorithms/geneticAlgorithm/GeneticAlgorithmUtils.hpp"


namespace spp{


    void computePopulationFitnessOneThread(int start,
                                           int end,
                                           std::vector<double> &population_fitness,
                                           std::vector<Solution> &population,
                                           const Instance &instance){

        for(int index = start; index <= end; index++){

            population_fitness[index] = static_cast<double>(population[index].getObjectiveValue(instance));
        }
    }





    std::vector<double> computePopulationFitness(std::vector<Solution> &population,
                                                 const Params &params,
                                                 const Instance &instance){
        
        // initialization
        std::vector<double> population_fitness(population.size());

        int work_size = static_cast<int>(population.size());
        int nb_threads_used = std::min(params.nb_threads, work_size);
        std::vector<std::thread> workers(nb_threads_used);

        for(int id = 0; id < nb_threads_used; id++){

            int start = start_index(id, work_size, nb_threads_used);
            int end = end_index(id, work_size, nb_threads_used);

            workers[id] = std::thread(computePopulationFitnessOneThread,
                                      start,
                                      end,
                                      std::ref(population_fitness),
                                      std::ref(population),
                                      std::ref(instance));
        }

        // waiting for all threads to finish
        for(auto &worker : workers){worker.join();}

        return population_fitness;
    }





    size_t getRandomIndex(const std::unordered_set<size_t> &indexes){

        auto& rng = getThreadLocalRng();
        std::uniform_int_distribution<size_t> dist(0, (indexes.size() - 1));

        size_t index = dist(rng);

        std::unordered_set<size_t>::const_iterator iterator = indexes.begin();
        std::advance(iterator, index);

        return *iterator;
    }





    bool stoppingCriteriaGeneticAlgorithm(double current_time, 
                                          size_t current_generation, 
                                          const Params &params){

        if(params.use_time_limit && !params.use_max_iterations){return current_time >= params.time_limit;}

        if(!params.use_time_limit && params.use_max_iterations){return current_generation >= params.nb_max_iterations;}

        else{
            return (current_generation >= params.nb_max_iterations) || (current_time >= params.time_limit);
        }

    }





    Solution findBestIndividual(std::vector<Solution> &population, 
                                const Instance &instance){


        Solution best_individual = population[0];
        std::int64_t best_individual_objective_value = best_individual.getObjectiveValue(instance);

        for(Solution individual : population){

            std::int64_t individual_objective_value = individual.getObjectiveValue(instance);

            if(individual_objective_value > best_individual_objective_value){

                best_individual_objective_value = individual_objective_value;
                best_individual = individual;

            }
        }

        return best_individual;

    }





    void computeMaximumMinimumAverageFitness(double &max_fitness,
                                             double &min_fitness,
                                             double &average_fitness,
                                             const std::vector<double> &fitness){

        // initialization
        max_fitness = fitness[0];
        min_fitness = fitness[0];
        average_fitness = 0.0;

        for(double value : fitness){

            average_fitness += value;
            max_fitness = std::max(value, max_fitness);
            min_fitness = std::min(value, min_fitness);
        }

        average_fitness = average_fitness / (std::max(static_cast<size_t>(1), fitness.size()));

    }





    void computeMaximumMinimumAverageMutationFitness(double &max_fitness,
                                                     double &min_fitness,
                                                     double &average_fitness,
                                                     const std::vector<size_t> children_to_mutate_indexes,
                                                     const std::vector<double> &fitness){

        if(children_to_mutate_indexes.size() == 0){return;}

        // initialization
        max_fitness = fitness[children_to_mutate_indexes[0]];
        min_fitness = fitness[children_to_mutate_indexes[0]];
        average_fitness = 0.0;

        for(size_t index : children_to_mutate_indexes){

            double value = fitness[index];
            average_fitness += value;
            max_fitness = std::max(value, max_fitness);
            min_fitness = std::min(value, min_fitness);
        }

        average_fitness = average_fitness / (std::max(static_cast<size_t>(1), children_to_mutate_indexes.size()));

    }
    
}