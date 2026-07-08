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
 * @file RunGRASP.hpp
 * @brief implements the Reactive Greedy Randomized Adaptative Search Procedure (GRASP)  for the Set Packing Solver
 * @author Josué Tambwe
 * @date 8 July 2026
 */

#pragma once

#include "configuration/GeneralConfig.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Parameters.hpp"
#include "output/Logger.hpp"
#include "dataStructures/Timer.hpp"
#include "algorithms/grasp/ReactiveGRASP.hpp"
#include "output/GeneralOutput.hpp"
#include "output/GRASPOutput.hpp"
#include <array>
#include <vector> 
#include <cstdint>


namespace spp{



    /**
     * @brief check of the stopping criteria for the Reactive GRASP algorithm
     */
    bool stoppingCriteriaGRASP(double current_time, 
                               size_t current_iteration, 
                               const Params &params);




    /**
     * @brief runs the Reactive Greedy Randomized Adaptative Search Procedure (GRASP) without Path-Relinking
     */
    void runReactiveGRASP(const Params &params);

}
