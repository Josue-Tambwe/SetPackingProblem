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
 * @file PathRelinking.cpp
 * @author Josué Tambwe
 * @date 11 July 2026
 */

#include "algorithms/grasp/PathRelinking.hpp"

namespace spp{


    void runSingleThreadLocalEliteGeneration(const std::array<float, 10> &alpha_probabilities,
                                             std::array<float, 10> &alpha_cumulative_scores,
                                             std::array<float, 10> &alpha_selection_count,
                                             const int nb_iterations,
                                             std::vector<Solution> &all_local_best_solutions,
                                             int thread_id,
                                             const Params &params,
                                             const Instance &instance){

        std::array<float, 10> cumulative_probabilities = computeAlphaCumulativeProbabilities(alpha_probabilities);

        // initialization of the local elite solution
        size_t alpha_index = selectAlphaIndexRandomly(cumulative_probabilities);
        float alpha = alpha_values[alpha_index];
        Solution local_best_solution = constructAndImproveSolution(alpha, 
                                                                   params, 
                                                                   instance);

        std::int64_t local_best_solution_objective_value = local_best_solution.getObjectiveValue(instance);

        // update of alpha parameters
        alpha_cumulative_scores[alpha_index] += static_cast<float>(local_best_solution_objective_value);
        alpha_selection_count[alpha_index] += 1.0f;

        for(int iteration = 1; iteration < nb_iterations; iteration++){

            // computation of a solution (construction + improvement)
            alpha_index = selectAlphaIndexRandomly(cumulative_probabilities);
            alpha = alpha_values[alpha_index];

            Solution current_solution = constructAndImproveSolution(alpha, 
                                                                    params, 
                                                                    instance);

            std::int64_t current_solution_objective_value = current_solution.getObjectiveValue(instance);

            // update of alpha parameters
            alpha_cumulative_scores[alpha_index] += static_cast<float>(current_solution_objective_value);
            alpha_selection_count[alpha_index] += 1.0f;


            // update of the best solution
            if(current_solution_objective_value > local_best_solution_objective_value){

                local_best_solution = current_solution;
                local_best_solution_objective_value = current_solution_objective_value;
            }
        }

        all_local_best_solutions[thread_id] = local_best_solution;

    }





    std::vector<Solution> runMultiThreadLocalEliteGeneration(std::array<float, 10> &alpha_probabilities,
                                                             Solution &current_elite_solution,
                                                             const Params &params,
                                                             const Instance &instance){

        // initialization
        std::array<float, 10> alpha_selection_count{};
        std::array<float, 10> alpha_scores{};

        int work_size = static_cast<int>(params.update_interval);
        int nb_threads_used = std::min(params.nb_threads, work_size);
        std::vector<std::thread> workers(nb_threads_used);

        std::vector<Solution> all_local_best_solutions(nb_threads_used, current_elite_solution);
        std::vector<std::array<float, 10>> all_alpha_cumulative_scores(nb_threads_used, alpha_scores);
        std::vector<std::array<float, 10>> all_alpha_selection_count(nb_threads_used, alpha_selection_count);

        for(int id = 0; id < nb_threads_used; id++){

            int start = start_index(id, work_size, nb_threads_used);
            int end = end_index(id, work_size, nb_threads_used);

            int nb_thread_iterations = (end - start) + 1; 

            workers[id] = std::thread(runSingleThreadLocalEliteGeneration,
                                      std::ref(alpha_probabilities),
                                      std::ref(all_alpha_cumulative_scores[id]),
                                      std::ref(all_alpha_selection_count[id]),
                                      nb_thread_iterations,
                                      std::ref(all_local_best_solutions),
                                      id,
                                      std::ref(params),
                                      std::ref(instance));
        }

        // waiting for all threads to finish
        for(auto &worker : workers){worker.join();}


        // synchronization
        alpha_scores = synchronizeAlphaScores(all_alpha_cumulative_scores,
                                              all_alpha_selection_count);

        // alpha probabilities update
        updateAlphaProbabilities(alpha_scores, 
                                 alpha_probabilities,
                                 params);

        return all_local_best_solutions;

    }





    size_t findGuidingSolutionIndex(Solution &current_elite_solution, 
                                    std::vector<Solution> &all_local_best_solutions,
                                    const Instance &instance){


        // initialization : guiding solution = current elite
        size_t guiding_solution_index = all_local_best_solutions.size();
        std::int64_t greatest_objective_value = current_elite_solution.getObjectiveValue(instance);

        for(size_t index = 0; index < all_local_best_solutions.size(); index++){

            std::int64_t local_best_solution_objective_value = all_local_best_solutions[index].getObjectiveValue(instance);

            // update of the guiding solution index
            if(local_best_solution_objective_value > greatest_objective_value){

                greatest_objective_value = local_best_solution_objective_value;
                guiding_solution_index = index;

            }
        }

        return guiding_solution_index;
    }





    Solution setGuidingSolution(size_t guiding_solution_index,
                                Solution &current_elite_solution,
                                std::vector<Solution> &all_local_best_solutions){

        // when the current elite is also the guiding solution
        if(guiding_solution_index == all_local_best_solutions.size()){

            return current_elite_solution;
        }

        // when the guiding solution is a new generated solution
        return all_local_best_solutions[guiding_solution_index];

    }






    std::vector<Solution> computeInitialSolutionPool(size_t guiding_solution_index, 
                                                     Solution &current_elite_solution, 
                                                     std::vector<Solution> &all_local_best_solutions){

        // initialization
        std::vector<Solution> initial_solutions_pool(all_local_best_solutions.size(), 
                                                     current_elite_solution);

        size_t internal_counter = 0;

        // when the current elite is not the guiding elite solution
        if(guiding_solution_index != all_local_best_solutions.size()){

            initial_solutions_pool[internal_counter] = current_elite_solution;
            internal_counter += 1;
        }

        for(size_t index = 0; index < all_local_best_solutions.size(); index++){

            if(index != guiding_solution_index){

                initial_solutions_pool[internal_counter] = all_local_best_solutions[index];
                internal_counter += 1;
                
            }

        }

        return initial_solutions_pool;

    }




    std::unordered_set<int> computeInitialSolutionNonZeroVarsIndexes(Solution &initial_solution){

        std::unordered_set<int> non_zero_vars_set;
        std::vector<int> non_zero_vars_vector = initial_solution.getNonZeroVarsIndexes();

        for(int index : non_zero_vars_vector){non_zero_vars_set.insert(index);}

        return non_zero_vars_set;
    }




    void deactivateConflictingVariables(int index_to_activate, 
                                        std::unordered_set<int> &non_zero_vars_set,
                                        Solution &solution, 
                                        const Instance &instance){

        // getting all conflicting variables indexes
        std::vector<int> conflicting_vars = instance.getAllConflictingVarsIndexes(index_to_activate);

        // deactivation of those conflicting variables 
        for(int var : conflicting_vars){

            if(non_zero_vars_set.count(var)){

                solution.deactivateVar(var, instance);
                non_zero_vars_set.erase(var);
            }
            
        }
    }




    void exploreRelinkingPath(Solution &initial_solution,
                              Solution &guiding_solution,
                              Solution &local_elite_solution,
                              const Params &params,
                              const Instance &instance){

        // to performs an intensified VND 
        bool use_intensified_local_search = true;

        // getting non-zero variables within the guiding solution
        std::vector<int> guiding_solution_non_zero_vars = guiding_solution.getNonZeroVarsIndexes();

        // getting non-zero variables within the initial solution
        std::unordered_set<int> initial_solution_non_zero_vars = computeInitialSolutionNonZeroVarsIndexes(initial_solution);

        // the local elite solution
        std::int64_t local_elite_solution_objective_value = local_elite_solution.getObjectiveValue(instance);
        std::int64_t guiding_solution_objective_value = guiding_solution.getObjectiveValue(instance);


        for(int var_in_guiding_solution : guiding_solution_non_zero_vars){

            // when the a non-zero variable in the guiding solution is deactivated in the initial solution
            if(!initial_solution_non_zero_vars.count(var_in_guiding_solution)){

                // deactivation of all conflicting variables in order to maintain feasibility
                deactivateConflictingVariables(var_in_guiding_solution,
                                               initial_solution_non_zero_vars, 
                                               initial_solution, 
                                               instance);

                // activation of the variable in the initial solution
                initial_solution.activateVar(var_in_guiding_solution, instance);

                // update of the set of non-zero variables within the initial solution
                initial_solution_non_zero_vars.insert(var_in_guiding_solution);

                std::int64_t intermediate_solution_objective_value = initial_solution.getObjectiveValue(instance);

                // VDN local search on a promissing intermediate solution (better than the guiding solution)
                if(intermediate_solution_objective_value > guiding_solution_objective_value){

                    // the intermediate promissing solution
                    Solution intermediate_promissing_solution = initial_solution;

                    // local search on the intermediate solution
                    variableNeighborhoodDescent(use_intensified_local_search,
                                                params, 
                                                intermediate_promissing_solution, 
                                                instance);

                    // update of the intermediate solution objective value after VND local search
                    intermediate_solution_objective_value = intermediate_promissing_solution.getObjectiveValue(instance);

                    // update of the local elite solution
                    if(intermediate_solution_objective_value > local_elite_solution_objective_value){

                        local_elite_solution_objective_value = intermediate_solution_objective_value;
                        local_elite_solution = intermediate_promissing_solution;
                    }
                }


            }

        }


    }





    std::vector<Solution> exploreMultiRelinkingPath(std::vector<Solution> &initial_solutions_pool,
                                                    Solution &guiding_solution,
                                                    const Params &params,
                                                    const Instance &instance){

        // the pool of local elite solutions (the guiding solution)
        std::vector<Solution> local_elite_pool(initial_solutions_pool.size(), guiding_solution);

        // the pool of CPU threads to use 
        std::vector<std::thread> workers(initial_solutions_pool.size());

        for(size_t id = 0; id < workers.size(); id++){

            workers[id] = std::thread(exploreRelinkingPath,
                                      std::ref(initial_solutions_pool[id]),
                                      std::ref(guiding_solution),
                                      std::ref(local_elite_pool[id]),
                                      std::ref(params),
                                      std::ref(instance));

        }

        // waiting for all threads to finish
        for(auto &worker : workers){worker.join();}

        return local_elite_pool;


    }





    Solution retrieveEliteSolution(const std::vector<Solution> &local_elite_pool,
                                   const Instance &instance){

        Solution elite = local_elite_pool[0];
        std::int64_t elite_objective_value = elite.getObjectiveValue(instance);

        for(Solution solution : local_elite_pool){

            std::int64_t solution_objective_value = solution.getObjectiveValue(instance);

            if(solution_objective_value > elite_objective_value){

                elite = solution;
                elite_objective_value = solution_objective_value;
            }
        }


        return elite;

    }





    Solution runOnePathRelinkingCycle(std::array<float, 10> &alpha_probabilities,
                                      Solution &current_elite_solution,
                                      const Params &params,
                                      const Instance &instance){

        // generation of local best solutions
        std::vector<Solution> all_local_best_solutions = runMultiThreadLocalEliteGeneration(alpha_probabilities,
                                                                                            current_elite_solution,
                                                                                            params,
                                                                                            instance);
        // finding the guiding solution index
        size_t guiding_solution_index = findGuidingSolutionIndex(current_elite_solution, 
                                                                 all_local_best_solutions,
                                                                 instance);

        // setting the guiding solution
        Solution guiding_solution = setGuidingSolution(guiding_solution_index,
                                                       current_elite_solution,
                                                       all_local_best_solutions);

        // computation of the intial solutions pool
        std::vector<Solution> initial_solutions_pool = computeInitialSolutionPool(guiding_solution_index, 
                                                                                  current_elite_solution, 
                                                                                  all_local_best_solutions);

        // computation of local elite solution
        std::vector<Solution> local_elite_pool = exploreMultiRelinkingPath(initial_solutions_pool,
                                                                           guiding_solution,
                                                                           params,
                                                                           instance);

        // retrieving the gobal elite solution
        return retrieveEliteSolution(local_elite_pool, instance);

    }


    



}

