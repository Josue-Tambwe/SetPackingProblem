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
 * @file BranchAndBoundOutput.hpp 
 * @author Josué Tambwe
 * @date 26 June 2026
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


    void printHeaderBaB(const Params &params, 
                        const Instance &instance);


    void printHeaderLineBaB();



    void printBaBIteration(double time,
                           size_t processed_nodes,
                           size_t open_nodes,
                           double dual_bound,
                           double primal_bound);



    void finalStatisticsBaB(double preprocessing_time,
                            double preprocessing_obj_value,
                            double dual_bound,
                            double primal_bound,
                            double total_time,
                            Status status,
                            size_t processed_nodes,
                            size_t open_nodes,
                            size_t nodes_fathomed_by_optimality,
                            size_t nodes_fathomed_by_dominance,
                            size_t nodes_fathomed_by_infeasibility,
                            const Params &params);

}
