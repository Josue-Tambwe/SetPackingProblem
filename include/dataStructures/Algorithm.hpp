/*------------------------------------------------------------------------------
 * SPP-Solver — A compact Set Packing solver blending MILP, heuristics,
 *              and efficient BitVector techniques.
 *------------------------------------------------------------------------------
 * Copyright (c) 2025 Josué Tambwe
 *               Nantes Université, France
 *------------------------------------------------------------------------------
 * This software is released under the MIT License.
 * See the LICENSE file for more details.
 *------------------------------------------------------------------------------
 */


/** 
 * @file Algorithm.hpp
 * @brief defines all algorithm options than can be used in  the Set Packing Solver
 * @author Josué Tambwe
 * @date 23 May 2026
 */

 #pragma once
 
 #include "configuration/OutputConfig.hpp"
 #include <iostream>

 namespace spp{

    enum  Algorithm {

        Greedy,
        Grasp,
        TabuSearch,
        SimulatedAnnealing,
        GeneticAlgorithm,
        BranchAndBound,
        Milp,
        Unknown,
    };

 }