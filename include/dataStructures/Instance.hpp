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
 * @file Instance.hpp
 * @brief defines  data structures that store an instance of the Set Packing Problem
 * @author Josué Tambwe
 * @date 22 May 2026
 */

 #pragma once 


 #include "dataStructures/Parameters.hpp"
 #include "output/Logger.hpp"
 #include <vector>
 #include <iostream>
 #include <fstream>
 #include <vector>
 #include <string>

 namespace spp{


    /**
     * @brief defines some statistics of an instance
     */
    struct InstanceStatistics{

        size_t nb_vars;
        size_t nb_constraints;
        size_t total_nonzeros; // in the constraint matrix
        size_t max_nonzeros_per_row; // in the constraint matrix
    };



    /**
     * @brief defines an instance of the Set Packing Problem
     */
    class Instance{

        private : 

            size_t nb_vars;
            size_t nb_constraints;

            std::vector<int> profit; // profit coefficients
            std::vector<std::vector<int>> constraint_matrix; // sparse constraint matrix

            InstanceStatistics stats;

        public : 

            // constructor
            Instance(Params &params);

            // destructor
            ~Instance() = default; 


            // getters 
            size_t getNbVars() const;
            size_t getNbConstraints() const;

            const std::vector<int>& getProfitVector() const;
            const std::vector<std::vector<int>>& getConstraintMatrix() const;
            const InstanceStatistics& getStatistics() const;


            void print() const;
        
    };


 }