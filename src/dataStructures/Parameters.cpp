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
 * @file Parameters.cpp
 * @author Josué Tambwe
 * @date 22 May 2026
 */

 #include "dataStructures/Parameters.hpp"

 namespace spp{

    Params::Params(){

        // general parameters
        algorithm = Algorithm::Unknown;
        instance_path = "";
        verbose = false;
        use_simd = false;
        use_intensification = false;
        nb_threads = NUMBER_PHYSICAL_CORES;


        use_time_limit = false;
        use_max_iterations = false;
        nb_max_iterations = 5;
        time_limit = 10.0;

        // Branch and Bound & Milp solver
        milp_solver='h';
        exploration_strategy='b';
        warm_start = false;
        branching_value = 0.0;
        optimality_gap = 0.0;


        // GRASP
        biais = 0.0;
        update_interval = 10;
    }
 }