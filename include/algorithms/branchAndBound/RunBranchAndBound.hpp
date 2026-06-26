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
 * @file RunBranchAndBound.hpp
 * @brief implements the Branch and Bound (B&B) algorithm
 * @author Josué Tambwe
 * @date 26 June 2026
 */

#pragma once

#if USE_BRANCH_AND_BOUND

#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Timer.hpp"
#include "output/Logger.hpp"
#include "dataStructures/Parameters.hpp"
#include "algorithms/branchAndBound/BestFirst.hpp"
#include "algorithms/branchAndBound/DepthFirst.hpp"
#include "algorithms/greedy/DeterministicConstruction.hpp"
#include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
#include "algorithms/milpSolver/LPSolver.hpp"
#include "algorithms/branchAndBound/BaBUtils.hpp"

#if HAS_GUROBI
#include "algorithms/milpSolver/GurobiBackend.hpp"
#endif
 
#if HAS_HIGHS
#include "algorithms/milpSolver/HighsBackend.hpp"
#endif


#include <vector>
#include <stdexcept>




namespace spp{


    /**
     * @brief initializes the greedy primal solution for the Branch And Bound algorithm
     */
    Solution initializePrimalSolution(const Params &params, 
                                      const Instance &instance);


    /**
     * @brief initializes the primal bound
     */
    double setInitialPrimalBound(Solution &primal_solution,
                                 const Instance &instance);


    /**
     * @brief sets the LP solver to use (Gurobi or Highs)
     */
    std::unique_ptr<LPSolver> setLPSolver(const Params &params);



    /**
     * @brief builds the model for linear relaxation
     */
    std::unique_ptr<LPSolver> buildLinearModel(const Params &params,
                                               const Instance &instance);



    /**
     * @brief updates the best known primal solution
     */
    Solution updateSolution(std::vector<double> &sol, const Instance &instance);



    /**
     * @brief runs the B&B algorithm with the best-first node exploration strategy
     */
    void branchAndBoundBestFirst(const Params &params);



    /**
     * @brief runs the B&B algorithm with the depth-first node exploration strategy
     */
    void branchAndBoundDepthFirst(const Params &params);



    /**
     * @brief defines the generic Branch And Bound Algorithm
     */
    void runBaB(const Params &params);


}


#endif