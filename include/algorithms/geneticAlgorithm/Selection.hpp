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
 * @file Selection.hpp
 * @brief defines  methods for the selection operator that keeps a proportion of elite individuals for 
 *        the next generation in the hyper-heuristic genetic algorithm
 *
 * @author Josué Tambwe
 * @date 12 August 2026
 */

#pragma once

#include "dataStructures/Parameters.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/MoveNode.hpp"
#include "algorithms/geneticAlgorithm/GeneticAlgorithmUtils.hpp"
#include <vector>
#include <cstdint>
#include <functional>

namespace spp{


    /**
     * @brief merges two group of individuals into a single unified population
     */
    std::vector<Solution> unifyPopulation(std::vector<Solution> &first_group,
                                          std::vector<Solution> &second_group,
                                          const Instance &instance);




    /**
     * @brief selects the nb_survivors best individuals from the unified population and returns their indexes.
     */
    std::vector<size_t> computeSurvivorsIndexes(size_t nb_survivors, 
                                                std::vector<Solution> &unified_population,
                                                const Params &params,
                                                const Instance &instance);



    /**
     * @brief extracts the top-scoring survivors from the unified population using fitness-based ranking.
     */
    std::vector<Solution> computeEliteSurvivors(size_t nb_survivors, 
                                                std::vector<Solution> &unified_population,
                                                const Params &params,
                                                const Instance &instance);
}