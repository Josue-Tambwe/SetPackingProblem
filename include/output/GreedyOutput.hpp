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
 * @file GreedyOutput.hpp 
 * @author Josué Tambwe
 * @date 6 June 2026
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


 namespace spp{


    void printHeaderGreedy(const Params &params, 
                           const Instance &instance);


    std::string computeVariationString(double new_value, double old_value);


    void printStepsGreedy(double &one_two_exchange_time, 
                          double &one_one_exchange_time, 
                          double &two_one_exchange_time, 
                          double &zero_one_exchange_time,
                          std::int64_t &construction_objective,
                          std::int64_t &one_two_exchange_objective,
                          std::int64_t &one_one_exchange_objective,
                          std::int64_t &two_one_exchange_objective,
                          std::int64_t &zero_one_exchange_objective,
                          size_t &one_two_exchange_iterations,
                          size_t &one_one_exchange_iterations,
                          size_t &two_one_exchange_iterations,
                          size_t &zero_one_exchange_iterations,
                          const Params &params);



    void printSummaryGreedy(double &construction_time, 
                            double total_time,
                            std::int64_t &construction_objective,
                            std::int64_t local_search_objective,
                            Status status);

   
    
 }