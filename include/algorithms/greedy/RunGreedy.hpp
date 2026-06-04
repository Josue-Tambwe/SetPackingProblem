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
 * @file RunGreedy.hpp
 * @brief runs the deterministic greedy construction and the variable neighborhood descent (local search)
 * @author Josué Tambwe
 * @date 4 June 2026
 */

 #pragma once

 #include "dataStructures/Parameters.hpp"
 #include "dataStructures/Instance.hpp"
 #include "dataStructures/Solution.hpp"
 #include "dataStructures/Status.hpp"
 #include "dataStructures/Timer.hpp"
 #include "output/Logger.hpp"
 #include "algorithms/greedy/DeterministicConstruction.hpp"
 #include "algorithms/localSearch/NeighborhoodUtils.hpp"
 #include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
 #include <string>
 #include <cstdint>
 #include <iostream>

 namespace spp{


    /**
     * @brief performs the Variable neighborhood Descent (VND) without SIMD extension
     */
    void localSearchScalar(double &one_two_exchange_time, 
                           double &one_one_exchange_time, 
                           double &two_one_exchange_time, 
                           double &zero_one_exchange_time,
                           std::int64_t &one_two_exchange_objective,
                           std::int64_t &one_one_exchange_objective,
                           std::int64_t &two_one_exchange_objective,
                           std::int64_t &zero_one_exchange_objective,
                           Solution &solution,
                           const Params &params,
                           const Instance &instance);



    /**
     * @brief performs the Variable neighborhood Descent (VND) with SIMD instrinsic (AVX2 on x86 ISA)
     */

    #if HAS_x86 && HAS_AVX2

    void localSearchSIMDX86(double &one_two_exchange_time, 
                           double &one_one_exchange_time, 
                           double &two_one_exchange_time, 
                           double &zero_one_exchange_time,
                           std::int64_t &one_two_exchange_objective,
                           std::int64_t &one_one_exchange_objective,
                           std::int64_t &two_one_exchange_objective,
                           std::int64_t &zero_one_exchange_objective,
                           Solution &solution,
                           const Params &params,
                           const Instance &instance);
    #endif

    /**
     * @brief performs the Variable neighborhood Descent (VND) with SIMD instrinsic (NEON on ARM ISA)
     */

    #if HAS_ARM && HAS_NEON

    void localSearchSIMDARM(double &one_two_exchange_time, 
                           double &one_one_exchange_time, 
                           double &two_one_exchange_time, 
                           double &zero_one_exchange_time,
                           std::int64_t &one_two_exchange_objective,
                           std::int64_t &one_one_exchange_objective,
                           std::int64_t &two_one_exchange_objective,
                           std::int64_t &zero_one_exchange_objective,
                           Solution &solution,
                           const Params &params,
                           const Instance &instance);
    #endif


    /**
     * @brief runs the greedy algorithm : deterministic construction + variable neighborhood search (VND)
     *        construction : based on the heuristic score = profit / nb resources required
              local search : VND = 1-2 exchange -> 1-1 exchange -> 2-1 exchange -> 0-1 exchange
                             VND intensification = 1-1 exchange -> 1-2 exchange -> 2-1 exchange -> 0-1 exchange
     */
    void runGreedy(const Params &params);

 }