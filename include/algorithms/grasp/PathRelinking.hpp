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
 * @file PathRelinking.hpp
 * @brief defines the Path-Relinking intensification method for the Reactive GRASP algorithm
 * @author Josué Tambwe
 * @date 11 July 2026
 */

#pragma once

#include "configuration/GeneralConfig.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Parameters.hpp"
#include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
#include "algorithms/grasp/GRASPUtils.hpp"
#include "hpc/thread/Thread.hpp"
#include <array>
#include <vector> 
#include <cstdint>
#include <unordered_set>


namespace spp{


    /**
     * @brief performs iterations (construction + local search) for a single CPU thread and saves the local best solution found
     */
    void runSingleThreadLocalEliteGeneration(const std::array<float, 10> &alpha_probabilities,
                                             std::array<float, 10> &alpha_cumulative_scores,
                                             std::array<float, 10> &alpha_selection_count,
                                             const int nb_iterations,
                                             std::vector<Solution> &all_local_best_solutions,
                                             int thread_id,
                                             const Params &params,
                                             const Instance &instance);





    /**
     * @brief performs iterations (construction + local search) for multiple CPU thread and saves each local best solution found
     */
    std::vector<Solution> runMultiThreadLocalEliteGeneration(std::array<float, 10> &alpha_probabilities,
                                                             Solution &best_solution,
                                                             const Params &params,
                                                             const Instance &instance);




    /**
     * @brief finds the index of the guinding (target) solution for the path-relinking process
     */
    size_t findGuidingSolutionIndex(Solution &current_elite_solution, 
                                    std::vector<Solution> &all_local_best_solutions,
                                    const Instance &instance);




    /**
     * @brief sets the guiding solution for the path-relinking process
     */
    Solution setGuidingSolution(size_t guiding_solution_index,
                                Solution &current_elite_solution,
                                std::vector<Solution> &all_local_best_solutions);




    /**
     * @brief computes the pool of initial elite solution (starting point) for the path-relinking process
     */
    std::vector<Solution> computeInitialSolutionPool(size_t guiding_solution_index, 
                                                     Solution &current_elite_solution, 
                                                     std::vector<Solution> &all_local_best_solutions);


    /**
     * @brief computes the set of non-zero variables indexes of the initial elite (starting point) solution
     */
    std::unordered_set<int> computeInitialSolutionNonZeroVarsIndexes(Solution &initial_solution);


    

    /**
     * @brief deactivates all activated variables which compete for ressources with the variable to active (currently deactivated)
     *        in order to maintain solution feasibility
     */
    void deactivateConflictingVariables(int index_to_activate,
                                        std::unordered_set<int> &non_zero_vars_set, 
                                        Solution &solution, 
                                        const Instance &instance);




    /**
     * @brief explores the relinking path between the initial solution and the guiding solution,
     *        while evaluating intermediate solutions and updating the best elite solution found along the path
     */
    void exploreRelinkingPath(Solution &initial_solution,
                              Solution guiding_solution,
                              Solution &local_elite_solution,
                              const Params &params,
                              const Instance &instance);


    

    /**
     * @brief explores the relinking path between the pool of initial solutions and the guiding solution with CPU multi-threading
     */
    std::vector<Solution> exploreMultiRelinkingPath(std::vector<Solution> &initial_solutions_pool,
                                                    Solution &guiding_solution,
                                                    const Params &params,
                                                    const Instance &instance);




    /**
     * @brief retrieves the elite solution within the local elite solutions pool
     */
    Solution retrieveEliteSolution(const std::vector<Solution> &local_elite_pool,
                                   const Instance &instance);



    /**
     * @brief performs one cycle (construction + local search) for multiple CPU thread and updates alpha probabilities
     */
    Solution runOnePathRelinkingCycle(std::array<float, 10> &alpha_probabilities,
                                      Solution &current_elite_solution,
                                      const Params &params,
                                      const Instance &instance);




    


}