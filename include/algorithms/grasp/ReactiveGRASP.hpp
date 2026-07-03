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
 * @file ReactiveGRASP.hpp
 * @brief defines the Reactive Greedy Randomized Adaptative Search Procedure (GRASP) method for the Set Packing Solver
 * @author Josué Tambwe
 * @date 2 July 2026
 */

#pragma once

#include "configuration/GeneralConfig.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Parameters.hpp"
#include "output/Logger.hpp"
#include "algorithms/greedy/RandomizedConstruction.hpp"
#include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
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
     * @brief generates a solution by a greedy randomized construction and a Variable Neighboorhood Descent local search
     */
    Solution constructAndImproveSolution(float alpha, 
                                         const Params &params, 
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
     * @brief performs iterations (construction + local search) for a single CPU thread and return the best solution found
     */
    Solution runThreadIterations(const std::array<float, 10> &alpha_probabilities,
                                 std::array<float, 10> &alpha_maximum_scores,
                                 const int nb_iterations,
                                 const Params &params,
                                 const Instance &instance);



    /**
     * @brief synchornizes the alpha values maximum scores across all CPU threads
     */
    std::array<float, 10> synchronizeAlphaMaximumScores(const std::vector<std::array<float, 10>> &all_alpha_maximum_scores);




    /**
     * @brief synchronizes best solutions across all CPU threads
     */
    Solution synchronizeBestSolutions(const std::vector<Solution> &best_solutions, 
                                      const Instance &instance);


}