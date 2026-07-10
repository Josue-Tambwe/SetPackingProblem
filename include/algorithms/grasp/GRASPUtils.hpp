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
 * @file GRASPUtils.hpp
 * @brief defines some functions used in the Reactive GRASP algorithm
 * @author Josué Tambwe
 * @date 10 July 2026
 */

#pragma once

#include "configuration/GeneralConfig.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Parameters.hpp"
#include "algorithms/greedy/RandomizedConstruction.hpp"
#include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
#include "hpc/thread/Thread.hpp"
#include <array>
#include <vector>
#include <algorithm> 
#include <cstdint>

namespace spp{

    /**
     * @brief different alpha values used in the GRASP algorithm
     */
    const std::array<float, 10> alpha_values = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f,0.9f, 1.0f};




    /**
     * @brief generates a solution by a greedy randomized construction and a Variable Neighboorhood Descent (VND) local search
     */
    Solution constructAndImproveSolution(float alpha, 
                                         const Params &params, 
                                         const Instance &instance);




    /**
     * @brief provides an initial elite solution by a greedy randomized construction and VND
     */
    Solution initializeEliteSolution(const Params &params, 
                                     const Instance &instance);




    /**
     * @brief computes the array of the alpha values cumulative probabilities
     */
    std::array<float, 10> computeAlphaCumulativeProbabilities(const std::array<float, 10> &alpha_probabilities);





    /**
     * @brief selects randomdly the index of an alpha value using the alpha values cumulative  distribution
     */
    size_t selectAlphaIndexRandomly(const std::array<float, 10> &cumulative_probabilities);




    /**
     * @brief synchornizes the alpha values scores across all CPU threads
     */
    std::array<float, 10> synchronizeAlphaScores(const std::vector<std::array<float, 10>> &all_alpha_cumulative_scores,
                                                 const std::vector<std::array<float, 10>> &all_alpha_selection_count);


    /**
     * @brief synchronizes best solutions across all CPU threads
     */
    Solution synchronizeBestSolutions(const std::vector<Solution> &all_best_solutions, 
                                      const Instance &instance);



    /**
     * @brief finds the lowest alpha value score
     */
    float findMinimumScore(const std::array<float, 10> &alpha_scores);





    /**
     * @brief computes computes biased scores that emphasize the highest values.
     */
    void computeBiasedScores(std::array<float, 10> &alpha_scores, 
                             const Params &params);





    /**
     * @brief computes the inverse cumulative score in order to nomarlize scores
     */
    float computeInverseCumulativeScore(const std::array<float, 10> &alpha_scores);





    /**
     * @brief updates alpha values probabilities after a complete iteration of the GRASP algorithm 
     */
    void updateAlphaProbabilities(std::array<float, 10> &alpha_scores, 
                                  std::array<float, 10> &alpha_probabilities,
                                  const Params &params);


}