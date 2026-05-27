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
 #include "dataStructures/Instance.hpp"
 #include "dataStructures/Solution.hpp"
 #include <vector>
 #include <algorithm>
 #include <functional>

 namespace spp{

    /**
     * @brief computes the  heuristic score of all variables
     *        heuristic score = profit / nb resource required
     */
    std::vector<float> computeVariableScores(const Instance &instance);



    /**
     * @brief sort all nonzero (1) variables by a increasing order of their heuristic score
     *        heuristic score = profit / nb resource required
     */
    std::vector<int> sortNonZeroVars(Solution &solution,
                                     std::vector<float> &scores);


    /**
     * @brief sort all zero (0) variables by a deacreasing order of their heuristic score
     *        heuristic score = profit / nb resource required
     */
    std::vector<int> sortZeroVars(Solution &solution,
                                  std::vector<float> &scores);

 }