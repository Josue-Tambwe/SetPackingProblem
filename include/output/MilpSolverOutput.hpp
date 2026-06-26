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
 * @file MilpSolverOutput.hpp 
 * @author Josué Tambwe
 * @date 27 June 2026
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

    void printHeaderMilp(const Params &params,
                         const Instance &instance);


  
    void finalStatisticsMilp(double preprocessing_time,
                             double total_time,
                             std::int64_t obj_value,
                             Status status);

}