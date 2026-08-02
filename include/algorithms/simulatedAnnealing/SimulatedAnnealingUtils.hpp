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
 * @file SimulatedAnnealingUtils.hpp
 * @brief defines some useful functions used in the Simulated Annealing local search
 * @author Josué Tambwe
 * @date 2 August 2026
 */

#pragma once

#include "dataStructures/Parameters.hpp"
#include "hpc/thread/Thread.hpp"


namespace spp{


    /**
     * @brief Approximates the Boltzmann exponential factor exp(-x) using a low-cost polynomial approximation
     *        f(x) = 1 / (1 + x + x²), since the exact exponential is too expensive to compute in tight inner loops.
     */
    double polynomialBoltzmannApproximation(double x);




    /**
     * @brief Evaluates whether a worsening move should be accepted based on the simulated‑annealing rule
     *        using the degradation and current temperature to compute the acceptance probability.
     */
    bool acceptanceCriterion(double degradation, double current_temperature);




    /**
     * @brief check of the stopping criteria for the Tabu Search algorithm
     */
    bool stoppingCriteriaSimulatedAnnealing(double current_time, 
                                            size_t current_iteration,
                                            double current_temperature,
                                            const Params &params);

}