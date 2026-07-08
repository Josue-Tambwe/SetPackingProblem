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
 * @file GRASPOutput.hpp 
 * @author Josué Tambwe
 * @date 8 July 2026
 */

#pragma once

#include "configuration/OutputConfig.hpp"
#include "dataStructures/Parameters.hpp"
#include "dataStructures/Instance.hpp"
#include "dataStructures/Status.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>


namespace spp{


    void printHeaderGRASP(const Params &params, 
                          const Instance &instance);
}
