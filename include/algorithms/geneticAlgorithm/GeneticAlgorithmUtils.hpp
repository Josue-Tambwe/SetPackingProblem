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
#include <array>
#include <algorithm>

namespace spp{

    const std::array<float, 10> alpha_values_GA = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f,0.9f, 1.0f};

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





    /**
     * @brief check of the stopping criteria for the Genetic Algorithm
     */
    bool stoppingCriteriaGeneticAlgorithm(double current_time, 
                                          size_t current_generation, 
                                          const Params &params);




    /**
     * @brief finds the fitness top-scoring individual within the population
     */
    Solution findBestIndividual(std::vector<Solution> &population, 
                                const Instance &instance);




    /**
     * @brief computes fitness statistics : maximum, minimum and average values
     */
    void computeMaximumMinimumAverageFitness(double &max_fitness,
                                             double &min_fitness,
                                             double &average_fitness,
                                             const std::vector<double> &fitness);



    void computeMaximumMinimumAverageMutationFitness(double &max_fitness,
                                                     double &min_fitness,
                                                     double &average_fitness,
                                                     const std::vector<size_t> children_to_mutate_indexes,
                                                     const std::vector<double> &fitness);

}