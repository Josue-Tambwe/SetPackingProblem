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
 #include "algorithms/localSearch/Neighborhoods.hpp"
 #include "algorithms/localSearch/RestrictedNeighborhoods.hpp"
 #include <vector>
 #include <cstdint>

 namespace spp{


    /**
     * @brief performs the 2-1 exchange while an improvement of the objective is possible
     */
    size_t twoOneNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance);


    /**
     * @brief performs the restricted 2-1 exchange while an improvement of the objective is possible
     */
    size_t restrictedTwoOneNeighborhood(std::vector<float> &scores,
                                      Solution &solution,
                                      const Params &params,
                                      const Instance &instance);


    


    
   /**
     * @brief performs the 1-1 exchange while an improvement of the objective is possible
     */
    size_t oneOneNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance);

    
    /**
     * @brief performs the 0-1 exchange while an improvement of the objective is possible
     */
    size_t zeroOneNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance);

    
        
                            



    /**
     * @brief performs the 1-2 exchange while an improvement of the objective is possible
     */
    size_t oneTwoNeighborhood(std::vector<float> &scores,
                            Solution &solution,
                            const Instance &instance);


    /**
     * @brief performs the restricted 1-2 exchange while an improvement of the objective is possible
     */
    size_t restrictedOneTwoNeighborhood(std::vector<float> &scores,
                                        Solution &solution,
                                        const Params &params,
                                        const Instance &instance);






    #if HAS_X86 && HAS_AVX2                        

    size_t twoOneNeighborhoodSIMDX86(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);

    size_t restrictedTwoOneNeighborhoodSIMDX86(std::vector<float> &scores,
                                              Solution &solution,
                                              const Params &params,
                                              const Instance &instance);





    size_t oneOneNeighborhoodSIMDX86(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);



    size_t zeroOneNeighborhoodSIMDX86(std::vector<float> &scores,
                                    Solution &solution,
                                    const Instance &instance);





    size_t oneTwoNeighborhoodSIMDX86(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);


    size_t restrictedOneTwoNeighborhoodSIMDX86(std::vector<float> &scores,
                                              Solution &solution,
                                              const Params &params,
                                              const Instance &instance);


    #endif






    #if HAS_ARM && HAS_NEON

    size_t twoOneNeighborhoodSIMDNEON(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);


    size_t restrictedTwoOneNeighborhoodSIMDARM(std::vector<float> &scores,
                                              Solution &solution,
                                              const Params &params,
                                              const Instance &instance);







    size_t oneOneNeighborhoodSIMDARM(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);



    size_t zeroOneNeighborhoodSIMDARM(std::vector<float> &scores,
                                    Solution &solution,
                                    const Instance &instance);





    size_t oneTwoNeighborhoodSIMDARM(std::vector<float> &scores,
                                   Solution &solution,
                                   const Instance &instance);

    size_t restrictedOneTwoNeighborhoodSIMDARM(std::vector<float> &scores,
                                              Solution &solution,
                                              const Params &params,
                                              const Instance &instance);



    #endif


    /**
     * @brief local search : VND (classic) = 1-2 exchange -> 1-1 exchange -> 2-1 exchange -> 0-1 exchange
     *                       VND intensification = 1-1 exchange -> 1-2 exchange -> 2-1 exchange -> 0-1 exchange
     */
    void variableNeighborhoodDescent(bool intensification,
                                     const Params &params, 
                                     Solution &solution, 
                                     const Instance &instance);




    /**
     * @brief VND local search with 1-2 and 2-1 restricted neighborhoods
     */
    void restrictedVariableNeighborhoodDescent(bool intensification,
                                              const Params &params, 
                                              Solution &solution, 
                                              const Instance &instance);
    
 }

