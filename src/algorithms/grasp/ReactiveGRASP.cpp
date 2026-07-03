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


    Solution constructAndImproveSolution(float alpha, 
                                         const Params &params, 
                                         const Instance &instance){
        
        // construction
        Solution solution = randomizedConstruction(alpha, instance);

        // local search
        variableNeighborhoodDescent(params.use_intensification,
                                    params, 
                                    solution, 
                                    instance);

        return solution;
    }




    std::array<float, 10> computeAlphaCumulativeProbabilities(const std::array<float, 10> &alpha_probabilities){

        std::array<float, 10> cumulative_probabilities{};
        float cumul_proba = 0.0f;

        for(size_t i = 0; i < alpha_probabilities.size(); i++){

            cumul_proba += alpha_probabilities[i];

            cumulative_probabilities[i] = cumul_proba;
        }

        return cumulative_probabilities;
    }




    size_t selectAlphaIndexRandomly(const std::array<float, 10> &cumulative_probabilities){

        // initialization of the thread random number generator (rng)
        auto& rng = getThreadLocalRng();
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        float random_number = dist(rng);

        for(size_t index = 0; index < cumulative_probabilities.size(); index++){

            if(cumulative_probabilities[index] >= random_number){return index;}
        }

        // the random number = 1
        return (cumulative_probabilities.size() - 1);


    }




    Solution runThreadIterations(const std::array<float, 10> &alpha_probabilities,
                                 std::array<float, 10> &alpha_maximum_scores,
                                 const int nb_iterations,
                                 const Params &params,
                                 const Instance &instance){

        std::array<float, 10> cumulative_probabilities = computeAlphaCumulativeProbabilities(alpha_probabilities);
        size_t alpha_index = selectAlphaIndexRandomly(cumulative_probabilities);
        float alpha = alpha_values[alpha_index];


        // initialization of the best solution
        Solution best_solution = constructAndImproveSolution(alpha, 
                                                             params, 
                                                             instance);

        std::int64_t best_solution_objective_value = best_solution.getObjectiveValue(instance);

        // update of the maximum alpha value score
        alpha_maximum_scores[alpha_index] = std::max(alpha_maximum_scores[alpha_index], 
                                                     static_cast<float>(best_solution_objective_value));


        for(int iteration = 1; iteration < nb_iterations; iteration++){

            // computation of a solution (construction + improvement)
            alpha_index = selectAlphaIndexRandomly(cumulative_probabilities);
            alpha = alpha_values[alpha_index];
            Solution current_solution = constructAndImproveSolution(alpha, 
                                                                    params, 
                                                                    instance);

            std::int64_t current_solution_objective_value = current_solution.getObjectiveValue(instance);

            // update of the maximum alpha value score
            alpha_maximum_scores[alpha_index] = std::max(alpha_maximum_scores[alpha_index], 
                                                         static_cast<float>(current_solution_objective_value));

            // update of the best solution
            if(current_solution_objective_value > best_solution_objective_value){

                best_solution = current_solution;
                best_solution_objective_value = current_solution_objective_value;
            }
        }

        return best_solution;

    }




    std::array<float, 10> synchronizeAlphaMaximumScores(const std::vector<std::array<float, 10>> &all_alpha_maximum_scores){

        std::array<float, 10> alpha_maximum_scores = all_alpha_maximum_scores[0];

        for(size_t thread_id = 1; thread_id < all_alpha_maximum_scores.size(); thread_id++){

            for(size_t index = 0; index < alpha_maximum_scores.size(); index++){

                alpha_maximum_scores[index] = std::max(alpha_maximum_scores[index], 
                                                       all_alpha_maximum_scores[thread_id][index]);
            }
        }

        return alpha_maximum_scores;

    }



    Solution synchronizeBestSolutions(const std::vector<Solution> &best_solutions, 
                                      const Instance &instance){

        Solution elite_solution = best_solutions[0];
        std::int64_t elite_solution_objective_value = elite_solution.getObjectiveValue(instance);

        for(size_t i = 1; i < best_solutions.size(); i++){

            Solution current_solution = best_solutions[i];
            std::int64_t current_solution_objective_value = current_solution.getObjectiveValue(instance);

            if(current_solution_objective_value > elite_solution_objective_value){

                elite_solution = current_solution;
                elite_solution_objective_value = current_solution_objective_value;
            }
        }

        return elite_solution;
    }


}