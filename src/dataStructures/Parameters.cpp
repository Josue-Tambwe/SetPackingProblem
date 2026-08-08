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
        nb_max_iterations = 1;
        time_limit = 10.0;

        // Branch and Bound & Milp solver
        milp_solver='h';
        exploration_strategy='b';
        warm_start = false;
        branching_value = 0.0;
        optimality_gap = 0.0;


        // GRASP
        bias = 0.0;
        update_interval = 10;
        use_path_relinking = false;
        nb_elites = -1;


        // Simulated Annealing and Tabu Search
        pruning_rate = 0.7; 
        alpha = 0.5;
        restart_interval = 50;


        // Simulated Annealing
        initial_temperature = 100.0;
        final_temperature = 1.0;
        cooling_factor = 0.9;


        // Genetic Algorithm
        improvement_time = 1.0;
        crossover_rate = 0.7;

        
        

    }
 }