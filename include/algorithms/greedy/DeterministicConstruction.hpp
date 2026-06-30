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
 * @file DeterministicConstruction.hpp
 * @brief defines the deterministic greedy construction method of the Set Packing Solver
 * @author Josué Tambwe
 * @date 26 May 2026
 */

 #pragma once
 
 #include "configuration/GeneralConfig.hpp"
 #include "dataStructures/Instance.hpp"
 #include "dataStructures/Solution.hpp"
 #include "dataStructures/Status.hpp"
 #include "output/Logger.hpp"
 #include <unordered_set>
 #include <vector> 


 namespace spp{


    /**
     * @brief computes the heuristic score of all variables and initializes the set of free variables
     */
    void initialization(std::vector<float> &scores,
                        std::unordered_set<int> &free_variables,
                        const spp::Instance &instance);


    /**
     * @brief returns the index of the variable (among free variables) with the greatest heuristic score
     */
    int findBestVariable(std::vector<float> &scores, 
                         std::unordered_set<int> &free_variables);


    /**
     * @brief updates the solution by setting the best variable to 'one' and all variables in conflict 
     *        with the best variable to 'zero' => removing them from the set of free variables
     */
    void updateSolution(int best_var,
                        std::unordered_set<int> &free_variables,
                        spp::Solution &solution,
                        const spp::Instance &instance);


 
    /**
     * @brief constructs a solution with the deterministic greedy method
     */
    spp::Solution deterministicConstruction(const spp::Instance &instance);


 }
 