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
 * @file RandomizedConstruction.hpp
 * @brief defines the randomized greedy construction method of the Set Packing Solver
 * @author Josué Tambwe
 * @date 29 June 2026
 */

#pragma once

#include "configuration/GeneralConfig.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "output/Logger.hpp"
#include "hpc/thread/Thread.hpp"
#include <unordered_set>
#include <vector>
#include <limits>
#include <cmath>


namespace spp{

    /**
     * @brief computes the heuristic score of each variable : score = profit / nb resources required
     */
    std::vector<float> computeScore(const Instance &instance);



    /**
     * @brief finds the  minimum and maximum scores among free variables
     */
    void findMinAndMaxScores(float &min_score, 
                             float &max_score,
                             int &max_score_index, 
                             std::vector<float> &scores, 
                             std::unordered_set<int> &free_variables);



    /**
     * @brief builds the Restricted Candidates List (RCL) in the Greedy Randomized Adaptative Search Procedure (GRASP) formalism
     */
    std::vector<int> buildRestrictedCandidatesList(float min_score, 
                                                   float max_score,
                                                   float alpha, 
                                                   std::vector<float> &scores, 
                                                   std::unordered_set<int> &free_variables);



    /**
     * @brief selects randomdly a variable within the Restricted Candidates List (RCL)
     */
    int selectVariableRandomdlyInRCL(int max_score_index, 
                                     float alpha, 
                                     std::vector<int> &RCL);


    /**
     * @brief updates the solution by setting the selected variable to 'one' and all variables in conflict 
     *        with the selected variable to 'zero' => removing them from the set of free variables
     */
    void updateSolutionRandomizedConstruction(int selected_var,
                                              std::unordered_set<int> &free_variables,
                                              Solution &solution,
                                              const Instance &instance);



    /**
     * @brief constructs a solution with a randomized greedy method
     */
    Solution randomizedConstruction(float alpha, const Instance &instance);


}