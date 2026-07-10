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
 * @file PathRelinking.hpp
 * @brief defines the Path-Relinking intensification method for the Reactive GRASP algorithm
 * @author Josué Tambwe
 * @date 11 July 2026
 */

#pragma once

#include "configuration/GeneralConfig.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Parameters.hpp"
#include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
#include "algorithms/grasp/GRASPUtils.hpp"
#include "hpc/thread/Thread.hpp"
#include <array>
#include <vector> 
#include <cstdint>


namespace spp{


    /**
     * @brief performs iterations (construction + local search) for a single CPU thread and saves the local best solution found
     */
    void runSingleThreadIterationsPathRelinking(const std::array<float, 10> &alpha_probabilities,
                                                std::array<float, 10> &alpha_cumulative_scores,
                                                std::array<float, 10> &alpha_selection_count,
                                                const int nb_iterations,
                                                std::vector<Solution> &all_local_best_solutions,
                                                int thread_id,
                                                const Params &params,
                                                const Instance &instance);


}