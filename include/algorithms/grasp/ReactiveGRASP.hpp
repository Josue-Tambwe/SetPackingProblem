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
 * @brief defines the Reactive Greedy Randomized Adaptive Search Procedure (GRASP) method for the Set Packing Solver
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
#include "algorithms/grasp/GRASPUtils.hpp"
#include "hpc/thread/Thread.hpp"
#include <array>
#include <vector> 
#include <algorithm>
#include <cstdint>


namespace spp{


    /**
     * @brief performs iterations (construction + local search) for a single CPU thread and saves the best solution found
     */
    void runSingleThreadIterations(const std::array<float, 10> &alpha_probabilities,
                                    std::array<float, 10> &alpha_cumulative_scores,
                                    std::array<float, 10> &alpha_selection_count,
                                    const int nb_iterations,
                                    std::vector<Solution> &all_best_solutions,
                                    int thread_id,
                                    const Params &params,
                                    const Instance &instance);






    /**
     * @brief performs iterations (construction + local search) for multiple CPU thread and updates alpha probabilities
     */
    Solution runOneReactiveGRASPCycle(std::array<float, 10> &alpha_probabilities,
                                      Solution &best_solution,
                                      const Params &params,
                                      const Instance &instance);


}