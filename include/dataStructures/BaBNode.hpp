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
 * @file BaBNode.hpp
 * @class BaBNode
 * @brief defines the data structure of a Node for the Branch and Bound algorithm
 * @author Josué Tambwe
 * @date 25 June 2026
 */

#pragma once

#include <vector>
#include <iostream>

namespace spp{

    class BaBNode{

            private : 
               
                double dual_bound; // The value of the linear relaxation
                std::vector<int> fixed_to_one; // variable indexes fixed to  one
                std::vector<int> fixed_to_zero; // variable indexes fixed to  zero

            public : 

                // constructors
                BaBNode(double value);
                BaBNode(double value, BaBNode &other);


                // getters
                double getDualBound() const;
                std::vector<int> getIndexesFixedToOne() const;
                std::vector<int> getIndexesFixedToZero() const;

                // setters
                void fixToOne(int index);
                void fixToZero(int index);

                // operator overloading
                bool operator<(const BaBNode &other) const;

                void print() const;

    };
}