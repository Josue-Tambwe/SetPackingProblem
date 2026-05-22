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
 * @file Status.hpp
 * @brief defines the status of a solution for the Set Packing Problem
 * @author Josué Tambwe
 * @date 23 May 2026
 */

 #pragma once

 #include "configuration/OutputConfig.hpp"
 #include <iostream>

 namespace spp{

    enum Status{

        FEASIBLE,
        INFEASIBLE,
        OPTIMAL,
        UNKNOWN,
    };

    std::ostream& operator<<(std::ostream &output_stream, const Status &status);
 }
