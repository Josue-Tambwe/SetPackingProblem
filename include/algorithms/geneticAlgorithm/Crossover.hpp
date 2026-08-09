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
 * @file Crossover.hpp
 * @brief defines  methods for the crossover operator in the hyper-heuristic genetic algorithm
 * @author Josué Tambwe
 * @date 8 August 2026
 */

#pragma once

#include "dataStructures/Parameters.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "hpc/thread/Thread.hpp"
#include "algorithms/geneticAlgorithm/GeneticAlgorithmUtils.hpp"
#include <vector>
#include <cstdint>
#include <limits>
#include <cmath>
#include <unordered_set>
#include <iterator>


namespace spp{


    /**
     * @brief finds the minimum fitness value within the population
     */
    double findMinimumFitness(const std::vector<double> &population_fitness);



    /**
     * @brief computes a quadratic biased fitness. new fitness = (old fitness - minimum fitness + epsilon)²
     */
    std::vector<double> computeQuadraticBiasedFitness(const std::vector<double> &population_fitness);



    /**
     * @brief normalize quadratic biased fitness. new fitness = old fitness / cumulative fitness
     */
    void normalizeBiasedFitness(std::vector<double> &biased_population_fitness);




    /**
     * @brief computes the cumulative biased fitness proportion after normalization
     */
    std::vector<double> computeCumulativeQuadraticBiasedFitness(const std::vector<double> &population_fitness);



    /**
     * @brief computes crossover participation using the Stochastic Universal Sampling (SUS) principle.
     */
    std::vector<size_t> computeIndividualsCrossOverParticipation(const std::vector<double> &cumulative_population_fitness,
                                                                 const Params &params);




    /**
     * @brief determines initial and guiding parents according to fitness.
     */
    void setInitialAndGuidingParents(size_t inner_index,
                                     size_t first_parent_index,
                                     size_t second_parent_index,
                                     std::vector<size_t> &initial_parents,
                                     std::vector<size_t> &guiding_parents,
                                     const std::vector<double> &population_fitness);




    /**
     * @brief gets the indexes of individuals with non-zero crossover participation
     */
    std::unordered_set<size_t> getParentIndexes(const std::vector<size_t> &crossover_participation);





    /**
     * @brief gets a random parent index within the set of parent indexes
     */
    size_t getRandomIndex(const std::unordered_set<size_t> &parents_indexes);






    /**
     * @brief builds crossover couples and assigns initial and guiding parents based on fitness and participation.
     */
    void setCrossoverCouples(std::vector<size_t> &initial_parents,
                             std::vector<size_t> &guiding_parents,
                             std::vector<size_t> &crossover_participation,
                             const std::vector<double> &population_fitness,
                             const Params &params);

}