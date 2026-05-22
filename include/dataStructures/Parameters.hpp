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
        int nb_threads;

        
        bool use_time_limit;
        bool use_max_iterations;
        size_t nb_max_iterations;
        double time_limit;


    };
    
 }