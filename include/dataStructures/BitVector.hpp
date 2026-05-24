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
 * @file BitVector.hpp
 * @class BitVector
 * @brief defines  a compact BitVector for the Set Packing Problem Solver
 * @author Josué Tambwe
 * @date 25 May 2026
 */

 #pragma once 

 #include <bit>
 #include <cstdint>
 #include <vector>
 #include <iostream>

 namespace spp{

    class BitVector{

        private : 

            size_t nb_logical_bits;
            std::vector<std::uint64_t> data;

        public : 

            // constructor 
            BitVector(size_t n);

            // destructor 
            ~BitVector() = default; 

            void print() const; 

            // getters 
            size_t getNbLogicalBits() const;
            size_t getNbWords() const;
            size_t getNbNonZeroBits() const;

            // setters 
            void activate(int index);
            void deactivate(int index);
    };

 }