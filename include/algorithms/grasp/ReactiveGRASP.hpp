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
 * @file ReactiveGRASP.hpp
 * @brief defines the Reactive Greedy Randomized Adaptative Search Procedure (GRASP) method for the Set Packing Solver
 * @author Josué Tambwe
 * @date 2 July 2026
 */

#pragma once

#include "configuration/GeneralConfig.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Parameters.hpp"
#include "output/Logger.hpp"
#include "algorithms/greedy/RandomizedConstruction.hpp"
#include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
#include <array>
#include <vector> 


namespace spp{


    /**
     * @brief different alpha values used in the GRASP algorithm
     */
    const std::array<float, 10> alpha_values = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f,0.9f, 1.0f};



    /**
     * @brief generates a solution by a greedy randomized construction and a Variable Neighboorhood Descent local search
     */
    Solution constructAndImproveSolution(float alpha, 
                                         const Params &params, 
                                         const Instance &instance);


}