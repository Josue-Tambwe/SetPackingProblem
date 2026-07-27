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
 * @file TabuSearchUtils.hpp
 * @brief defines some useful functions used in the Tabu Search local search
 * @author Josué Tambwe
 * @date 27 July 2026
 */

#pragma once

#include "dataStructures/Parameters.hpp"
#include "dataStructures/TabuList.hpp"


namespace spp{


    /**
     * @brief updates the tabu list by adding the best non-tabu move found in the neighborhoods
     */
    void updateTabuList(const TabuMove &best_move, TabuList &tabu_list);




    /**
     * @brief check of the stopping criteria for the Tabu Search algorithm
     */
    bool stoppingCriteriaTabuSearch(double current_time, 
                                    size_t current_iteration, 
                                    const Params &params);



}