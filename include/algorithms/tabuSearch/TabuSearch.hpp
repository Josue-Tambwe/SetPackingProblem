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
 * @file TabuSearch.hpp
 * @brief defines  functions used in the Tabu Search local search
 * @author Josué Tambwe
 * @date 25 July 2026
 */

#pragma once

#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Parameters.hpp"
#include "dataStructures/TabuList.hpp"
#include "algorithms/localSearch/Neighborhoods.hpp"
#include "algorithms/localSearch/RestrictedNeighborhoods.hpp"
#include <vector>
#include <cstdint>

namespace spp{


    /**
     * @brief Identifies the best admissible move in the neighborhood which means the best non‑tabu move that improves
     *        or least degrades the current solution according to the evaluation criteria.
     */
    TabuMove findBestMove(TabuList &tabu_list,
                          Solution &solution,
                          const Params &params,
                          const Instance &instance);

}