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
 * @file ConflictCheckerTwoOneSIMDX86.cpp
 * @author Josué Tambwe
 * @date 31 May 2026
 */

 #if HAS_X86 && HAS_AVX2

 #include "hpc/simd/ConflictCheckerTwoOneSIMDX86.hpp"

 namespace spp{

    bool checkConflictTwoOneMoveAVX2_X4(size_t &cursor,
                                        const size_t &nb_words,
                                        const std::uint64_t* first_to_deactivate,
                                        const std::uint64_t* second_to_deactivate,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources){

            while((cursor + 15) < nb_words){

                // loading 4  256-bit 

                // concumed resources
                __m256i occupied_after_removal_1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + cursor));
                __m256i occupied_after_removal_2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 4)));
                __m256i occupied_after_removal_3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 8)));
                __m256i occupied_after_removal_4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 12)));

                // first to deactivate
                __m256i first_to_deactivate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_deactivate + cursor));
                __m256i first_to_deactivate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_deactivate + (cursor + 4)));
                __m256i first_to_deactivate_3 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_deactivate + (cursor + 8)));
                __m256i first_to_deactivate_4 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_deactivate + (cursor + 12)));

                // second to deactivate
                __m256i second_to_deactivate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_deactivate + cursor));
                __m256i second_to_deactivate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_deactivate + (cursor + 4)));
                __m256i second_to_deactivate_3 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_deactivate + (cursor + 8)));
                __m256i second_to_deactivate_4 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_deactivate + (cursor + 12)));


                // to activate
                __m256i to_activate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + cursor));
                __m256i to_activate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + (cursor + 4)));
                __m256i to_activate_3 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + (cursor + 8)));
                __m256i to_activate_4 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + (cursor + 12)));


                // deactivation :  occupied_after_removal <- not (fist_to_deactivate or second_to_deactivate) and consumed_resources

                occupied_after_removal_1 = _mm256_andnot_si256(_mm256_or_si256(first_to_deactivate_1, second_to_deactivate_1), 
                                                              occupied_after_removal_1);

                occupied_after_removal_2 = _mm256_andnot_si256(_mm256_or_si256(first_to_deactivate_2, second_to_deactivate_2), 
                                                              occupied_after_removal_2);

                occupied_after_removal_3 = _mm256_andnot_si256(_mm256_or_si256(first_to_deactivate_3, second_to_deactivate_3), 
                                                              occupied_after_removal_3);

                occupied_after_removal_4 = _mm256_andnot_si256(_mm256_or_si256(first_to_deactivate_4, second_to_deactivate_4), 
                                                              occupied_after_removal_4);


                // checking conflict with the potential activation 

                __m256i conflict_1 = _mm256_and_si256(occupied_after_removal_1, to_activate_1);
                __m256i conflict_2 = _mm256_and_si256(occupied_after_removal_2, to_activate_2);
                __m256i conflict_3 = _mm256_and_si256(occupied_after_removal_3, to_activate_3);
                __m256i conflict_4 = _mm256_and_si256(occupied_after_removal_4, to_activate_4);

                // synchronization 
                __m256i all_conflict = _mm256_or_si256(_mm256_or_si256(conflict_1, conflict_2), 
                                                       _mm256_or_si256(conflict_3, conflict_4));

                // _mm256_testz_si256 returns (all_conflict & all_conflict) == 0
                if(!_mm256_testz_si256(all_conflict, all_conflict)){return false;}


                cursor += 16;
            }

        return true;
    }



    bool checkConflictTwoOneMoveAVX2_X2(size_t &cursor,
                                       const std::uint64_t* first_to_deactivate,
                                       const std::uint64_t* second_to_deactivate,
                                       const std::uint64_t* to_activate,
                                       std::uint64_t* consumed_resources){

        // loading 2  256-bit 

        // concumed resources
        __m256i occupied_after_removal_1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + cursor));
        __m256i occupied_after_removal_2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 4)));

        // first to deactivate
        __m256i first_to_deactivate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_deactivate + cursor));
        __m256i first_to_deactivate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_deactivate + (cursor + 4)));
                
        // second to deactivate
        __m256i second_to_deactivate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_deactivate + cursor));
        __m256i second_to_deactivate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_deactivate + (cursor + 4)));


        // to activate
        __m256i to_activate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + cursor));
        __m256i to_activate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + (cursor + 4)));
                

        // deactivation :  occupied_after_removal <- not (fist_to_deactivate or second_to_deactivate) and consumed_resources

        occupied_after_removal_1 = _mm256_andnot_si256(_mm256_or_si256(first_to_deactivate_1, second_to_deactivate_1), 
                                                        occupied_after_removal_1);

        occupied_after_removal_2 = _mm256_andnot_si256(_mm256_or_si256(first_to_deactivate_2, second_to_deactivate_2), 
                                                        occupied_after_removal_2);

        
        // checking conflict with the potential activation 

        __m256i conflict_1 = _mm256_and_si256(occupied_after_removal_1, to_activate_1);
        __m256i conflict_2 = _mm256_and_si256(occupied_after_removal_2, to_activate_2);

        // synchronization 
        __m256i all_conflict = _mm256_or_si256(conflict_1, conflict_2);

        cursor += 8;

        // _mm256_testz_si256 returns (all_conflict & all_conflict) == 0
        return (_mm256_testz_si256(all_conflict, all_conflict));             

    }



    bool checkConflictTwoOneMoveAVX2_X1(size_t &cursor,
                                       const std::uint64_t* first_to_deactivate,
                                       const std::uint64_t* second_to_deactivate,
                                       const std::uint64_t* to_activate,
                                       std::uint64_t* consumed_resources){

        // loading  256-bit 

        // concumed resources
        __m256i occupied_after_removal = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + cursor));
        
        // first to deactivate
        __m256i first_to_deactivate_register = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_deactivate + cursor));
        
        // second to deactivate
        __m256i second_to_deactivate_register = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_deactivate + cursor));
        
        // to activate
        __m256i to_activate_register = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + cursor));

       
        // deactivation :  occupied_after_removal <- not (fist_to_deactivate or second_to_deactivate) and consumed_resources

        occupied_after_removal = _mm256_andnot_si256(_mm256_or_si256(first_to_deactivate_register, second_to_deactivate_register), 
                                                    occupied_after_removal);


        // checking conflict with the potential activation 
        __m256i conflict = _mm256_and_si256(occupied_after_removal, to_activate_register);
        
        
        cursor += 4;

        // _mm256_testz_si256 returns (conflict & conflict) == 0
        return (_mm256_testz_si256(conflict, conflict));             
    }



    bool checkConflictTwoOneTail(size_t &cursor,
                                const size_t &nb_words,
                                const std::uint64_t* first_to_deactivate,
                                const std::uint64_t* second_to_deactivate,
                                const std::uint64_t* to_activate,
                                std::uint64_t* consumed_resources){

        std::uint64_t conflict = 0ULL;

        for(size_t index = cursor; index < nb_words; index++){

            // loading
            std::uint64_t occupied_after_removal = consumed_resources[index];

            // first deactivation
            occupied_after_removal &= ~first_to_deactivate[index];

            // second deactivation
            occupied_after_removal &= ~second_to_deactivate[index];

            // checking conflict witn the potential  activation
            conflict |= occupied_after_removal & to_activate[index];
        }

        return (conflict == 0ULL);
                    
    }



    bool checkConflictTwoOneMoveAVX2(const size_t &nb_words,
                                    const std::uint64_t* first_to_deactivate,
                                    const std::uint64_t* second_to_deactivate,
                                    const std::uint64_t* to_activate,
                                    std::uint64_t* consumed_resources){

        size_t cursor = 0;

        if(!checkConflictTwoOneMoveAVX2_X4(cursor, 
                                           nb_words, 
                                           first_to_deactivate, 
                                           second_to_deactivate, 
                                           to_activate, 
                                           consumed_resources)){

            return true;

        }

        if((cursor + 7) < nb_words){

            if(!checkConflictTwoOneMoveAVX2_X2(cursor, 
                                               first_to_deactivate, 
                                               second_to_deactivate, 
                                               to_activate, 
                                               consumed_resources)){

            return true;

            }

        }


        if((cursor + 3) < nb_words){

            if(!checkConflictTwoOneMoveAVX2_X1(cursor, 
                                               first_to_deactivate, 
                                               second_to_deactivate, 
                                               to_activate, 
                                               consumed_resources)){

            return true;

            }

        }

        if(cursor < nb_words){

            if(!checkConflictTwoOneTail(cursor,
                                        nb_words,  
                                        first_to_deactivate, 
                                        second_to_deactivate, 
                                        to_activate, 
                                        consumed_resources)){

            return true;

            }

        }

        return false;

    }

 }


 #endif



 