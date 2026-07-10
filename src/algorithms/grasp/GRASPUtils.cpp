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
 * @file GRASPUtils.cpp
 * @author Josué Tambwe
 * @date 10 July 2026
 */

#include "algorithms/grasp/GRASPUtils.hpp"


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




    Solution initializeEliteSolution(const Params &params, 
                                     const Instance &instance){

        // initialization of the thread random number generator (rng)
        auto& rng = getThreadLocalRng();
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        float alpha = dist(rng);

        return constructAndImproveSolution(alpha, 
                                           params, 
                                           instance);

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




    std::array<float, 10> synchronizeAlphaScores(const std::vector<std::array<float, 10>> &all_alpha_cumulative_scores,
                                                 const std::vector<std::array<float, 10>> &all_alpha_selection_count){

        std::array<float, 10> alpha_scores = all_alpha_cumulative_scores[0];
        std::array<float, 10> alpha_selection_count = all_alpha_selection_count[0];

        // adding up all cumulative scores and selection counts
        for(size_t thread_id = 1; thread_id < all_alpha_cumulative_scores.size(); thread_id++){

            for(size_t index = 0; index < alpha_scores.size(); index++){

                alpha_scores[index] += all_alpha_cumulative_scores[thread_id][index];
                alpha_selection_count[index] += all_alpha_selection_count[thread_id][index];
            }

        }

        // computing the alpha score (average) : cumulative score / selection count

        for(size_t index = 0; index < alpha_scores.size(); index++){

            alpha_scores[index] = alpha_scores[index] / 
                                  (std::max(1.0f, alpha_selection_count[index]));
        }

        return alpha_scores;
                                                    
    }





    Solution synchronizeBestSolutions(const std::vector<Solution> &all_best_solutions, 
                                      const Instance &instance){

        Solution elite_solution = all_best_solutions[0];
        std::int64_t elite_solution_objective_value = elite_solution.getObjectiveValue(instance);

        for(size_t i = 1; i < all_best_solutions.size(); i++){

            Solution current_solution = all_best_solutions[i];
            std::int64_t current_solution_objective_value = current_solution.getObjectiveValue(instance);

            if(current_solution_objective_value > elite_solution_objective_value){

                elite_solution = current_solution;
                elite_solution_objective_value = current_solution_objective_value;
            }
        }

        return elite_solution;
    }




    float findMinimumScore(const std::array<float, 10> &alpha_scores){

        float min_score = alpha_scores[0];

        for(size_t i = 1; i < alpha_scores.size(); i++){

            min_score = std::min(min_score, alpha_scores[i]);
        }

        return min_score;
    }




    void computeBiasedScores(std::array<float, 10> &alpha_scores, 
                             const Params &params){

        float min_score = findMinimumScore(alpha_scores);

        for(size_t i = 0; i < alpha_scores.size(); i++){

            alpha_scores[i] = (alpha_scores[i] * (2.0f - params.bias)) - min_score;

        }
    }





    float computeInverseCumulativeScore(const std::array<float, 10> &alpha_scores){

        float cumulative_score = alpha_scores[0];

        for(size_t i = 1; i < alpha_scores.size(); i++){

            cumulative_score += alpha_scores[i];
        }

        return (1.0f / (cumulative_score + epsilon));

    }





    void updateAlphaProbabilities(std::array<float, 10> &alpha_scores, 
                                  std::array<float, 10> &alpha_probabilities,
                                  const Params &params){

        computeBiasedScores(alpha_scores, params);
        float inverse_cumaltive_score = computeInverseCumulativeScore(alpha_scores);

        for(size_t i = 0; i < alpha_scores.size(); i++){

            alpha_probabilities[i] = alpha_scores[i] * inverse_cumaltive_score;
        }


    }

}