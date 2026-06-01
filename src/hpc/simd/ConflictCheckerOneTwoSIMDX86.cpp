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
 * @file ConflictCheckerOneTwoSIMDX86.cpp
 * @author Josué Tambwe
 * @date 1 June 2026
 */

 #if HAS_X86

 #include "hpc/simd/ConflictCheckerOneTwoSIMDX86.hpp"

 namespace spp{


    #if HAS_AVX2

    bool checkConflictOneTwoMoveAVX2_X4(size_t &cursor,
                                        const size_t &nb_words,
                                        const std::uint64_t* first_to_activate,
                                        const std::uint64_t* second_to_activate,
                                        const std::uint64_t* to_deactivate,
                                        std::uint64_t* consumed_resources){

            while((cursor + 15) < nb_words){

                // loading 4  256-bit 

                // concumed resources
                __m256i occupied_after_removal_1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + cursor));
                __m256i occupied_after_removal_2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 4)));
                __m256i occupied_after_removal_3 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 8)));
                __m256i occupied_after_removal_4 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 12)));

                // first to activate
                __m256i first_to_activate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_activate + cursor));
                __m256i first_to_activate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_activate + (cursor + 4)));
                __m256i first_to_activate_3 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_activate + (cursor + 8)));
                __m256i first_to_activate_4 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_activate + (cursor + 12)));

                // second to activate
                __m256i second_to_activate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_activate + cursor));
                __m256i second_to_activate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_activate + (cursor + 4)));
                __m256i second_to_activate_3 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_activate + (cursor + 8)));
                __m256i second_to_activate_4 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_activate + (cursor + 12)));


                // to deactivate
                __m256i to_deactivate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_deactivate + cursor));
                __m256i to_deactivate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_deactivate + (cursor + 4)));
                __m256i to_deactivate_3 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_deactivate + (cursor + 8)));
                __m256i to_deactivate_4 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_deactivate + (cursor + 12)));


                // deactivation :  occupied_after_removal <- not (to_deactivate) and consumed_resources

                occupied_after_removal_1 = _mm256_andnot_si256(to_deactivate_1, occupied_after_removal_1);
                occupied_after_removal_2 = _mm256_andnot_si256(to_deactivate_2, occupied_after_removal_2);
                occupied_after_removal_3 = _mm256_andnot_si256(to_deactivate_3, occupied_after_removal_3);
                occupied_after_removal_4 = _mm256_andnot_si256(to_deactivate_4, occupied_after_removal_4);


                // checking conflict with the potential activations 

                __m256i conflict_1 = _mm256_or_si256(_mm256_and_si256(first_to_activate_1, second_to_activate_1), 
                                                     _mm256_or_si256(_mm256_and_si256(occupied_after_removal_1, first_to_activate_1), 
                                                                     _mm256_and_si256(occupied_after_removal_1, second_to_activate_1))
                                                    );

                __m256i conflict_2 = _mm256_or_si256(_mm256_and_si256(first_to_activate_2, second_to_activate_2), 
                                                     _mm256_or_si256(_mm256_and_si256(occupied_after_removal_2, first_to_activate_2), 
                                                                     _mm256_and_si256(occupied_after_removal_2, second_to_activate_2))
                                                    );

                __m256i conflict_3 = _mm256_or_si256(_mm256_and_si256(first_to_activate_3, second_to_activate_3), 
                                                     _mm256_or_si256(_mm256_and_si256(occupied_after_removal_3, first_to_activate_3), 
                                                                     _mm256_and_si256(occupied_after_removal_3, second_to_activate_3))
                                                    );

                __m256i conflict_4 = _mm256_or_si256(_mm256_and_si256(first_to_activate_4, second_to_activate_4), 
                                                     _mm256_or_si256(_mm256_and_si256(occupied_after_removal_4, first_to_activate_4), 
                                                                     _mm256_and_si256(occupied_after_removal_4, second_to_activate_4))
                                                    );



                

                // synchronization 
                __m256i all_conflict = _mm256_or_si256(_mm256_or_si256(conflict_1, conflict_2), 
                                                       _mm256_or_si256(conflict_3, conflict_4));

                // _mm256_testz_si256 returns (all_conflict & all_conflict) == 0

                if(!_mm256_testz_si256(all_conflict, all_conflict)){return false;}


                cursor += 16;
            }

        return true;
    }



    bool checkConflictOneTwoMoveAVX2_X2(size_t &cursor,
                                       const std::uint64_t* first_to_activate,
                                       const std::uint64_t* second_to_activate,
                                       const std::uint64_t* to_deactivate,
                                       std::uint64_t* consumed_resources){

        // loading 2  256-bit 

        // concumed resources
        __m256i occupied_after_removal_1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + cursor));
        __m256i occupied_after_removal_2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + (cursor + 4)));
          
        // first to activate
        __m256i first_to_activate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_activate + cursor));
        __m256i first_to_activate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_activate + (cursor + 4)));
        
        // second to activate
        __m256i second_to_activate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_activate + cursor));
        __m256i second_to_activate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_activate + (cursor + 4)));
        
        // to deactivate
        __m256i to_deactivate_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_deactivate + cursor));
        __m256i to_deactivate_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_deactivate + (cursor + 4)));
        

        // deactivation :  occupied_after_removal <- not (to_deactivate) and consumed_resources

        occupied_after_removal_1 = _mm256_andnot_si256(to_deactivate_1, occupied_after_removal_1);
        occupied_after_removal_2 = _mm256_andnot_si256(to_deactivate_2, occupied_after_removal_2);
        
        // checking conflict with the potential activations 
        __m256i conflict_1 = _mm256_or_si256(_mm256_and_si256(first_to_activate_1, second_to_activate_1), 
                                             _mm256_or_si256(_mm256_and_si256(occupied_after_removal_1, first_to_activate_1), 
                                                             _mm256_and_si256(occupied_after_removal_1, second_to_activate_1))
                                            );

        __m256i conflict_2 = _mm256_or_si256(_mm256_and_si256(first_to_activate_2, second_to_activate_2), 
                                            _mm256_or_si256(_mm256_and_si256(occupied_after_removal_2, first_to_activate_2), 
                                                            _mm256_and_si256(occupied_after_removal_2, second_to_activate_2))
                                            );

        // synchronization 
        __m256i all_conflict = _mm256_or_si256(conflict_1, conflict_2);

        

        cursor += 8;

        // _mm256_testz_si256 returns (all_conflict & all_conflict) == 0

        return (_mm256_testz_si256(all_conflict, all_conflict));             

    }



    bool checkConflictOneTwoMoveAVX2_X1(size_t &cursor,
                                       const std::uint64_t* first_to_activate,
                                       const std::uint64_t* second_to_activate,
                                       const std::uint64_t* to_deactivate,
                                       std::uint64_t* consumed_resources){

        // loading  256-bit 

        // concumed resources
        __m256i occupied_after_removal = _mm256_loadu_si256(reinterpret_cast<__m256i*>(consumed_resources + cursor));
        
        // first to activate
        __m256i first_to_activate_register = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(first_to_activate + cursor));
        
        // second to activate
        __m256i second_to_activate_register = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(second_to_activate + cursor));
        
        // to deactivate
        __m256i to_deactivate_register = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(to_deactivate + cursor));
        

        // deactivation :  occupied_after_removal <- not (to_deactivate) and consumed_resources
        occupied_after_removal = _mm256_andnot_si256(to_deactivate_register, occupied_after_removal);
        
        // checking conflict with the potential activations 
        __m256i conflict = _mm256_or_si256(_mm256_and_si256(first_to_activate_register, second_to_activate_register), 
                                             _mm256_or_si256(_mm256_and_si256(occupied_after_removal, first_to_activate_register), 
                                                             _mm256_and_si256(occupied_after_removal, second_to_activate_register))
                                            );

        
        cursor += 4;

        // _mm256_testz_si256 returns (conflict & conflict) == 0

        return (_mm256_testz_si256(conflict, conflict));             
    }



    bool checkConflictOneTwoTail(size_t &cursor,
                                const size_t &nb_words,
                                const std::uint64_t* first_to_activate,
                                const std::uint64_t* second_to_activate,
                                const std::uint64_t* to_deactivate,
                                std::uint64_t* consumed_resources){

        std::uint64_t conflict = 0ULL;

        for(size_t index = cursor; index < nb_words; index++){

            // loading
            std::uint64_t occupied_after_removal = consumed_resources[index];

            // deactivation
            occupied_after_removal &= ~to_deactivate[index];

            // checking conflict witn the potential  activation
            conflict |= (first_to_activate[index] & second_to_activate[index]) |
                        (occupied_after_removal & first_to_activate[index]) |
                        (occupied_after_removal & second_to_activate[index]);
        }

        return (conflict == 0ULL);
                    
    }



    bool checkConflictOneTwoMoveAVX2(const size_t &nb_words,
                                    const std::uint64_t* first_to_activate,
                                    const std::uint64_t* second_to_activate,
                                    const std::uint64_t* to_deactivate,
                                    std::uint64_t* consumed_resources){

        size_t cursor = 0;

        if(!checkConflictOneTwoMoveAVX2_X4(cursor, 
                                           nb_words,
                                           first_to_activate, 
                                           second_to_activate,
                                           to_deactivate,
                                           consumed_resources)){

            return true;

        }

        if((cursor + 7) < nb_words){

            if(!checkConflictOneTwoMoveAVX2_X2(cursor,
                                               first_to_activate, 
                                               second_to_activate,
                                               to_deactivate,
                                               consumed_resources)){

            return true;

            }

        }


        if((cursor + 3) < nb_words){

            if(!checkConflictOneTwoMoveAVX2_X1(cursor,
                                               first_to_activate, 
                                               second_to_activate,
                                               to_deactivate,
                                               consumed_resources)){

            return true;

            }

        }

        if(cursor < nb_words){

            if(!checkConflictOneTwoTail(cursor,
                                        nb_words,
                                        first_to_activate, 
                                        second_to_activate,
                                        to_deactivate,
                                        consumed_resources)){

            return true;

            }

        }

        return false;

    }

    #endif


 }


 #endif



 