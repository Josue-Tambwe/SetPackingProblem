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
 * @file DepthFirst.hpp
 * @class DepthFirst
 * @brief defines the data structure of the depth-first node selection strategy in the Branch and Bound algorithm
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#pragma once

#include "dataStructures/BaBNode.hpp"
#include <stack>
#include <limits>
#include <iostream>

namespace spp{

    class DepthFirst{

        private : 

            std::stack<spp::BaBNode> node_list; // list of open nodes

        public : 

            // constructor
            DepthFirst() = default;

            // getters

            double getLowestDualBound() const;

            spp::BaBNode pop();

            bool isEmpty() const;

            size_t getSize() const;


            
            // Setter

            void add(spp::BaBNode &node);



            void print() const;
    };
}