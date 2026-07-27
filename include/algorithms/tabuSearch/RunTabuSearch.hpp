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
 * @file RunTabuSeach.hpp
 * @brief runs the Tabu Search algorithm with a greedy randomized construction
 * @author Josué Tambwe
 * @date 27 July 2026
 */

#pragma once

#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Parameters.hpp"
#include "dataStructures/TabuList.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Timer.hpp"
#include "output/Logger.hpp"
#include "output/GeneralOutput.hpp"
#include "algorithms/greedy/RandomizedConstruction.hpp"
#include "algorithms/tabuSearch/TabuSearchNeighborhoods.hpp"
#include "algorithms/tabuSearch/TabuSearchUtils.hpp"
#include <cstdint>
#include <iostream> // to remove


namespace spp{


    /**
     * @brief runs the Tabu Search algorithm
     */
    void runTabuSearch(const Params &params);


}

