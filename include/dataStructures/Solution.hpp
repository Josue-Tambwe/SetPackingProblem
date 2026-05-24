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
 * @file Solution.hpp
 * @class Solution
 * @brief defines a the data structure which stores a solution relating to an instance of the Set Packing Problem
 * @author Josué Tambwe
 * @date 25 May 2026
 */

 #pragma once 

 #include "dataStructures/BitVector.hpp"
 #include "dataStructures/Status.hpp"
 #include <iostream>

 namespace spp{

    class Solution{

        private : 

            BitVector solution_vector;
            BitVector consumed_resources;
            Status status;


        public : 

            // constructor 
            Solution(size_t nb_vars, size_t nb_constraints);

            // destructor 
            ~Solution() = default; 

            // getters 
            size_t getNbVars() const;
            size_t getNbNonZeroVars() const;
            size_t getNbVarWords() const;

            size_t getNbConstraints() const;
            size_t getNbConsumedResources() const;
            size_t getNbConsumedResourceWords() const;

            Status getStatus() const;

            // setters
            void activateVar(int index);
            void deactivateVar(int index);

            void consumeResource(int index);
            void freeResource(int index);


            void printVars() const;
            void printConsumedResources() const;
            void print() const;
    };
 }