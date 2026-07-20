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
 * @file NeighborhoodUtils.hpp
 * @brief defines some functions used in the local search
 * @author Josué Tambwe
 * @date 28 May 2026
 */

 #pragma once

 #include "dataStructures/MoveNode.hpp"
 #include "dataStructures/Parameters.hpp"
 #include "dataStructures/Instance.hpp"
 #include "dataStructures/Solution.hpp"
 #include <vector>
 #include <algorithm>
 #include <functional>
 #include <cmath>

 namespace spp{

    /**
     * @brief computes the  heuristic score of all variables
     *        heuristic score = profit / nb resource required
     */
    std::vector<float> computeVariableScores(const Instance &instance);



    /**
     * @brief sorts all nonzero (1) variables by a increasing order of their heuristic score
     *        heuristic score = profit / nb resource required
     */
    std::vector<int> sortNonZeroVars(Solution &solution,
                                     std::vector<float> &scores);


    /**
     * @brief sorts all zero (0) variables by a deacreasing order of their heuristic score
     *        heuristic score = profit / nb resource required
     */
    std::vector<int> sortZeroVars(Solution &solution,
                                  std::vector<float> &scores);




    /**
     * @brief computes the bound on the set of deactivated variables sorted in a decreasing order of their
     *        heuristic scores (score = profit / nb resources required)
     *
     *        The bound = (1 - pruning rate) * original bound
     */
    size_t computeDeactiatedVariablesBound(size_t original_bound, const Params &params);


   

    /**
     * @brief computes the index normalization ratio t = index / original bound  = index * (inverse original bound)
     */
    double computeIndexRatio(size_t index, double inverse_original_bound);





    /**
     * @brief computes the index bound when applied the linear pruning strategy  f(t) = (1 - t) with 't' = index ratio
     */
    size_t computeLinearPruningIndexBound(double index_ratio, size_t original_bound);




     /**
     * @brief computes the index bound when applied the quadratic pruning strategy  f(t) = (1 - t²) with 't' = index ratio
     */
    size_t computeQuadraticPruningIndexBound(double index_ratio, size_t original_bound);

 }