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
 * @file Parameters.hpp
 * @class Params
 * @brief Stores options and flags defines by the user
 * @author Josué Tambwe
 * @date 22 May 2026
 */

 #pragma once 

 #include "configuration/GeneralConfig.hpp"
 #include "dataStructures/Algorithm.hpp"
 #include <string>


 namespace spp{

    struct Params{

        // constructor
        Params();


        // destructor
        ~Params() = default;

        // general parameters
        Algorithm algorithm; 
        std::string instance_path;
        bool verbose;
        bool use_simd;
        bool use_intensification;
        int nb_threads;

        bool use_time_limit;
        bool use_max_iterations;
        size_t nb_max_iterations;
        double time_limit;

        // Branch and Bound & Milp solver
        char milp_solver;
        /* Possible value for the attribute "milp_solver"
         - 'g' : gurobi
         - 'h' : highs
         - 'x ': hexaly 
        */

        char exploration_strategy;
        /* Possible value
         - 'b' : best first search
         - 'd' : depth first search
        */

        bool warm_start;
        double branching_value;
        double optimality_gap;

        // GRASP 
        double bias;
        size_t update_interval;
        bool use_path_relinking;



    };
    
 }