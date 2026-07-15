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
 * @file ReactiveGRASP.cpp
 * @author Josué Tambwe
 * @date 2 July 2026
 */

#include "algorithms/grasp/ReactiveGRASP.hpp"


namespace spp{


    void runSingleThreadIterations(const std::array<float, 10> &alpha_probabilities,
                                    std::array<float, 10> &alpha_cumulative_scores,
                                    std::array<float, 10> &alpha_selection_count,
                                    const int nb_iterations,
                                    std::vector<Solution> &all_best_solutions,
                                    int thread_id,
                                    const Params &params,
                                    const Instance &instance){

        std::array<float, 10> cumulative_probabilities = computeAlphaCumulativeProbabilities(alpha_probabilities);
        
        // initialization of the best solution
        Solution best_solution = all_best_solutions[thread_id];
        std::int64_t best_solution_objective_value = best_solution.getObjectiveValue(instance);

        for(int iteration = 1; iteration < nb_iterations; iteration++){

            // computation of a solution (construction + improvement)
            size_t alpha_index = selectAlphaIndexRandomly(cumulative_probabilities);
            float alpha = alpha_values[alpha_index];
            Solution current_solution = constructAndImproveSolution(alpha, 
                                                                    params, 
                                                                    instance);

            std::int64_t current_solution_objective_value = current_solution.getObjectiveValue(instance);

            // update of alpha parameters
            alpha_cumulative_scores[alpha_index] += static_cast<float>(current_solution_objective_value);
            alpha_selection_count[alpha_index] += 1.0f;


            // update of the best solution
            if(current_solution_objective_value > best_solution_objective_value){

                best_solution = current_solution;
                best_solution_objective_value = current_solution_objective_value;
            }
        }

        all_best_solutions[thread_id] = best_solution;


    }





    Solution runOneReactiveGRASPCycle(std::array<float, 10> &alpha_probabilities,
                                      Solution &best_solution,
                                      const Params &params,
                                      const Instance &instance){

        // initialization
        std::array<float, 10> alpha_selection_count{};
        std::array<float, 10> alpha_scores{};

        int work_size = static_cast<int>(params.update_interval);
        int nb_threads_used = std::min(params.nb_threads, work_size);
        std::vector<std::thread> workers(nb_threads_used);

        std::vector<Solution> all_best_solutions(nb_threads_used, best_solution);
        std::vector<std::array<float, 10>> all_alpha_cumulative_scores(nb_threads_used, alpha_scores);
        std::vector<std::array<float, 10>> all_alpha_selection_count(nb_threads_used, alpha_selection_count);

        for(int id = 0; id < nb_threads_used; id++){

            int start = start_index(id, work_size, nb_threads_used);
            int end = end_index(id, work_size, nb_threads_used);

            int nb_thread_iterations = (end - start) + 1; 

            workers[id] = std::thread(runSingleThreadIterations,
                                      std::ref(alpha_probabilities),
                                      std::ref(all_alpha_cumulative_scores[id]),
                                      std::ref(all_alpha_selection_count[id]),
                                      nb_thread_iterations,
                                      std::ref(all_best_solutions),
                                      id,
                                      std::ref(params),
                                      std::ref(instance));
        }

        // waiting for all threads to finish
        for(auto &worker : workers){worker.join();}


        // synchronization
        Solution elite_solution = synchronizeBestSolutions(all_best_solutions, instance);
        alpha_scores = synchronizeAlphaScores(all_alpha_cumulative_scores,
                                              all_alpha_selection_count);

        // alpha probabilities update
        updateAlphaProbabilities(alpha_scores, 
                                 alpha_probabilities,
                                 params);

        return elite_solution;

    }


}
