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
 * @file ConflictCheckerTwoOneSIMDX86.hpp
 * @brief defines functions that check conflicting variables during the the 
 *        local search in order to ensure feasibility of solutions with SIMD vectorization
 *        X86 Instruction Set Architecture
 * 
 * @author Josué Tambwe
 * @date 31 May 2026
 */

 #pragma once

 #if HAS_X86

 #include <vector>
 #include <cstdint>
 #include <immintrin.h>

 namespace spp{


    
    #if HAS_AVX2
    
    /**
     * @brief checks conflict in a 2-1 exchange move (two variables are set to '0' and one variable is set to '1') 
     *        by performing 4 checks of 256 bits (4 64-bit words)  every iteration (loop unrolling) : 1024 bits per iteration
     */
    bool checkConflictTwoOneMoveAVX2_X4(size_t &cursor,
                                        size_t &nb_constraints,
                                        const std::uint64_t* first_to_deactivate,
                                        const std::uint64_t* second_to_deactivate,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources);

        
    #endif

 }

 #endif