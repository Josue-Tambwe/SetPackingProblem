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
 * @file PopulationGeneration.hpp
 * @brief defines  methods for generating new individual within the population in the hyper-heuristic genetic algorithm
 * @author Josué Tambwe
 * @date 6 August 2026
 */

#pragma once

#include "dataStructures/Parameters.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "hpc/thread/Thread.hpp"
#include "algorithms/greedy/RandomizedConstruction.hpp"
#include "algorithms/geneticAlgorithm/LowLevelMetaheuristics.hpp"
#include <array>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <iostream> // to remove

namespace spp{

    /**
     * @brief computes the integer allocation of individuals across low‑level metaheuristics (VND, TS, SA)
     *        based on their respective proportions. Given a total number of individuals to generate, this 
     *        function converts the three proportional weights into discrete counts that sum exactly to nb_individuals.
     */
    std::array<size_t, 3> allocateIndividualsToMetaheuristics(std::array<double, 3> proportions,
                                                              size_t nb_individuals);




    /**
     * @brief computes an even integer distribution of number of individuals to generate across the 10 alpha values.
     */
    std::array<size_t, 10> allocateIndividualsToAlphaValues(size_t nb_individuals_to_generate);





    /**
     * @brief sets the 'alpha value' for the greedy randomized construction and the 'local search method' for each individual to generate
     *        Local search methods : 'v' = Variable Neighborhood Descent
     *                               't' = Tabu Search
     *                               's' = Simulated Annealing
     */

    void setAlphaAndLocalSearch(size_t nb_individuals_to_generate,
                                std::array<size_t, 10> &nb_individuals_per_alpha_value,
                                std::vector<std::array<size_t, 3>> &nb_individuals_per_local_search, 
                                std::vector<float> &individuals_construction_alpha_value, 
                                std::vector<char> &individuals_local_search,
                                const std::array<float, 10> &alpha_values,
                                const std::vector<std::array<double, 3>> &all_proportions);




    /**
     * @brief generates an individual with a greedy randomized construction and performs a local search improvement
     */
    Solution generateIndividual(float alpha,
                                char local_search,
                                const Params &params,
                                const Instance &instance);




    /**
     * @brief generates multiple individuals (construction + local search) on a single CPU thread
     */
    void generateIndividualsSingleThread(int start,
                                         int end,
                                         std::vector<Solution> &individuals,
                                         const std::vector<float> &individuals_construction_alpha_value,
                                         const std::vector<char> &individuals_local_search,
                                         const Params &params,
                                         const Instance &instance);


    /**
     * @brief generates multiple individuals (construction + local search) on multiple CPU threads
     */
    std::vector<Solution> generateIndividuals(const std::vector<float> &individuals_construction_alpha_value,
                                              const std::vector<char> &individuals_local_search,
                                              const Params &params,
                                              const Instance &instance);




    /**
     * @brief updates the proportions of the three low‑level local search methods (VND, TS, SA) for 
     *        each alpha value based on the performance of the individuals generated. 
     */
    void updateLowLevelLocalSearchProportions(std::vector<std::array<double, 3>> &all_proportions,
                                              const std::array<size_t, 10> &nb_individuals_per_alpha_value,
                                              const std::vector<std::array<size_t, 3>> &nb_individuals_per_local_search,
                                              std::vector<Solution> &individuals,
                                              const Instance &instance);

}