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
 * @file Neighborhoods.hpp
 * @brief defines  all neighborhoods  used in  local search
 * @author Josué Tambwe
 * @date 29 May 2026
 */

 #pragma once

 #include "dataStructures/MoveNode.hpp"
 #include "dataStructures/Instance.hpp"
 #include "dataStructures/Solution.hpp"
 #include "algorithms/localSearch/ConflictCheckerScalar.hpp"

 #if HAS_X86
 #include "hpc/simd/ConflictCheckerTwoOneSIMDX86.hpp" // 
 #include "hpc/simd/ConflictCheckerOneOneSIMDX86.hpp" //
 #include "hpc/simd/ConflictCheckerZeroOneSIMDX86.hpp" //
 #include "hpc/simd/ConflictCheckerOneTwoSIMDX86.hpp" //
 #endif
 
 #include <vector>

 namespace spp{


    /**
     * @brief searches a feasible 2-1 exchange move : 2 variables to deactivate and 1 variable to activate
     *        the vector of currently activated variables (nonzero variables) is sorted in increasing order of the heuristic score
     *        the vector of currently deactivated variables (zero variables) is sorted in decreasing order of the heuristic score
     *        heuristic score : profit / nb resources required
     */
    bool findTwoOneExchange(int &first_index_to_deactivate,
                            int &second_index_to_deactivate,
                            int &index_to_activate,
                            std::vector<int> &sorted_activated_vars,
                            std::vector<int> &sorted_deactivated_vars,
                            Solution &solution,
                            const Instance &instance);


   /**
    * @brief searches a feasible 1-1 exchange move : 1 variable to deactivate and 1 variable to activate
    */
   bool findOneOneExchange(int &index_to_deactivate,
                           int &index_to_activate,
                           std::vector<int> &sorted_activated_vars,
                           std::vector<int> &sorted_deactivated_vars,
                           Solution &solution,
                           const Instance &instance);


   /**
    * @brief searches a feasible 0-1 exchange move : 0 variable to deactivate and 1 variable to activate
    */
   bool findZeroOneExchange(int &index_to_activate,
                            std::vector<int> &sorted_deactivated_vars,
                            Solution &solution,
                            const Instance &instance);


   /**
    * @brief finds an upper bound for the loop on the first index to activate (currently deactivated variable)
    *        This bound reduces the number of iteration because of this large 1-2 neighborhood
    */
   size_t findIterationBoundFirstToActivate(int to_deactivate_index,
                                            std::vector<int> &sorted_deactivated_vars,
                                            const Instance &instance);



   /**
    * @brief searches a feasible 1-2 exchange move : 1 variable to deactivate and 2 variables to activate
    */
   bool findOneTwoExchange(int &to_deactivate,
                           int &first_index_to_activate,
                           int &second_index_to_activate,
                           std::vector<int> &sorted_activated_vars,
                           std::vector<int> &sorted_deactivated_vars,
                           Solution &solution,
                           const Instance &instance);




   #if HAS_X86

   bool findTwoOneExchangeSIMDX86(int &first_index_to_deactivate,
                                 int &second_index_to_deactivate,
                                 int &index_to_activate,
                                 std::vector<int> &sorted_activated_vars,
                                 std::vector<int> &sorted_deactivated_vars,
                                 Solution &solution,
                                 const Instance &instance);




   bool findOneOneExchangeSIMDX86(int &index_to_deactivate,
                                 int &index_to_activate,
                                 std::vector<int> &sorted_activated_vars,
                                 std::vector<int> &sorted_deactivated_vars,
                                 Solution &solution,
                                 const Instance &instance);


   bool findZeroOneExchangeSIMDX86(int &index_to_activate,
                                 std::vector<int> &sorted_deactivated_vars,
                                 Solution &solution,
                                 const Instance &instance);



   bool findOneTwoExchangeSIMDX86(int &to_deactivate,
                                 int &first_index_to_activate,
                                 int &second_index_to_activate,
                                 std::vector<int> &sorted_activated_vars,
                                 std::vector<int> &sorted_deactivated_vars,
                                 Solution &solution,
                                 const Instance &instance);

   #endif


 }

