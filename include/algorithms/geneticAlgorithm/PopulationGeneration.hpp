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
#include <array>
#include <cmath>
#include <vector>

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
                                std::vector<float> &individuals_construction_alpha_value, 
                                std::vector<char> &individuals_local_search,
                                const std::array<float, 10> &alpha_values,
                                const std::vector<std::array<double, 3>> all_proportions);

}