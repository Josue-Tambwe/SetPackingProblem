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

            size_t nb_logical_bits; // number of logical bits
            std::vector<std::uint64_t> data;  // vector of 64-bit words

        public : 

            // constructors 
            BitVector(size_t n);

            // for copy
            BitVector() = default;

            // destructor 
            ~BitVector() = default; 

            void print() const;
            void printNonZeroIndexes() const;
            void printZeroIndexes() const; 

            // getters 

            /**
             * @brief returns the vector 'data' which contains all 64-bit words
             */
            std::vector<std::uint64_t>& getData();
            const std::vector<std::uint64_t>& getData() const;



            /**
             * @brief returns a pointer to the data of the 'data' member which contains all 64-bit words
             */
            std::uint64_t*  getPointerToData();
            const std::uint64_t*  getPointerToData() const ;



            /**
             * @brief returns the number of logical bits
             */
            size_t getNbLogicalBits() const;


            /**
             * @brief returns the number of 64-bit words within the vector 'data'
             */
            size_t getNbWords() const;


            /**
             * @brief returns the number of nonzero (1) bits
             */
            size_t getNbNonZeroBits() const;


            /**
             * returns 'true' if the bit at the index 'index' is nonzero (1) or 'false' otherwise
             */
            bool isNonZero(int index) const;

            
            /**
             * @brief returns a vector that contains the indexes (from 0 to nb_logical_bits-1) of nonzero (1) bits
             */
            std::vector<int> getNonZeroBitIndexes() const;


            /**
             * @brief returns a vector that contains the indexes (from 0 to nb_logical_bits-1) of zero (0) bits
             */
            std::vector<int> getZeroBitIndexes() const;


            // setters 

            /**
             * @brief activates (turns to 1) the bit at the index 'index'
            */
            void activate(int index);
            

            /**
             * @brief deactivates (turns to 0) the bit at the index 'index' 
             */
            void deactivate(int index);
    };

 }