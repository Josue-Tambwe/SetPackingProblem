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

            if(index != internal_counter){

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



}

