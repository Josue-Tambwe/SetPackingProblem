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
 * @file ConflictCheckerScalar.hpp
 * @brief defines functions that check conflicting variables during the the 
 *        local search in order to ensure feasibility of solutions
 * @author Josué Tambwe
 * @date 28 May 2026
 */

 #pragma once


 #include <vector>
 #include <cstdint>


 namespace spp{


    /**
     * @brief checks conflict in a 2-1 exchange move (two variables are set to '0' and one variable is set to '1') 
     *        by performing 4 checks of 64-bit words  every iteration (loop unrolling)
     */
    bool checkConflictTwoOneMove_X4(size_t &cursor,
                                    const std::vector<std::uint64_t> &first_to_deactivate,
                                    const std::vector<std::uint64_t> &second_to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources);


    /**
     * @brief checks conflict in a 2-1 exchange move by performing 2 checks of 64-bit words
     */
    bool checkConflictTwoOneMove_X2(size_t &cursor,
                                    const std::vector<std::uint64_t> &first_to_deactivate,
                                    const std::vector<std::uint64_t> &second_to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources);

    /**
     * @brief checks conflict in a 2-1 exchange move by performing one check of 64-bit word
     */
    bool checkConflictTwoOneMove_X1(size_t &cursor,
                                    const std::vector<std::uint64_t> &first_to_deactivate,
                                    const std::vector<std::uint64_t> &second_to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources);



    /**
     * @brief returns 'true' when there is a 'conflict' therefore the exchange is infeasible
     */
    bool checkConflictTwoOneMove(const std::vector<std::uint64_t> &first_to_deactivate,
                                 const std::vector<std::uint64_t> &second_to_deactivate,
                                 const std::vector<std::uint64_t> &to_activate,
                                 std::vector<std::uint64_t> &consumed_resources);



   ////////////////////////////////////////////////
   // ALL OTHER MOVES FOLLOWS THE SAME STRUCTURE //
   ///////////////////////////////////////////////


   bool checkConflictOneOneMove_X4(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources);


   bool checkConflictOneOneMove_X2(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources);


   bool checkConflictOneOneMove_X1(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_deactivate,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources);


   bool checkConflictOneOneMove(const std::vector<std::uint64_t> &to_deactivate,
                                 const std::vector<std::uint64_t> &to_activate,
                                 std::vector<std::uint64_t> &consumed_resources);





   bool checkConflictZeroOneMove_X4(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources);


   bool checkConflictZeroOneMove_X2(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources);


   bool checkConflictZeroOneMove_X1(size_t &cursor,
                                    const std::vector<std::uint64_t> &to_activate,
                                    std::vector<std::uint64_t> &consumed_resources);


   bool checkConflictZeroOneMove(const std::vector<std::uint64_t> &to_activate,
                                 std::vector<std::uint64_t> &consumed_resources);





      
   bool checkConflictOneTwoMove_X4(size_t &cursor,
                                   const std::vector<std::uint64_t> &first_to_activate,
                                   const std::vector<std::uint64_t> &second_to_activate,
                                   const std::vector<std::uint64_t> &to_deactivate,
                                   std::vector<std::uint64_t> &consumed_resources);


   bool checkConflictOneTwoMove_X2(size_t &cursor,
                                   const std::vector<std::uint64_t> &first_to_activate,
                                   const std::vector<std::uint64_t> &second_to_activate,
                                   const std::vector<std::uint64_t> &to_deactivate,
                                   std::vector<std::uint64_t> &consumed_resources);


   bool checkConflictOneTwoMove_X1(size_t &cursor,
                                   const std::vector<std::uint64_t> &first_to_activate,
                                   const std::vector<std::uint64_t> &second_to_activate,
                                   const std::vector<std::uint64_t> &to_deactivate,
                                   std::vector<std::uint64_t> &consumed_resources);


   bool checkConflictOneTwoMove(const std::vector<std::uint64_t> &first_to_activate,
                                const std::vector<std::uint64_t> &second_to_activate,
                                const std::vector<std::uint64_t> &to_deactivate,
                                std::vector<std::uint64_t> &consumed_resources);





 }