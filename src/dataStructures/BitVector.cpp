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
    }


    void BitVector::printNonZeroIndexes() const {

        std::cout << " nonzeros indexes  = [";
        std::vector<int> nonzeros_indexes = this->getNonZeroBitIndexes();
        for(int index : nonzeros_indexes){std::cout << (index + 1) << " ";}
        std::cout << "] \n\n";
    }


    void BitVector::printZeroIndexes() const {

        std::cout << " zeros indexes = [";
        std::vector<int> zeros_indexes = this->getZeroBitIndexes();
        for(int index : zeros_indexes){std::cout << (index + 1) << " ";}
        std::cout << "] \n\n";
    }



    // getters

    std::vector<std::uint64_t>& BitVector::getData() {return this->data;}
    const std::vector<std::uint64_t>& BitVector::getData() const {return this->data;}

    std::uint64_t*  BitVector::getPointerToData() {return this->data.data();}
    const std::uint64_t*  BitVector::getPointerToData() const {return this->data.data();}

    size_t BitVector::getNbLogicalBits() const {return nb_logical_bits;}
    size_t BitVector::getNbWords() const {return data.size();}

    size_t BitVector::getNbNonZeroBits() const {

        size_t counter = 0;

        for(std::uint64_t word : data){

            counter += std::popcount(word);
        }
        return counter;
    }



    bool BitVector::isNonZero(int index) const {

        int word_index = index / 64;
        int bit_index = index % 64;

        return ((data[word_index] & (1ULL << bit_index)) != 0);
    }



    std::vector<int> BitVector::getNonZeroBitIndexes() const{

        size_t nonzeros_count = this->getNbNonZeroBits();
        std::vector<int> nonzeros_indexes(nonzeros_count);
        int cursor = 0;

        for(int word_index = 0; word_index < static_cast<int>(data.size()); word_index++){

            std::uint64_t word = data[word_index];

            while(word != 0){

                int bit_index = std::countr_zero(word);
                nonzeros_indexes[cursor] = (word_index * 64) + bit_index;
                cursor += 1;

                word &= (word - 1ULL);
            }

        }
        return nonzeros_indexes;

        /* This method extract in each word every nonzero bit from the LSB to the MSB
         * Example with word = 00101000
         * bit 7 : 0
         * bit 6 : 0
         * bit 5 : 1
         * bit 4 : 0
         * bit 3 : 1
         * bit 2 : 0
         * bit 1 : 0
         * bit 0 : 0
         * 
         * bit index = 3
         * 
         * word     = 00101000
         * word - 1 = 00100111
         *    '&'     --------
         *            00100000
         * 
         * The next value of bit_index in that 'while loop' is 5
         * Thefore, the algorithm worst case complexity is O(k)
         * with k : the number of nonzero bit within a word
        */
    }



    std::vector<int> BitVector::getZeroBitIndexes() const{

        size_t zeros_count = this->nb_logical_bits - this->getNbNonZeroBits();
        std::vector<int> zeros_indexes(zeros_count);
        int cursor = 0;

        for(int word_index = 0; word_index < static_cast<int>(data.size()); word_index++){

            // inverse (flip) all bits within the word in other to deal with bits equal to 1
            std::uint64_t word = ~data[word_index];

            // extra bits (out of the bound of nb_logical_bits) are set to 0
            if(word_index == static_cast<int>(data.size() - 1)){

                size_t extra_bits = (data.size() * 64) - nb_logical_bits;

                if (extra_bits > 0) {
                    std::uint64_t mask = (~0ULL) >> extra_bits;
                    word &= mask;
                }
            }

            /* Extra bits management (LSB -> MSB):

                    Suppose the BitVector has 100 logical bits.
                    Internally, this means 2 words of 64 bits = 128 stored bits.
                    So the last 28 bits (index 100..127) are extra bits and must be ignored.

                    We look at the last 64-bit word (bits 64..127), written LSB -> MSB:

                        Logical bits (64..99)          Extra bits (100..127)
                        [ b64 b65 ... b99 ]            [ 0 0 ... 0 ]

                    After flipping the word (~word), still LSB -> MSB:

                        [ ~b64 ~b65 ... ~b99 ]         [ 1 1 ... 1 ]

                    These extra bits (100..127) must NOT be treated as zeros.
                    So we build a mask that keeps only the logical bits:

                        extra_bits = 128 - 100 = 28
                        mask = (~0ULL) >> extra_bits

                    In LSB -> MSB order, the mask looks like:

                        mask = [ 36 ones ] [ 28 zeros ]

                    Applying the mask:

                        flipped_word &= mask

                    This clears all extra bits (forcing them to 0) and keeps only the inverted
                    logical bits. The algorithm can then scan the zero bits safely.

            */


            // This part follows the same logic as the previous method 'getNonZeroBitIndexes()'
            while(word != 0){

                int bit_index = std::countr_zero(word);
                zeros_indexes[cursor] = (word_index * 64) + bit_index;
                cursor += 1;

                word &= (word - 1ULL);
            }


        }

        return zeros_indexes;

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
         * then the '~' inverses all bits within the mask : 00100 -> 11011
         *
         * '&' operator :  10101
         *               & 11011
         *                 -----
         *                 10001
        */
    }

 }