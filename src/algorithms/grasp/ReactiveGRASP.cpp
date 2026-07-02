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
 * @file ReactiveGRASP.cpp
 * @author Josué Tambwe
 * @date 2 July 2026
 */

#include "algorithms/grasp/ReactiveGRASP.hpp"


namespace spp{


    Solution constructAndImproveSolution(float alpha, 
                                         const Params &params, 
                                         const Instance &instance){
        
        // construction
        Solution solution = randomizedConstruction(alpha, instance);

        // local search
        variableNeighborhoodDescent(params.use_intensification,
                                    params, 
                                    solution, 
                                    instance);

        return solution;
    }


}