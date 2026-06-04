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
 * @file VariableNeighborhoodDescent.hpp
 * @brief defines  functions used in the VND local search
 * @author Josué Tambwe
 * @date 28 May 2026
 */

 #pragma once

 #include "dataStructures/MoveNode.hpp"
 #include "dataStructures/Instance.hpp"
 #include "dataStructures/Solution.hpp"
 #include "dataStructures/Parameters.hpp"
 #include "dataStructures/Status.hpp"
 #include "algorithms/localSearch/NeighborhoodUtils.hpp"
 #include "algorithms/localSearch/Neighborhoods.hpp"
 #include <vector>
 #include <cstdint>

 namespace spp{


    /**
     * @brief performs the 2-1 exchange while an improvement of the objective is possible
     */
    void twoOneNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance);

    
   /**
     * @brief performs the 1-1 exchange while an improvement of the objective is possible
     */
    void oneOneNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance);

    
    /**
     * @brief performs the 0-1 exchange while an improvement of the objective is possible
     */
    void zeroOneNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance);

    

    /**
     * @brief performs the 1-2 exchange while an improvement of the objective is possible
     */
    void oneTwoNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance);


    #if HAS_AVX2                        

    void twoOneNeighborhoodSIMDX86(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);



    void oneOneNeighborhoodSIMDX86(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);



    void zeroOneNeighborhoodSIMDX86(std::vector<float> &scores,
                                    Solution &solution,
                                    const Instance &instance);



    void oneTwoNeighborhoodSIMDX86(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);

    #endif




    #if HAS_NEON

    void twoOneNeighborhoodSIMDNEON(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);



    void oneOneNeighborhoodSIMDARM(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);



    void zeroOneNeighborhoodSIMDARM(std::vector<float> &scores,
                                    Solution &solution,
                                    const Instance &instance);



    void oneTwoNeighborhoodSIMDARM(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);



    #endif


    /**
     * @brief local search : VND (classic) = 1-2 exchange -> 1-1 exchange -> 2-1 exchange -> 0-1 exchange
     *                       VND intensification = 1-1 exchange -> 1-2 exchange -> 2-1 exchange -> 0-1 exchange
     */
    void variableNeighborhoodDescent(const Params &params, 
                                     Solution &solution, 
                                     const Instance &instance);
    
 }

