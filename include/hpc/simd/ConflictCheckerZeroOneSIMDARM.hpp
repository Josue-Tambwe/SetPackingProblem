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
 * @file ConflictCheckerZeroOneSIMDARM.hpp
 * @brief defines functions that check conflicting variables during the 0-1 exchange 
 *        in order to ensure feasibility of solutions with SIMD vectorization
 *        for ARM Instruction Set Architecture
 * 
 * @author Josué Tambwe
 * @date 4 June 2026
 */

#pragma once

#if HAS_ARM

#include <vector>
#include <cstdint>
#include <arm_neon.h>


namespace spp{

    #if HAS_NEON


    /**
     * @brief checks conflict in a 0-1 exchange move (no variable is set to '0' and one variable is set to '1') 
     *        by performing 4 checks of 128 bits every iteration (loop unrolling) : 512 bits per iteration
     */
    bool checkConflictZeroOneMoveNEON_X4(size_t &cursor,
                                        const size_t &nb_words,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources);


    /**
     * @brief checks conflict in a 0-1 exchange move (no variable is set to '0' and one variable is set to '1') 
     *        by performing 2 checks of 128 bits
     */
    bool checkConflictZeroOneMoveNEON_X2(size_t &cursor,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources);


    /**
     * @brief checks conflict in a 0-1 exchange move (no variable is set to '0' and one variable is set to '1') 
     *        by performing one check of 128 bits 
     */
    bool checkConflictZeroOneMoveNEON_X1(size_t &cursor,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources);


    /**
    * @brief Handles the remaining single 64‑bit constraint when the SIMD blocks
    *        (X4, X2, X1) cannot fully cover the end of the vector.
    */
    bool checkConflictZeroOneMoveNEONTail(size_t &cursor,
                                        const std::uint64_t* to_activate,
                                        std::uint64_t* consumed_resources);



    /**
     * @brief returns 'true' when there is a 'conflict' therefore the exchange is infeasible
     */
    bool checkConflictZeroOneMoveNEON(const size_t &nb_words,
                                     const std::uint64_t* to_activate,
                                     std::uint64_t* consumed_resources);


    #endif
}

#endif
