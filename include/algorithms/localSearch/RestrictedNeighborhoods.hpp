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
 * @file RestrictedNeighborhoods.hpp
 * @brief defines restricted 1-2 and 2-1 neighborhoods 
 * @author Josué Tambwe
 * @date 18 July 2026
 */

#pragma once

#include "dataStructures/MoveNode.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Parameters.hpp"
#include "algorithms/localSearch/NeighborhoodUtils.hpp"
#include "algorithms/localSearch/ConflictCheckerScalar.hpp"
#include <vector>
#include <algorithm>


namespace spp{



    /**
     * @brief searches a feasible 2-1 exchange move : 2 variables to deactivate and 1 variable to activate
     *        the vector of currently activated variables (nonzero variables) is sorted in increasing order of the heuristic score
     *        the vector of currently deactivated variables (zero variables) is sorted in decreasing order of the heuristic score
     *        heuristic score : profit / nb resources required
     *
     *        To reduce the neighborhood size, only a given percentage of the best deactivated variables is considered.
     *        A second linear pruning is applied inside the inner loop, significantly lowering the complexity of the search.
     */
    bool findRestrictedTwoOneExchange(int &first_index_to_deactivate,
                                    int &second_index_to_deactivate,
                                    int &index_to_activate,
                                    std::vector<int> &sorted_activated_vars,
                                    std::vector<int> &sorted_deactivated_vars,
                                    Solution &solution,
                                    const Params &params,
                                    const Instance &instance);


}


