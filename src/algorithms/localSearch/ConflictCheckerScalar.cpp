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
 * @file ConflictCheckerScalar.cpp
 * @author Josué Tambwe
 * @date 28 May 2026
 */

 #include "algorithms/localSearch/ConflictCheckerScalar.hpp"


 namespace spp{


    bool checkConflictTwoOneMove_X4(size_t &cursor,
                                    std::vector<std::uint64_t> &first_to_deactivate,
                                    std::vector<std::uint64_t> &second_to_deactivate,
                                    std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){

        while(cursor < first_to_deactivate.size() - 3){

            // loading 4 64-bits words
            std::uint64_t word_1 = consumed_resources[cursor];
            std::uint64_t word_2 = consumed_resources[(cursor + 1)];
            std::uint64_t word_3 = consumed_resources[(cursor + 2)];
            std::uint64_t word_4 = consumed_resources[(cursor + 3)];

            // first deactivation
            word_1 &= ~first_to_deactivate[cursor];
            word_2 &= ~first_to_deactivate[(cursor + 1)];
            word_3 &= ~first_to_deactivate[(cursor + 2)];
            word_4 &= ~first_to_deactivate[(cursor + 3)];

            // second deactivation
            word_1 &= ~second_to_deactivate[cursor];
            word_2 &= ~second_to_deactivate[(cursor + 1)];
            word_3 &= ~second_to_deactivate[(cursor + 2)];
            word_4 &= ~second_to_deactivate[(cursor + 3)];

            // activation
            word_1 |= to_activate[cursor];
            word_2 |= to_activate[(cursor + 1)];
            word_3 |= to_activate[(cursor + 2)];
            word_4 |= to_activate[(cursor + 3)];

            // synchronization
            if((word_1 | word_2 | word_3 | word_4) != 0ULL){return false;}

            cursor += 4;

        }

        return true;
    }



    bool checkConflictTwoOneMove_X2(size_t &cursor,
                                    std::vector<std::uint64_t> &first_to_deactivate,
                                    std::vector<std::uint64_t> &second_to_deactivate,
                                    std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){

        // loading 3 64-bits words
        std::uint64_t word_1 = consumed_resources[cursor];
        std::uint64_t word_2 = consumed_resources[(cursor + 1)];

        // first deactivation
        word_1 &= ~first_to_deactivate[cursor];
        word_2 &= ~first_to_deactivate[(cursor + 1)];

        // second deactivation
        word_1 &= ~second_to_deactivate[cursor];
        word_2 &= ~second_to_deactivate[(cursor + 1)];

        // activation
        word_1 |= to_activate[cursor];
        word_2 |= to_activate[(cursor + 1)];

        cursor += 2;

        return ((word_1 | word_2 ) != 0ULL);
        

    }



    bool checkConflictTwoOneMove_X1(size_t &cursor,
                                    std::vector<std::uint64_t> &first_to_deactivate,
                                    std::vector<std::uint64_t> &second_to_deactivate,
                                    std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources){
        // loading
        std::uint64_t word = consumed_resources[cursor];

        // first deactivation
        word &= ~first_to_deactivate[cursor];

        // second deactivation
        word &= ~second_to_deactivate[cursor];

        // activation
        word |= to_activate[cursor];
        
        cursor += 1;

        return (word != 0ULL);

    }



    bool checkConflictTwoOneMove(std::vector<std::uint64_t> &first_to_deactivate,
                                 std::vector<std::uint64_t> &second_to_deactivate,
                                 std::vector<std::uint64_t> &to_activate,
                                 std::vector<std::uint64_t> &consumed_resources){

        size_t cursor = 0;

        if(!checkConflictTwoOneMove_X4(cursor, 
                                      first_to_deactivate, 
                                      second_to_deactivate, 
                                      to_activate, consumed_resources)){

            return false;
        }

        if(cursor < first_to_deactivate.size() - 2 ){

            if(!checkConflictTwoOneMove_X2(cursor, 
                                          first_to_deactivate, 
                                          second_to_deactivate, 
                                          to_activate, consumed_resources)){

                return false;
            }

        }

        if(cursor < first_to_deactivate.size()){

            if(!checkConflictTwoOneMove_X1(cursor, 
                                          first_to_deactivate, 
                                          second_to_deactivate, 
                                          to_activate, consumed_resources)){

                return false;
            }
            
        }

        return true;
    }



 }