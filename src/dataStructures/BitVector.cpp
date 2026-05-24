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
 * @file BitVector.cpp
 * @author Josué Tambwe
 * @date 25 May 2026
 */

 #include "dataStructures/BitVector.hpp"


 namespace spp{

    // constructor 
    BitVector::BitVector(size_t n) : nb_logical_bits(n){

        size_t nb_words = (n + 63) / 64;
        this->data.resize(nb_words, 0ULL);
    }



    void BitVector::print() const{

        for(size_t i = 0; i < nb_logical_bits; i++){

            int word_index = i / 64; // which word (64 bits)
            int bit_index = i % 64; // which index within that word

            /* shifting the bit from 'index_bit' positions to the right 
             * the '&' operator keeps only the bit we want
             * Example with 5 bits : 10101 (LSB -> MSB), we want to print the bit at index 2.
             * 
             * bit 0 : 1
             * bit 1 : 0
             * bit 2 : 1
             * bit 3 : 0
             * bit 4 : 1
             *                       
             * the bit at index 2 is shifted to the right : 10101 -> 00101 
             *  '&' operator :  00101
             *                & 00001
             *                  -----
             *                  00001
             * 
             * The 00001 is printed as 1.                 
            */
            std::cout << ((data[word_index] >> bit_index) & 1ULL) << " ";
        }
        std::cout << "\n" << std::endl;
    }


    // getters
    size_t BitVector::getNbLogicalBits() const {return nb_logical_bits;}
    size_t BitVector::getNbWords() const {return data.size();}

    size_t BitVector::getNbNonZeroBits() const {

        size_t counter = 0;

        for(std::uint64_t word : data){

            counter += std::popcount(word);
        }
        return counter;
    }



    // setters
    void BitVector::activate(int index){

        int word_index = index / 64; 
        int bit_index = index % 64;
        data[word_index] |= (1ULL << bit_index);

        /* Example with 5 bits : 10101 (LSB -> MSB), we want to active the bit at index 1
         * bit 0 : 1
         * bit 1 : 0
         * bit 2 : 1
         * bit 3 : 0
         * bit 4 : 1
         * 
         * the mask 00001 is created and the bit 1 is moved form 'bit_index' positions to the left
         * 00001 -> 00010
         * 
         * '|' operator :  10101
         *               | 00010
         *                 -----
         *                 10111
        */
    }


    void BitVector::deactivate(int index){

        int word_index = index / 64;
        int bit_index = index % 64;
        std::uint64_t mask = (1ULL << bit_index);
        data[word_index] &= ~mask;

        /* Example with 5 bits : 10101 (LSB -> MSB), we want to deactive the bit at index 2
         * bit 0 : 1
         * bit 1 : 0
         * bit 2 : 1
         * bit 3 : 0
         * bit 4 : 1
         * 
         * the mask 00001 is created and the bit 1 is moved form 'bit_index' positions to the left
         * 00001 -> 00100
         * then the '~' inverses all bit within the mask : 00100 -> 11011
         *
         * '&' operator :  10101
         *               & 11011
         *                 -----
         *                 10001
        */
    }

 }