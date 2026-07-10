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
 * @file GRASPOutput.hpp 
 * @author Josué Tambwe
 * @date 8 July 2026
 */

#pragma once

#include "configuration/OutputConfig.hpp"
#include "dataStructures/Parameters.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Status.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <array>
#include <cstdint>
#include <algorithm>


namespace spp{


    void printHeaderGRASP(const Params &params, 
                          const Instance &instance);





    void printGRASPInitialElite(double current_time, 
                                size_t current_iteration,
                                std::int64_t elite_objective_value,
                                const std::array<float, 10> &alpha_values,
                                const std::array<float, 10> &alpha_probabilities);





    void printGRASPIteration(double current_time, 
                             size_t current_iteration,
                             std::int64_t elite_objective_value,
                             std::int64_t local_best_solution_objective_value,
                             const std::array<float, 10> &alpha_values,
                             const std::array<float, 10> &alpha_probabilities);




    void printSummaryGRASP(double total_time, 
                           std::int64_t elite_objective_value,
                           size_t iterations,
                           Status status,
                           const Params &params);
}
