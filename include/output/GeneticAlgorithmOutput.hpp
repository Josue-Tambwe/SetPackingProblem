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


namespace spp{


    void printHeaderGeneticAlgorithm(const Params &params, 
                                     const Instance &instance);


}