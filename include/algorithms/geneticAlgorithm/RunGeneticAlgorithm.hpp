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
 * @file RunGeneticAlgorithm.hpp
 * @brief implements the selection-based hyper-heuristic Genetic Algorithm  for the Set Packing Solver
 * @author Josué Tambwe
 * @date 12 August 2026
 */

#pragma once

#include "configuration/GeneralConfig.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Status.hpp"
#include "dataStructures/Parameters.hpp"
#include "output/Logger.hpp"
#include "dataStructures/Timer.hpp"
#include "algorithms/geneticAlgorithm/GeneticAlgorithmUtils.hpp"
#include "algorithms/geneticAlgorithm/PopulationGeneration.hpp"
#include "algorithms/geneticAlgorithm/Crossover.hpp"
#include "algorithms/geneticAlgorithm/Mutation.hpp"
#include "algorithms/geneticAlgorithm/Selection.hpp"
#include "output/GeneralOutput.hpp"
#include <array>
#include <vector> 
#include <cstdint>
#include <iostream> // to remove


namespace spp{


    /**
     * @brief runs the selection-based hyper-heuristic Genetic Algorithm
     */
    void runGeneticAlgorithm(const Params &params);

}