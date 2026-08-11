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
 * @file GeneticAlgorithmUtils.hpp
 * @brief defines some useful functions used in the hyper-heuristic genetic algorithm
 * @author Josué Tambwe
 * @date 8 August 2026
 */

#pragma once

#include "dataStructures/Parameters.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "hpc/thread/Thread.hpp"
#include <vector>
#include <cstdint>

namespace spp{


    /**
     * @brief computes the fitness  on a single CPU thread
     */
    void computePopulationFitnessOneThread(int start,
                                           int end,
                                           std::vector<double> &population_fitness,
                                           std::vector<Solution> &population,
                                           const Instance &instance);




    /**
     * @brief computes the fitness of each individual within the population (fitness = objective value)
     */
    std::vector<double> computePopulationFitness(std::vector<Solution> &population,
                                                 const Params &params,
                                                 const Instance &instance);



    /**
     * @brief gets a random parent index within the set of  indexes
     */
    size_t getRandomIndex(const std::unordered_set<size_t> &indexes);

}