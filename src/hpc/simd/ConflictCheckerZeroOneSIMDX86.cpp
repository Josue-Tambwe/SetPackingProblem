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
 * @file ConflictCheckerZeroOneSIMDX86.cpp
 * @author Josué Tambwe
 * @date 1 June 2026
 */

 #if HAS_X86

 #include "hpc/simd/ConflictCheckerZeroOneSIMDX86.hpp"

 namespace spp{


    #if HAS_AVX2

    bool checkConflictZeroOneMoveAVX2_X4(size_t &cursor,
                                        const size_t &nb_words,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources){

            while((cursor + 15) < nb_words){

                // loading 4  256-bit 

                // concumed resources
                __m256i occupied_after_removal_1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + cursor));
                __m256i occupied_after_removal_2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 4)));
                __m256i occupied_after_removal_3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 8)));
                __m256i occupied_after_removal_4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 12)));
                
                // to activate
                __m256i to_activate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + cursor));
                __m256i to_activate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + (cursor + 4)));
                __m256i to_activate_3 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + (cursor + 8)));
                __m256i to_activate_4 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + (cursor + 12)));

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



    bool checkConflictZeroOneMoveAVX2_X2(size_t &cursor,
                                       const std::uint64_t* to_activate,
                                       std::uint64_t* consumed_resources){

        // loading 2  256-bit 

        // concumed resources
        __m256i occupied_after_removal_1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + cursor));
        __m256i occupied_after_removal_2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 4)));

        // to activate
        __m256i to_activate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + cursor));
        __m256i to_activate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + (cursor + 4)));
        
        // checking conflict with the potential activation 

        __m256i conflict_1 = _mm256_and_si256(occupied_after_removal_1, to_activate_1);
        __m256i conflict_2 = _mm256_and_si256(occupied_after_removal_2, to_activate_2);

        // synchronization 

        __m256i all_conflict = _mm256_or_si256(conflict_1, conflict_2);

        cursor += 8;

        // _mm256_testz_si256 returns (all_conflict & all_conflict) == 0

        return (_mm256_testz_si256(all_conflict, all_conflict));             

    }



    bool checkConflictZeroOneMoveAVX2_X1(size_t &cursor,
                                       const std::uint64_t* to_activate,
                                       std::uint64_t* consumed_resources){

        // loading  256-bit 

        // concumed resources
        __m256i occupied_after_removal = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + cursor));
        
        // to activate
        __m256i to_activate_register = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_activate + cursor));

        // checking conflict with the potential activation 
        __m256i conflict = _mm256_and_si256(occupied_after_removal, to_activate_register);
        
        cursor += 4;

        // _mm256_testz_si256 returns (conflict & conflict) == 0

        return (_mm256_testz_si256(conflict, conflict));             
    }



    bool checkConflictZeroOneTail(size_t &cursor,
                                const size_t &nb_words,
                                const std::uint64_t* to_activate,
                                std::uint64_t* consumed_resources){

        std::uint64_t conflict = 0ULL;

        for(size_t index = cursor; index < nb_words; index++){

            // loading
            std::uint64_t occupied_after_removal = consumed_resources[index];

            // checking conflict witn the potential  activation
            conflict |= occupied_after_removal & to_activate[index];
        }

        return (conflict == 0ULL);
                    
    }



    bool checkConflictZeroOneMoveAVX2(const size_t &nb_words,
                                    const std::uint64_t* to_activate,
                                    std::uint64_t* consumed_resources){

        size_t cursor = 0;

        if(!checkConflictZeroOneMoveAVX2_X4(cursor, 
                                           nb_words,
                                           to_activate, 
                                           consumed_resources)){

            return true;

        }

        if((cursor + 7) < nb_words){

            if(!checkConflictZeroOneMoveAVX2_X2(cursor,
                                               to_activate, 
                                               consumed_resources)){

            return true;

            }

        }


        if((cursor + 3) < nb_words){

            if(!checkConflictZeroOneMoveAVX2_X1(cursor,
                                               to_activate, 
                                               consumed_resources)){

            return true;

            }

        }

        if(cursor < nb_words){

            if(!checkConflictZeroOneTail(cursor,
                                        nb_words,
                                        to_activate, 
                                        consumed_resources)){

            return true;

            }

        }

        return false;

    }

    #endif


 }


 #endif



 