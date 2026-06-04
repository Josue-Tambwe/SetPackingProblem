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
 * @file ConflictCheckerOneOneSIMDARM.cpp
 * @author Josué Tambwe
 * @date 3 June 2026
 */

#if HAS_ARM && HAS_NEON

#include "hpc/simd/ConflictCheckerOneOneSIMDARM.hpp"

namespace spp{

    bool checkConflictOneOneMoveNEON_X4(size_t &cursor,
                                        const size_t &nb_words,
                                        const std::uint64_t* to_deactivate,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources){

        while((cursor + 7) < nb_words){

            // loading 4  128-bit  = 8 x 64-bit words

            // consumed resources
            uint64x2_t occupied_after_removal_1 = vld1q_u64(consumed_resources + cursor);
            uint64x2_t occupied_after_removal_2 = vld1q_u64(consumed_resources + (cursor + 2));
            uint64x2_t occupied_after_removal_3 = vld1q_u64(consumed_resources + (cursor + 4));
            uint64x2_t occupied_after_removal_4 = vld1q_u64(consumed_resources + (cursor + 6));

            // to deactivate 
            uint64x2_t to_deactivate_1 = vld1q_u64(to_deactivate + cursor);
            uint64x2_t to_deactivate_2 = vld1q_u64(to_deactivate + (cursor + 2));
            uint64x2_t to_deactivate_3 = vld1q_u64(to_deactivate + (cursor + 4));
            uint64x2_t to_deactivate_4 = vld1q_u64(to_deactivate + (cursor + 6));

            // to activate 
            uint64x2_t to_activate_1 = vld1q_u64(to_activate + cursor);
            uint64x2_t to_activate_2 = vld1q_u64(to_activate + (cursor + 2));
            uint64x2_t to_activate_3 = vld1q_u64(to_activate + (cursor + 4));
            uint64x2_t to_activate_4 = vld1q_u64(to_activate + (cursor + 6));

            // deactivation :  occupied_after_removal <- not (to_deactivate) and consumed_resources

            occupied_after_removal_1 = vbicq_u64(occupied_after_removal_1, to_deactivate_1);
            occupied_after_removal_2 = vbicq_u64(occupied_after_removal_2, to_deactivate_2);
            occupied_after_removal_3 = vbicq_u64(occupied_after_removal_3, to_deactivate_3);
            occupied_after_removal_4 = vbicq_u64(occupied_after_removal_4, to_deactivate_4);

            // checking conflict with the potential activation

            uint64x2_t conflict_1 = vandq_u64(occupied_after_removal_1, to_activate_1);
            uint64x2_t conflict_2 = vandq_u64(occupied_after_removal_2, to_activate_2);
            uint64x2_t conflict_3 = vandq_u64(occupied_after_removal_3, to_activate_3);
            uint64x2_t conflict_4 = vandq_u64(occupied_after_removal_4, to_activate_4);

            // synchronization
            uint64x2_t all_conflict = vorrq_u64(vorrq_u64(conflict_1, conflict_2), 
                                                vorrq_u64(conflict_3, conflict_4));

            std::uint64_t conflict_word = vgetq_lane_u64(all_conflict, 0) |
                                          vgetq_lane_u64(all_conflict, 1);

            if(conflict_word != 0ULL){return false;}

            cursor += 8;
        }

        return true;

    }



    bool checkConflictOneOneMoveNEON_X2(size_t &cursor,
                                        const std::uint64_t* to_deactivate,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources){

        // loading 2  128-bit  = 4 x 64-bit words

        // consumed resources
        uint64x2_t occupied_after_removal_1 = vld1q_u64(consumed_resources + cursor);
        uint64x2_t occupied_after_removal_2 = vld1q_u64(consumed_resources + (cursor + 2));
        
        // to deactivate 
        uint64x2_t to_deactivate_1 = vld1q_u64(to_deactivate + cursor);
        uint64x2_t to_deactivate_2 = vld1q_u64(to_deactivate + (cursor + 2)); 

        // to activate 
        uint64x2_t to_activate_1 = vld1q_u64(to_activate + cursor);
        uint64x2_t to_activate_2 = vld1q_u64(to_activate + (cursor + 2));
         
        // deactivation :  occupied_after_removal <- not (to_deactivate) and consumed_resources

        occupied_after_removal_1 = vbicq_u64(occupied_after_removal_1, to_deactivate_1);
        occupied_after_removal_2 = vbicq_u64(occupied_after_removal_2, to_deactivate_2);

        // checking conflict with the potential activation

        uint64x2_t conflict_1 = vandq_u64(occupied_after_removal_1, to_activate_1);
        uint64x2_t conflict_2 = vandq_u64(occupied_after_removal_2, to_activate_2);
        
        // synchronization
        uint64x2_t all_conflict = vorrq_u64(conflict_1, conflict_2);

        std::uint64_t conflict_word = vgetq_lane_u64(all_conflict, 0) |
                                      vgetq_lane_u64(all_conflict, 1);

        cursor += 4;

        return (conflict_word == 0ULL);

    }



    bool checkConflictOneOneMoveNEON_X1(size_t &cursor,
                                        const std::uint64_t* to_deactivate,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources){

        // loading 1  128-bit  = 2 x 64-bit words

        // consumed resources
        uint64x2_t occupied_after_removal = vld1q_u64(consumed_resources + cursor);
        
        // to deactivate 
        uint64x2_t to_deactivate_register = vld1q_u64(to_deactivate + cursor);
        
        // to activate 
        uint64x2_t to_activate_register = vld1q_u64(to_activate + cursor);
         
        // deactivation :  occupied_after_removal <- not (to_deactivate) and consumed_resources
        occupied_after_removal = vbicq_u64(occupied_after_removal, to_deactivate_register);

        // checking conflict with the potential activation
        uint64x2_t conflict = vandq_u64(occupied_after_removal, to_activate_register);
        

        std::uint64_t conflict_word = vgetq_lane_u64(conflict, 0) |
                                      vgetq_lane_u64(conflict, 1);

        cursor += 2;

        return (conflict_word == 0ULL);

    }



    bool checkConflictOneOneMoveNEONTail(size_t &cursor,
                                        const std::uint64_t* to_deactivate,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources){

        // loading a 64-bit word
        std::uint64_t occupied_after_removal = consumed_resources[cursor];

        // deactivation
        occupied_after_removal &= ~to_deactivate[cursor];

        // checking conflict witn the potential  activation
        return ((occupied_after_removal & to_activate[cursor]) == 0ULL);

    }




    bool checkConflictOneOneMoveNEON(const size_t &nb_words,
                                     const std::uint64_t* to_deactivate,
                                     const std::uint64_t* to_activate,
                                     std::uint64_t* consumed_resources){

        size_t cursor = 0;

        if(!checkConflictOneOneMoveNEON_X4(cursor, 
                                           nb_words, 
                                           to_deactivate,
                                           to_activate, 
                                           consumed_resources)){

            return true;

        }

        if((cursor + 3) < nb_words){

            if(!checkConflictOneOneMoveNEON_X2(cursor, 
                                               to_deactivate,
                                               to_activate, 
                                               consumed_resources)){

            return true;

            }

        }


        if((cursor + 1) < nb_words){

            if(!checkConflictOneOneMoveNEON_X1(cursor, 
                                               to_deactivate,
                                               to_activate, 
                                               consumed_resources)){

            return true;

            }

        }

        if(cursor < nb_words){

            if(!checkConflictOneOneMoveNEONTail(cursor, 
                                                to_deactivate,
                                                to_activate, 
                                                consumed_resources)){

            return true;

            }

        }

        return false;

    }

}

#endif

