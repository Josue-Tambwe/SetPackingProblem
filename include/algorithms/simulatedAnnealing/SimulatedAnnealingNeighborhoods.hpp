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
 * @file SimulatedAnnealingNeighborhoods.hpp
 * @brief defines  neighborhoods used in the Simulated Annealing local search
 * @author Josué Tambwe
 * @date 1 August 2026
 */

#pragma once

#include "dataStructures/Instance.hpp"
#include "dataStructures/Solution.hpp"
#include "dataStructures/Parameters.hpp"
#include "dataStructures/Status.hpp"
#include "algorithms/localSearch/RestrictedNeighborhoods.hpp"
#include <vector>
#include <cstdint>
#include <limits>



namespace spp{


    // ------------------ 2-1 Exchange --------------------

    /**
     * @brief finds the best feasible move in the 2-1 neighborhood
     */

    bool findRestrictedTwoOneExchangeScalarSimulatedAnnealing(int &first_index_to_deactivate,
                                                            int &second_index_to_deactivate,
                                                            int &index_to_activate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
                                                            const Params &params,
                                                            const Instance &instance);

    #if HAS_X86 && HAS_AVX2

    bool findRestrictedTwoOneExchangeSIMDX86SimulatedAnnealing(int &first_index_to_deactivate,
                                                            int &second_index_to_deactivate,
                                                            int &index_to_activate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
                                                            const Params &params,
                                                            const Instance &instance);

    #endif



    #if HAS_ARM && HAS_NEON

    bool findRestrictedTwoOneExchangeSIMDARMSimulatedAnnealing(int &first_index_to_deactivate,
                                                            int &second_index_to_deactivate,
                                                            int &index_to_activate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
                                                            const Params &params,
                                                            const Instance &instance);

    #endif



    bool findRestrictedTwoOneExchangeSimulatedAnnealing(Solution &solution,
                                                        std::vector<float> &scores,
                                                        const Params &params,
                                                        const Instance &instance);







    // ------------------ 1-1 Exchange --------------------

    /**
     * @brief finds the best feasible move in the 1-1 neighborhood
     */

    bool findOneOneExchangeScalarSimulatedAnnealing(int &index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance);


    #if HAS_X86 && HAS_AVX2

    bool findOneOneExchangeSIMDX86SimulatedAnnealing(int &index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance);

    #endif



    #if HAS_ARM && HAS_NEON

    bool findOneOneExchangeSIMDARMSimulatedAnnealing(int &index_to_deactivate,
                                                    int &index_to_activate,
                                                    std::vector<int> &sorted_activated_vars,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance);

    #endif




    bool findOneOneExchangeSimulatedAnnealing(Solution &solution,
                                            std::vector<float> &scores,
                                            const Params &params,
                                            const Instance &instance);






    
    // ------------------ 0-1 Exchange --------------------

    /**
     * @brief finds the best feasible move in the 0-1 neighborhood
     */

    bool findZeroOneExchangeScalarSimulatedAnnealing(int &index_to_activate,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance);


    
    #if HAS_X86 && HAS_AVX2

    bool findZeroOneExchangeSIMDX86SimulatedAnnealing(int &index_to_activate,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance);

    #endif



    #if HAS_ARM && HAS_NEON

    bool findZeroOneExchangeSIMDARMSimulatedAnnealing(int &index_to_activate,
                                                    std::vector<int> &sorted_deactivated_vars,
                                                    Solution &solution,
                                                    const Instance &instance);

    #endif




    bool findZeroOneExchangeSimulatedAnnealing(Solution &solution,
                                            std::vector<float> &scores,
                                            const Params &params,
                                            const Instance &instance);








    // ------------------ 1-2 Exchange --------------------

    /**
     * @brief finds the best feasible move in the 1-2 neighborhood
     */


    bool findRestrictedOneTwoExchangeScalarSimulatedAnnealing(int &first_index_to_activate,
                                                            int &second_index_to_activate,
                                                            int &to_deactivate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
                                                            const Params &params,
                                                            const Instance &instance);



    #if HAS_X86 && HAS_AVX2

    bool findRestrictedOneTwoExchangeSIMDX86SimulatedAnnealing(int &first_index_to_activate,
                                                            int &second_index_to_activate,
                                                            int &to_deactivate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
                                                            const Params &params,
                                                            const Instance &instance);

    #endif



    #if HAS_ARM && HAS_NEON

    bool findRestrictedOneTwoExchangeSIMDARMSimulatedAnnealing(int &first_index_to_activate,
                                                            int &second_index_to_activate,
                                                            int &to_deactivate,
                                                            std::vector<int> &sorted_activated_vars,
                                                            std::vector<int> &sorted_deactivated_vars,
                                                            Solution &solution,
                                                            const Params &params,
                                                            const Instance &instance);

    #endif




    bool findRestrictedOneTwoExchangeSimulatedAnnealing(Solution &solution,
                                                        std::vector<float> &scores,
                                                        const Params &params,
                                                        const Instance &instance);
    


}