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
 * @file GeneralConfig.hpp
 * @brief defines substancial elements for the configuration of the SPP solver
 * @author Josué Tambwe
 * @date 20 May 2026
 */
 
 #pragma once
 
 #ifndef NUMBER_PHYSICAL_CORES
    #define NUMBER_PHYSICAL_CORES 1 // default value (1 physical core)
 #endif

 #ifndef HAS_GUROBI
    #define HAS_GUROBI false
 #endif

 #ifndef HAS_HIGHS
    #define HAS_HIGHS false
 #endif

 #ifndef HAS_HEXALY
   #define HAS_HEXALY false
 #endif

 #ifndef USE_BRANCH_AND_BOUND
   #define USE_BRANCH_AND_BOUND false
 #endif

 #ifndef USE_MILP
   #define USE_MILP false
 #endif
 
const float epsilon = 1e-6f; // to avoid division by zero