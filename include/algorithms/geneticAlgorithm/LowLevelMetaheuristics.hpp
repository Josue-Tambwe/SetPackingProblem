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
 * @file LowlevelMetaheuristics.hpp
 * @brief defines  low level metaheuristics (VND, Tabu Search and Simulated Annealing) used in the hyper-heuristic genetic algorithm
 * @author Josué Tambwe
 * @date 6 August 2026
 */

#pragma once

#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Parameters.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Timer.hpp"
#include "dataStructures/TabuList.hpp"
#include "algorithms/greedy/RandomizedConstruction.hpp"
#include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
#include "algorithms/tabuSearch/TabuSearchNeighborhoods.hpp"
#include "algorithms/tabuSearch/TabuSearchUtils.hpp"
#include "algorithms/simulatedAnnealing/SimulatedAnnealingNeighborhoods.hpp"
#include "algorithms/simulatedAnnealing/SimulatedAnnealingUtils.hpp"
#include <vector>
#include <cstdint>


namespace spp{

    // ------------- Variable Neighborhood Descent -----------------

    /**
     * @brief Intensified VND local search with 1-2 and 2-1 restricted neighborhoods
     */
    void variableNeighborhoodDescentImprovement(Solution &solution,
                                                const Params &params,
                                                const Instance &instance);



                            
    // ------------- Tabu Search -----------------

    /**
     * @brief performs Tabu Search with 1-2 and 2-1 restricted neighborhoods with periodic restart
     */
    void tabuSearchImprovement(double time_limit_local_search,
                               double alpha,
                               Solution &solution,
                               const Params &params,
                               const Instance &instance);


    
    
    // ------------- Simulated Annealing -----------------

    /**
     * @brief performs Simulated Annealing with 1-2 and 2-1 restricted neighborhoods with periodic restart
     */
    void simulatedAnnealingImprovement(double time_limit_local_search,
                                       double alpha,
                                       Solution &solution,
                                       const Params &params,
                                       const Instance &instance);


}
