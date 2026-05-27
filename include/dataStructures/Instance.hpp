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
 * @file Instance.hpp
 * @brief defines  data structures that store an instance of the Set Packing Problem
 * @author Josué Tambwe
 * @date 22 May 2026
 */

 #pragma once 


 #include "dataStructures/Parameters.hpp"
 #include "output/Logger.hpp"
 #include "dataStructures/BitVector.hpp"
 #include <vector>
 #include <iostream>
 #include <iomanip>
 #include <fstream>
 #include <vector>
 #include <string>
 #include <limits>
 #include <unordered_set>

 namespace spp{


    /**
     * @brief defines some statistics of an instance
     */
    struct InstanceStatistics{

        size_t nb_vars;
        size_t nb_constraints;
        size_t total_nonzeros; // in the constraint matrix
        size_t max_nonzeros_per_row; // in the constraint matrix
        float density; // percentage of nonzeros in the constraint matrix
        int max_profit;
        int min_profit;
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
            std::vector<BitVector> resource_requirements; // the transpose of constraint matrix 
            InstanceStatistics stats;

            void computeResourceRequirements();


        public : 

            // constructor
            Instance(Params &params);

            // destructor
            ~Instance() = default; 


            // getters 
            size_t getNbVars() const;
            size_t getNbConstraints() const;
            std::unordered_set<int> getAllConflictingVarsIndexes(int index) const;

            const std::vector<int>& getProfitVector() const;
            const std::vector<std::vector<int>>& getConstraintMatrix() const;
            const InstanceStatistics& getStatistics() const;
            const std::vector<BitVector>& getResourceRequirements() const;


            void print() const;
            void printResourceRequirements() const;
        
    };


 }