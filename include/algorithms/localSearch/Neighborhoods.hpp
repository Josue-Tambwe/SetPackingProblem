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


 }

