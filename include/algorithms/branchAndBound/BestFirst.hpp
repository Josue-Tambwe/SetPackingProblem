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
 * @file BestFirst.hpp
 * @class BestFirst
 * @brief defines the data structure of the best-first node selection strategy in the Branch and Bound algorithm
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#pragma once

#include "dataStructures/BaBNode.hpp"
#include <queue>
#include <limits>
#include <iostream>

namespace spp{

    class BestFirst{

        private : 

            std::priority_queue<spp::BaBNode> node_list; // list of open nodes

        public :

            // constructor
            BestFirst() = default;


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