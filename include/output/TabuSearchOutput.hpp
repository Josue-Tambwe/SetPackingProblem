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
 * @file TabuSearchOutput.hpp 
 * @author Josué Tambwe
 * @date 30 July 2026
 */

#pragma once

#include "configuration/OutputConfig.hpp"
#include "dataStructures/Parameters.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Status.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <algorithm>
#include <cmath>


namespace spp{


    void printHeaderTabuSearch(const Params &params, 
                               const Instance &instance);




    std::string formatImprovementTabuSearch(double value);




    std::string formatDegradationTabuSearch(double value);




    void printTabuSearchIterations(double current_time, 
                                   size_t current_iteration,
                                   size_t variation_iteration_count,
                                   std::int64_t current_solution_objective_value,
                                   std::int64_t best_solution_objective_value,
                                   double relative_cumulative_improvement,
                                   double relative_cumulative_degradation);





    void printSummaryTabuSearch(double construction_time, 
                                double total_time,
                                size_t iterations,
                                std::int64_t construction_objective,
                                std::int64_t local_search_objective,
                                Status status);


}