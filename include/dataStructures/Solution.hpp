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
 #include "dataStructures/Instance.hpp"
 #include <iostream>
 #include <vector>
 #include <cstdint>

 namespace spp{

    class Solution{

        private : 

            BitVector solution_vector;
            BitVector consumed_resources;
            std::int64_t objective_value;
            Status status;

            void computeObjectiveValue(const Instance &instance);
            


        public : 

            // constructor 
            Solution(const Instance &instance);

            // destructor 
            ~Solution() = default; 

            // getters 
            size_t getNbVars() const;
            size_t getNbNonZeroVars() const;
            size_t getNbVarWords() const;

            std::vector<int> getNonZeroVarsIndexes() const;
            std::vector<int> getZeroVarsIndexes() const;

            size_t getNbConstraints() const;
            size_t getNbConsumedResources() const;
            size_t getNbConsumedResourceWords() const;

            std::vector<int> getNonZeroConsumedResourcesIndexes() const;
            std::vector<int> getZeroConsumedResourcesIndexes() const;
            std::vector<std::uint64_t>& getConsumedResourcesData();
            std::uint64_t* getConsumedResourcesPointerToData();

            std::int64_t getObjectiveValue(const Instance &instance);
            Status getStatus() const;

            bool isFeasible(const Instance &instance);


            // setters
            void activateVar(int var_index, const Instance &instance);
            void deactivateVar(int var_index, const Instance &instance);
            void setStatus(Status status);

            void printVars() const;
            void printConsumedResources() const;
            void print(const Instance &instance);
    };
 }