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
 * @file MoveNode.hpp
 * @class MoveNode
 * @brief Stores a variable id (index) and its heuristic score in order to sort all variables
 * @author Josué Tambwe
 * @date 28 May 2026
 */

 #pragma once
 
 #include <iostream>


 namespace spp{

    class MoveNode{

        private : 

            int id;
            float score;

        public : 

            // constructors 
            MoveNode() = default;
            MoveNode(int id, float score);


            // destructor 
            ~MoveNode() = default;

            // getters 
            int getId() const;
            float getScore() const;

            // operators overloading 
            bool operator<(const MoveNode &other) const;
            bool operator>(const MoveNode &other) const;

            void print() const;

    };

 }