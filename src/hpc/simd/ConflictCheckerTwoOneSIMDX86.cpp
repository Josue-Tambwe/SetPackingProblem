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

 #if HAS_X86

 #include "hpc/simd/ConflictCheckerTwoOneSIMDX86.hpp"

 namespace spp{


    #if HAS_AVX2

    bool checkConflictTwoOneMoveAVX2_X4(size_t &cursor,
                                        size_t &nb_constraints,
                                        const std::uint64_t* first_to_deactivate,
                                        const std::uint64_t* second_to_deactivate,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources){

            while((cursor + 15) < nb_constraints){

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

    #endif


 }


 #endif
