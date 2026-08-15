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
 * @file GeneticAlgorithmOutput.hpp 
 * @author Josué Tambwe
 * @date 14 August 2026
 */

#pragma once

#include "configuration/OutputConfig.hpp"
#include "dataStructures/Parameters.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Status.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <array>


namespace spp{


    void printHeaderGeneticAlgorithm(const Params &params, 
                                     const Instance &instance);




    void computeProportionVectors(std::vector<double> &VND,
                                  std::vector<double> &TS,
                                  std::vector<double> &SA,
                                  const std::vector<std::array<double, 3>> all_proportions);




    void printGeneticAlgorithmIteration(double current_time, 
                                        size_t current_generation,
                                        size_t nb_couples,
                                        size_t nb_children,
                                        size_t nb_mutated_children,
                                        std::int64_t best_individual_objective_value,
                                        double max_population_fitness,
                                        double min_population_fitness,
                                        double average_population_fitness,
                                        double max_children_fitness,
                                        double min_children_fitness,
                                        double average_children_fitness,
                                        double max_mutated_children_fitness,
                                        double min_mutated_children_fitness,
                                        double average_mutated_children_fitness,
                                        const std::array<float, 10> &alpha_values,
                                        const std::vector<std::array<double, 3>> &all_proportions);




    void printSummaryGeneticAlgorithm(double total_time, 
                                      std::int64_t elite_objective_value,
                                      size_t total_generations,
                                      Status status);


}