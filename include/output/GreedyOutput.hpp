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
 #include <iostream>
 #include <string>
 #include <iomanip>

 namespace spp{

    void printHeaderGreedy(const Params &params, 
                           const Instance &instance);
    
 }