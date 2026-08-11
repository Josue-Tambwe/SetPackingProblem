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
 * @file Mutation.hpp
 * @brief defines  methods for the mutation operator based on Iterated Local Search in the hyper-heuristic genetic algorithm
 * @author Josué Tambwe
 * @date 11 August 2026
 */

#pragma once

#include "dataStructures/Parameters.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "hpc/thread/Thread.hpp"
#include "algorithms/grasp/PathRelinking.hpp"
#include "algorithms/localSearch/VariableNeighborhoodDescent.hpp"
#include "algorithms/geneticAlgorithm/GeneticAlgorithmUtils.hpp"
#include <vector>
#include <cstdint>
#include <cmath>
#include <unordered_set>
#include <iterator>

namespace spp{


    /**
     * @brief gets a random parent index within the set of non-zero variables
     */
    size_t getRandomIndex(const std::unordered_set<int> &non_zero_vars);





    /**
     * @brief deactivates randomly  a fraction (20%) of non-zero variables in the current solution.
     */
    void computeDisturbedSolution(Solution &solution,
                                  const Instance &instance);




    /**
     * @brief selects random distinct child indexes to mutate.
     */
    std::vector<size_t> selectIndexesToMutate(size_t nb_children, const Params &params);





    /**
     * @brief performs offspring mutation on a single CPU thread
     */
    void performMutationSingleThread(int start,
                                     int end,
                                     std::vector<Solution> &children,
                                     const std::vector<size_t> &children_to_mutate_indexes,
                                     const Params &params,
                                     const Instance &instance);





    /**
     * @brief performs offspring mutation on multiple CPU threads
     */
    void performMutation(std::vector<Solution> &children,
                         const std::vector<size_t> &children_to_mutate_indexes,
                         const Params &params,
                         const Instance &instance);

}