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
 * @file Crossover.cpp
 * @author Josué Tambwe
 * @date 6 August 2026
 */

#include "algorithms/geneticAlgorithm/Crossover.hpp"


namespace spp{


    double findMinimumFitness(const std::vector<double> &population_fitness){

        double minimum_fitness = std::numeric_limits<double>::max();

        for(double fitness : population_fitness){

            if(fitness < minimum_fitness){minimum_fitness = fitness;}
        }

        return minimum_fitness;
    }




    std::vector<double> computeQuadraticBiasedFitness(const std::vector<double> &population_fitness){

        double minimum_fitness = findMinimumFitness(population_fitness);

        std::vector<double> biased_population_fitness(population_fitness.size());

        for(size_t i = 0; i < population_fitness.size(); i++){

            biased_population_fitness[i] = (population_fitness[i] + 1e-6) - minimum_fitness;

            biased_population_fitness[i] *= biased_population_fitness[i];

        }

        return biased_population_fitness;
    }





    void normalizeBiasedFitness(std::vector<double> &biased_population_fitness){

        // computation of the cumulative fitness
        
        double cumulative_fitness = 0.0;

        for(double fitness : biased_population_fitness){cumulative_fitness += fitness;}

        // computation of the inverse cumulative fitness
        double inverse_cumulative_fitness = 1.0 / (cumulative_fitness + 1e-9);

        for(size_t i = 0; i < biased_population_fitness.size(); i++){

            biased_population_fitness[i] = biased_population_fitness[i] * inverse_cumulative_fitness;
        }

    }





    std::vector<double> computeCumulativeQuadraticBiasedFitness(const std::vector<double> &population_fitness){

        // computation of quadratic biased fitness
        std::vector<double> biased_population_fitness = computeQuadraticBiasedFitness(population_fitness);

        // fitness normalization
        normalizeBiasedFitness(biased_population_fitness);

        double cumulative_fitness = 0.0;

        std::vector<double> cumulative_population_fitness(population_fitness.size());

        for(size_t i = 0; i < biased_population_fitness.size(); i++){

            cumulative_fitness += biased_population_fitness[i];

            cumulative_population_fitness[i] = cumulative_fitness;

        }

        return cumulative_population_fitness;
    }





    std::vector<size_t> computeIndividualsCrossOverParticipation(const std::vector<double> &cumulative_population_fitness,
                                                                 const Params &params){

        size_t total_crossover_participation = static_cast<size_t>(std::floor((cumulative_population_fitness.size() * params.crossover_rate) / 2.0) * 2.0);

        double inverse = 1.0 / (total_crossover_participation + 1e-9);

        // initialization of the thread random number generator
        auto& rng = getThreadLocalRng();

        // initialization of the uniform real distribution
        std::uniform_real_distribution<double> dist(0.0, inverse);

        double random_number = dist(rng);

        std::vector<size_t> crossover_participation(cumulative_population_fitness.size());

        size_t i = 0;

        for(size_t j = 0; j < total_crossover_participation; j++){

            double pointer = random_number + (j * inverse);

            while((i < cumulative_population_fitness.size()) && 
                  (cumulative_population_fitness[i] < pointer )){

                    i += 1;
            }

            crossover_participation[i] += 1;
        }

        return crossover_participation;
    }





    void setInitialAndGuidingParents(size_t inner_index,
                                     size_t first_parent_index,
                                     size_t second_parent_index,
                                     std::vector<size_t> &initial_parents,
                                     std::vector<size_t> &guiding_parents,
                                     const std::vector<double> &population_fitness){

        if(population_fitness[first_parent_index] < population_fitness[second_parent_index]){

            initial_parents[inner_index] = first_parent_index;
            guiding_parents[inner_index] = second_parent_index;

        }

        else{

            initial_parents[inner_index] = second_parent_index;
            guiding_parents[inner_index] = first_parent_index;
        }

    }






    std::unordered_set<size_t> getParentIndexes(const std::vector<size_t> &crossover_participation){

        std::unordered_set<size_t> parents_indexes;

        for(size_t i = 0; i < crossover_participation.size(); i++){

            if(crossover_participation[i] > 0){parents_indexes.insert(i);}
        }

        return parents_indexes;
    }




    size_t getRandomIndex(const std::unordered_set<size_t> &parents_indexes){

        auto& rng = getThreadLocalRng();
        std::uniform_int_distribution<size_t> dist(0, (parents_indexes.size() - 1));

        size_t index = dist(rng);

        std::unordered_set<size_t>::const_iterator iterator = parents_indexes.begin();
        std::advance(iterator, index);

        return *iterator;
    }





    void setCrossoverCouples(std::vector<size_t> &initial_parents,
                             std::vector<size_t> &guiding_parents,
                             std::vector<size_t> &crossover_participation,
                             const std::vector<double> &population_fitness,
                             const Params &params){


        size_t nb_couples = static_cast<size_t>(std::floor(population_fitness.size() * params.crossover_rate) / 2.0);

        initial_parents.resize(nb_couples);
        guiding_parents.resize(nb_couples);

        std::unordered_set<size_t> parents_indexes = getParentIndexes(crossover_participation);

        for(size_t inner_index = 0; inner_index < nb_couples; inner_index++){

            // selection of the first parent
            size_t first_parent_index = getRandomIndex(parents_indexes);
            parents_indexes.erase(first_parent_index);
            crossover_participation[first_parent_index] -= 1;

            // selection of the second parent
            size_t second_parent_index = getRandomIndex(parents_indexes);
            parents_indexes.erase(second_parent_index);
            crossover_participation[second_parent_index] -= 1;

            setInitialAndGuidingParents(inner_index,
                                        first_parent_index,
                                        second_parent_index,
                                        initial_parents,
                                        guiding_parents,
                                        population_fitness);


            if(crossover_participation[first_parent_index] > 0){parents_indexes.insert(first_parent_index);}

            if(crossover_participation[second_parent_index] > 0){parents_indexes.insert(second_parent_index);}

        }


        for(size_t inner_index = 0; inner_index < nb_couples; inner_index++){

            std::cout << " couple : " << (inner_index + 1)
                        << " ( " << (initial_parents[inner_index] + 1)
                        << " - "
                        << (guiding_parents[inner_index] + 1)
                        << ") \n\n";
        }

    }





    void performPathRelinkingCrossover(Solution &initial_parent,
                                       Solution &guiding_parent,
                                       std::vector<Solution> &children,
                                       const Params &params,
                                       const Instance &instance){

        // copy of the initial parent
        Solution initial_parent_copy = initial_parent;

        // to performs an intensified VND 
        bool use_intensified_local_search = params.use_intensification;

        // getting non-zero variables within the guiding parent
        std::vector<int> guiding_parent_non_zero_vars = guiding_parent.getNonZeroVarsIndexes();

        // getting non-zero variables within the initial parent
        std::unordered_set<int> initial_parent_non_zero_vars = computeInitialSolutionNonZeroVarsIndexes(initial_parent_copy);

        std::int64_t initial_parent_objective_value = initial_parent_copy.getObjectiveValue(instance);

        size_t children_counter = 0;
        size_t max_children_per_couple = children.size();
        size_t inner_child_index = 0;

        for(int var_in_guiding_parent : guiding_parent_non_zero_vars){

            // when the a non-zero variable in the guiding parent is deactivated in the initial parent
            if(!initial_parent_non_zero_vars.count(var_in_guiding_parent)){

                // deactivation of all conflicting variables in order to maintain feasibility
                deactivateConflictingVariables(var_in_guiding_parent,
                                               initial_parent_non_zero_vars, 
                                               initial_parent_copy, 
                                               instance);

                // activation of the variable in the initial parent
                initial_parent_copy.activateVar(var_in_guiding_parent, instance);

                // update of the set of non-zero variables within the initial parent
                initial_parent_non_zero_vars.insert(var_in_guiding_parent);

                std::int64_t child_objective_value = initial_parent_copy.getObjectiveValue(instance);

                // VDN local search on a promissing intermediate child (better than the initial solution)
                if(child_objective_value > initial_parent_objective_value){

                    // the intermediate promissing child
                    Solution intermediate_promissing_child = initial_parent_copy;

                    // local search on the intermediate solution
                    variableNeighborhoodDescent(use_intensified_local_search,
                                                params, 
                                                intermediate_promissing_child, 
                                                instance);


                    // adding the child within the children vector
                    children[inner_child_index] = intermediate_promissing_child;
                    inner_child_index += 1;
                    children_counter += 1;
                }
            }

            if(children_counter >= max_children_per_couple){break;}
        }
    }





    void performCrossoverSingleThread(int start,
                                      int end,
                                      std::vector<Solution> &population,
                                      std::vector<std::vector<Solution>> all_children,
                                      const std::vector<size_t> &initial_parent_indexes,
                                      const std::vector<size_t> &guiding_parent_indexes,
                                      const Params &params,
                                      const Instance &instance){

        for(int i = start; i <= end; i++){

            performPathRelinkingCrossover(population[initial_parent_indexes[i]],
                                          population[guiding_parent_indexes[i]],
                                          all_children[i],
                                          params,
                                          instance);

        }

    }





    

    std::vector<Solution> extractFeasibleChildren(std::vector<std::vector<Solution>> &all_children,
                                                  const Instance &instance){

        size_t feasible_children_counter = 0;

        // counting feasible children

        for(size_t couple_index = 0; couple_index < all_children.size(); couple_index++){

            for(size_t child_index = 0; child_index < all_children[couple_index].size(); child_index++){

                if(all_children[couple_index][child_index].getStatus() == Status::FEASIBLE){

                    feasible_children_counter += 1;
                }

            }
        }

        // extracting feasible children

        std::vector<Solution> feasible_children(feasible_children_counter, Solution(instance));

        size_t inner_index = 0;

        for(size_t couple_index = 0; couple_index < all_children.size(); couple_index++){

            for(size_t child_index = 0; child_index < all_children[couple_index].size(); child_index++){

                if(all_children[couple_index][child_index].getStatus() == Status::FEASIBLE){

                    feasible_children[inner_index] = all_children[couple_index][child_index];

                    inner_index += 1;
                }

            }
        }

        return feasible_children;

    }







    std::vector<Solution> performCrossover(std::vector<Solution> &population,
                                           const std::vector<size_t> &initial_parent_indexes,
                                           const std::vector<size_t> &guiding_parent_indexes,
                                           const Params &params,
                                           const Instance &instance){

        size_t nb_couples = initial_parent_indexes.size();

        size_t max_children_per_couple = 2;

        std::vector<std::vector<Solution>> all_children(nb_couples,
                                                        std::vector<Solution>(max_children_per_couple,
                                                                              Solution(instance)));

        int work_size = static_cast<int>(nb_couples);
        int nb_threads_used = std::min(params.nb_threads, work_size);
        std::vector<std::thread> workers(nb_threads_used);

        for(int id = 0; id < nb_threads_used; id++){

            int start = start_index(id, work_size, nb_threads_used);
            int end = end_index(id, work_size, nb_threads_used);

            workers[id] = std::thread(performCrossoverSingleThread,
                                      start,
                                      end,
                                      std::ref(population),
                                      std::ref(all_children),
                                      std::ref(initial_parent_indexes),
                                      std::ref(guiding_parent_indexes),
                                      std::ref(params),
                                      std::ref(instance));
        }

        // waiting for all threads to finish
        for(auto &worker : workers){worker.join();}

        // extraction of feasible children
        return extractFeasibleChildren(all_children, instance);

    }

    
}