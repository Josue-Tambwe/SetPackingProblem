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


    void runSingleThreadIterationsPathRelinking(const std::array<float, 10> &alpha_probabilities,
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
        Solution best_solution = constructAndImproveSolution(alpha, 
                                                             params, 
                                                             instance);

        std::int64_t best_solution_objective_value = best_solution.getObjectiveValue(instance);

        // update of alpha parameters
        alpha_cumulative_scores[alpha_index] += static_cast<float>(best_solution_objective_value);
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
            if(current_solution_objective_value > best_solution_objective_value){

                best_solution = current_solution;
                best_solution_objective_value = current_solution_objective_value;
            }
        }

        all_local_best_solutions[thread_id] = best_solution;

    }
}

