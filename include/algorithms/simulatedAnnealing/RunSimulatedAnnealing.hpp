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
 * @file RunSimulatedAnnealing.hpp
 * @brief runs the Simulated Annealing algorithm with a greedy randomized construction
 * @author Josué Tambwe
 * @date 2 August 2026
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
#include "output/SimulatedAnnealingOutput.hpp"
#include "algorithms/greedy/RandomizedConstruction.hpp"
#include "algorithms/simulatedAnnealing/SimulatedAnnealingNeighborhoods.hpp"
#include "algorithms/simulatedAnnealing/SimulatedAnnealingUtils.hpp"
#include <cstdint>


namespace spp{


    /**
     * @brief computes the relative improvement of a single iteration on the incumbent solution
     */
    double computeRelativeImprovementSimulatedAnnealing(std::int64_t new_objective_value, 
                                                        std::int64_t old_objective_value);




    /**
     * @brief computes the relative degradation of a single iteration on the incumbent solution
     */
    double computeRelativeDegradationSimulatedAnnealing(std::int64_t new_objective_value, 
                                                        std::int64_t old_objective_value);




    /**
     * @brief runs the Simulated Annealing algorithm
     */
    void runSimulatedAnnealing(const Params &params);

}